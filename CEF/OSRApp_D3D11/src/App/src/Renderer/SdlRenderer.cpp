#include "SdlRenderer.hpp"
#include "include/cef_app.h"
#include <algorithm>
#include <stdexcept>
#include <vector>
#if defined(_WIN32)
// Windows-specific helpers
// D3D device/context are stored as translation-unit statics and reused.
static ID3D11Device1* m_pDevice = nullptr;
static ID3D11DeviceContext* m_pDeviceContext = nullptr;

void CreateD3DDeviceIfNeeded()
{
  if (m_pDevice)
    return;

  HRESULT hr;
  ID3D11Device* base_device{};
  D3D_FEATURE_LEVEL featureLevels[]{ D3D_FEATURE_LEVEL_11_0 };
  hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_SINGLETHREADED, featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION, &base_device, nullptr, &m_pDeviceContext);
  if (FAILED(hr))
  {
    throw std::runtime_error("Failed to create D3D11 device");
  }

  hr = base_device->QueryInterface(__uuidof(ID3D11Device1), (void**)&m_pDevice);
  base_device->Release();
  base_device = nullptr;
  if (FAILED(hr) || !m_pDevice)
  {
    throw std::runtime_error("Failed to query ID3D11Device1");
  }
}
#endif
SdlRenderer::SdlRenderer()
{
  SDL_Init(SDL_INIT_EVERYTHING);
  m_pWindow = SDL_CreateWindow("CEF OSR", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_nWidth, m_nHeigh, SDL_WINDOW_RESIZABLE);
  // We still create an SDL renderer for event handling, but GPU presentation is
  // done via D3D swapchain directly.
  m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC); // NOLINT

#if defined(_WIN32)
  // Create D3D device/context and DXGI swapchain bound to the SDL window.
  CreateD3DDeviceIfNeeded();

  // Obtain native HWND from SDL
  SDL_SysWMinfo wmInfo{};
  SDL_VERSION(&wmInfo.version);
  if (SDL_GetWindowWMInfo(m_pWindow, &wmInfo))
  {
    HWND hwnd = wmInfo.info.win.window;

    // Create swapchain
    IDXGIDevice* dxgiDevice = nullptr;
    if (SUCCEEDED(m_pDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice)))
    {
      IDXGIAdapter* adapter = nullptr;
      if (SUCCEEDED(dxgiDevice->GetAdapter(&adapter)))
      {
        IDXGIFactory* factory = nullptr;
        if (SUCCEEDED(adapter->GetParent(__uuidof(IDXGIFactory), (void**)&factory)))
        {
          DXGI_SWAP_CHAIN_DESC sd{};
          sd.BufferCount = 1;
          sd.BufferDesc.Width = m_nWidth;
          sd.BufferDesc.Height = m_nHeigh;
          sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
          sd.BufferDesc.RefreshRate.Numerator = 0;
          sd.BufferDesc.RefreshRate.Denominator = 1;
          sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
          sd.OutputWindow = hwnd;
          sd.SampleDesc.Count = 1;
          sd.Windowed = TRUE;
          sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

          IDXGISwapChain* swapchain = nullptr;
          if (SUCCEEDED(factory->CreateSwapChain(m_pDevice, &sd, &swapchain)))
          {
            m_pSwapChain = swapchain;
            // Create RTV for backbuffer
            ID3D11Texture2D* backBuf = nullptr;
            if (SUCCEEDED(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuf)))
            {
              m_pBackBufferTexture = backBuf;
              m_pDevice->CreateRenderTargetView(m_pBackBufferTexture, nullptr, &m_pBackBufferRTV);
              m_useD3DPresent = true;
            }
          }

          factory->Release();
        }
        adapter->Release();
      }
      dxgiDevice->Release();
    }
  }
#endif

  SDL_DisplayMode current_mode;
  SDL_GetCurrentDisplayMode(SDL_GetWindowDisplayIndex(m_pWindow), &current_mode);
  m_nFrameRate = std::min(current_mode.refresh_rate, 300);
}

SdlRenderer::~SdlRenderer()
{
  // Release D3D resources first (if present)
#if defined(_WIN32)
  if (m_pBackBufferRTV)
  {
    m_pBackBufferRTV->Release();
    m_pBackBufferRTV = nullptr;
  }
  if (m_pBackBufferTexture)
  {
    m_pBackBufferTexture->Release();
    m_pBackBufferTexture = nullptr;
  }
  if (m_pSwapChain)
  {
    m_pSwapChain->Release();
    m_pSwapChain = nullptr;
  }
  if (m_pTextureD3D)
  {
    m_pTextureD3D->Release();
    m_pTextureD3D = nullptr;
  }
  if (m_pDeviceContext)
  {
    m_pDeviceContext->Release();
    m_pDeviceContext = nullptr;
  }
  if (m_pDevice)
  {
    m_pDevice->Release();
    m_pDevice = nullptr;
  }
#endif
  SDL_DestroyTexture(m_pTexture);
  SDL_DestroyRenderer(m_pRenderer);
  SDL_DestroyWindow(m_pWindow);
}

void SdlRenderer::UpdateTexture(const std::vector<CefRect>& dirtyRects, const void* buffer, int width, int height)
{
  // CPU path removed - renderer uses GPU present only.
  (void)dirtyRects;
  (void)buffer;
  (void)width;
  (void)height;
}

#if defined(_WIN32)
void SdlRenderer::UpdateSharedTextureFromHandle(const void* shared_handle, int /*width*/, int /*height*/)
{
  if (!shared_handle)
    return;

  CreateD3DDeviceIfNeeded();

  ID3D11Texture2D* openedTexture = nullptr;
  HRESULT hr = m_pDevice->OpenSharedResource1(const_cast<void*>(shared_handle), __uuidof(ID3D11Texture2D), (void**)&openedTexture);
  if (FAILED(hr) || !openedTexture)
  {
    return;
  }

  // Replace previously opened shared texture
  if (m_pTextureD3D)
  {
    m_pTextureD3D->Release();
    m_pTextureD3D = nullptr;
  }
  m_pTextureD3D = openedTexture;
}
#endif

void SdlRenderer::HandleMouseEvent(SDL_Event& rfEvent, const CefRefPtr<CefBrowser>& rfBrowser)
{
  auto fn_get_mouse_status = [](SDL_MouseButtonEvent const& rfButtonEvent) -> std::pair<int, CefBrowserHost::MouseButtonType> {
    int modifiers = EVENTFLAG_NONE;
    CefBrowserHost::MouseButtonType button_type{};

    {
      const auto mod_state = SDL_GetModState();

      if (mod_state & KMOD_CAPS)
      {
        modifiers |= EVENTFLAG_CAPS_LOCK_ON;
      }
      if (mod_state & KMOD_SHIFT)
      {
        modifiers |= EVENTFLAG_SHIFT_DOWN;
      }
      if (mod_state & KMOD_CTRL)
      {
        modifiers |= EVENTFLAG_CONTROL_DOWN;
      }
      if (mod_state & KMOD_ALT)
      {
        modifiers |= EVENTFLAG_ALT_DOWN;
      }
    }

    switch (rfButtonEvent.button)
    {
    case SDL_BUTTON_X1:
    case SDL_BUTTON_LEFT:
      button_type = MBT_LEFT;
      modifiers |= EVENTFLAG_LEFT_MOUSE_BUTTON;
      break;
    case SDL_BUTTON_MIDDLE:
      button_type = MBT_MIDDLE;
      modifiers |= EVENTFLAG_MIDDLE_MOUSE_BUTTON;
      break;
    case SDL_BUTTON_X2:
    case SDL_BUTTON_RIGHT:
      button_type = MBT_RIGHT;
      modifiers |= EVENTFLAG_RIGHT_MOUSE_BUTTON;
      break;
    }

    return { modifiers, button_type };
  };

  const auto [modifiers, button_type] = fn_get_mouse_status(rfEvent.button);

  CefMouseEvent mouse_event;
  mouse_event.modifiers = modifiers;

  switch (rfEvent.type)
  {
  case SDL_MOUSEBUTTONUP: {
    mouse_event.x = rfEvent.button.x;
    mouse_event.y = rfEvent.button.y;
    rfBrowser->GetHost()->SendMouseClickEvent(mouse_event, button_type, true, 1);
  }
  break;
  case SDL_MOUSEBUTTONDOWN: {
    mouse_event.x = rfEvent.button.x;
    mouse_event.y = rfEvent.button.y;
    rfBrowser->GetHost()->SendMouseClickEvent(mouse_event, button_type, false, 1);
  }
  break;
  case SDL_MOUSEMOTION: {
    mouse_event.x = rfEvent.motion.x;
    mouse_event.y = rfEvent.motion.y;
    rfBrowser->GetHost()->SendMouseMoveEvent(mouse_event, false);
  }
  break;
  case SDL_MOUSEWHEEL: {
    mouse_event.x = rfEvent.wheel.mouseX;
    mouse_event.y = rfEvent.wheel.mouseY;
    rfBrowser->GetHost()->SendMouseWheelEvent(mouse_event, rfEvent.wheel.x * 150, rfEvent.wheel.y * 150);
  }
  break;
  }
}

void SdlRenderer::HanldeWindwEvent(SDL_Event& rfEvent, const CefRefPtr<CefBrowser>& rfBrowser)
{
  switch (rfEvent.window.event)
  {
  case SDL_WINDOWEVENT_RESIZED:
  case SDL_WINDOWEVENT_SIZE_CHANGED: {
    m_nWidth = rfEvent.window.data1;
    m_nHeigh = rfEvent.window.data2;
    SDL_DestroyTexture(m_pTexture);
    m_pTexture = SDL_CreateTexture(m_pRenderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, m_nWidth, m_nHeigh);
    rfBrowser->GetHost()->WasResized();
  }
  break;
  default:
    break;
  }
}

bool SdlRenderer::HandleEvent(const CefRefPtr<CefBrowser>& rfBrowser)
{
  while (SDL_PollEvent(&m_Event) != 0)
  {
    switch (m_Event.type)
    {
    case SDL_QUIT:
      return false;
      break;
    case SDL_MOUSEBUTTONDOWN:
    case SDL_MOUSEBUTTONUP:
    case SDL_MOUSEMOTION:
    case SDL_MOUSEWHEEL:
      this->HandleMouseEvent(m_Event, rfBrowser); // NOLINT
      break;
    case SDL_WINDOWEVENT:
      this->HanldeWindwEvent(m_Event, rfBrowser); // NOLINT
      break;
    }
  }

  return true;
}

void SdlRenderer::Run(const CefRefPtr<CefBrowser>& rfBrowser)
{
  Uint32 frame_beg{};
  Uint32 frame_end{};
  Uint32 frame_time{};
  const Uint32 frame_delay = 1000 / m_nFrameRate;

  while (true)
  {
    frame_beg = SDL_GetTicks();

    if (!this->HandleEvent(rfBrowser))
    {
      break;
    }

    CefDoMessageLoopWork();

    // GPU present path: copy shared D3D texture to swapchain backbuffer and present.
#if defined(_WIN32)
    if (m_useD3DPresent && m_pSwapChain && m_pTextureD3D && m_pBackBufferTexture)
    {
      // Copy shared texture to backbuffer
      m_pDeviceContext->CopyResource(m_pBackBufferTexture, m_pTextureD3D);
      // Present
      m_pSwapChain->Present(1, 0);
    }
#else
    // No GPU path on this platform.
#endif

    frame_end = SDL_GetTicks();

    frame_time = frame_end - frame_beg;
    if (frame_delay > frame_time)
    {
      SDL_Delay(frame_delay - frame_time);
    }
  }
}
