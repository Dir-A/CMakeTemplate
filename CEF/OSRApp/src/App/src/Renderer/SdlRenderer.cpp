#include "SdlRenderer.hpp"
#include "include/cef_app.h"
#include <algorithm>
#include <stdexcept>
#include <vector>
#include <windef.h>

SdlRenderer::SdlRenderer()
{
  SDL_Init(SDL_INIT_EVERYTHING);
  m_pWindow = SDL_CreateWindow("CEF OSR", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_nWidth, m_nHeigh, SDL_WINDOW_RESIZABLE);
#if defined(_WIN32)
  SDL_SysWMinfo wmInfo{}; // NOLINT
  SDL_VERSION(&wmInfo.version);
  if (SDL_GetWindowWMInfo(m_pWindow, &wmInfo))
  {
    this->CreateD3DDevice(wmInfo.info.win.window); // NOLINT
  }
#endif

  SDL_DisplayMode current_mode;
  SDL_GetCurrentDisplayMode(SDL_GetWindowDisplayIndex(m_pWindow), &current_mode);
  m_nFrameRate = std::min(current_mode.refresh_rate, 300);
}

SdlRenderer::~SdlRenderer()
{
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

void SdlRenderer::CreateD3DDevice(HWND hwnd)
{
  ComPtr<ID3D11Device> base_device;
  D3D_FEATURE_LEVEL feature_levels[]{ D3D_FEATURE_LEVEL_11_0 };                                                                                                                                                                                                                                  // NOLINT
  const HRESULT hres = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, D3D11_CREATE_DEVICE_BGRA_SUPPORT | D3D11_CREATE_DEVICE_SINGLETHREADED, feature_levels, ARRAYSIZE(feature_levels), D3D11_SDK_VERSION, base_device.GetAddressOf(), nullptr, m_pDeviceContext.GetAddressOf()); // NOLINT
  if (FAILED(hres))
  {
    return;
  }

  const auto hres_2 = base_device.As(&m_pDevice);
  if (FAILED(hres_2) || !m_pDevice)
  {
    return;
  }

  ComPtr<IDXGIDevice> dxgi_device;
  if (FAILED(m_pDevice.As(&dxgi_device)))
  {
    return;
  }

  ComPtr<IDXGIAdapter> adapter;
  if (FAILED(dxgi_device->GetAdapter(adapter.GetAddressOf())))
  {
    return;
  }

  ComPtr<IDXGIFactory> factory;
  if (FAILED(adapter->GetParent(__uuidof(IDXGIFactory), (void**)factory.GetAddressOf()))) // NOLINT
  {
    return;
  }

  DXGI_SWAP_CHAIN_DESC dxgi_sd{};
  dxgi_sd.BufferCount = 1;
  dxgi_sd.BufferDesc.Width = m_nWidth;
  dxgi_sd.BufferDesc.Height = m_nHeigh;
  dxgi_sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
  dxgi_sd.BufferDesc.RefreshRate.Numerator = 0;
  dxgi_sd.BufferDesc.RefreshRate.Denominator = 1;
  dxgi_sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  dxgi_sd.OutputWindow = hwnd;
  dxgi_sd.SampleDesc.Count = 1;
  dxgi_sd.Windowed = TRUE;
  dxgi_sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

  if (FAILED(factory->CreateSwapChain(m_pDevice.Get(), &dxgi_sd, m_pSwapChain.GetAddressOf())))
  {
    return;
  }

  if (FAILED(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)m_pBackBufferTexture.GetAddressOf()))) // NOLINT
  {
    return;
  }

  m_pDevice->CreateRenderTargetView(m_pBackBufferTexture.Get(), nullptr, m_pBackBufferRTV.GetAddressOf());

  m_useD3DPresent = true;
}

void SdlRenderer::UpdateSharedTextureFromHandle(const void* shared_handle, int width, int height)
{
  if (!shared_handle)
  {
    return;
  }

  if ((width != m_nWidth) || (height != m_nHeigh))
  {
    return;
  }

  ComPtr<ID3D11Texture2D> opened_texture;
  const auto hres = m_pDevice->OpenSharedResource1(const_cast<void*>(shared_handle), __uuidof(ID3D11Texture2D), (void**)opened_texture.GetAddressOf()); // NOLINT
  if (FAILED(hres) || !opened_texture)
  {
    return;
  }

  m_pTextureD3D = opened_texture;
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

#if defined(_WIN32)
    if (m_useD3DPresent && m_pTextureD3D)
    {
      m_pDeviceContext->CopyResource(m_pBackBufferTexture.Get(), m_pTextureD3D.Get());
      m_pSwapChain->Present(1, 0);
      m_pTextureD3D.Reset();
    }
#endif

    frame_end = SDL_GetTicks();

    frame_time = frame_end - frame_beg;
    if (frame_delay > frame_time)
    {
      SDL_Delay(frame_delay - frame_time);
    }
  }
}
