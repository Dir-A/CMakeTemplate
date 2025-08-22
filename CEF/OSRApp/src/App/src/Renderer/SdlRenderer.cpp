#include "SdlRenderer.hpp"
#include "include/cef_app.h"
#include <algorithm>

SdlRenderer::SdlRenderer()
{
  SDL_Init(SDL_INIT_EVERYTHING);
  m_pWindow = SDL_CreateWindow("CEF OSR", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_nWidth, m_nHeigh, SDL_WINDOW_RESIZABLE);
  m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC); // NOLINT
  m_pTexture = SDL_CreateTexture(m_pRenderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, m_nWidth, m_nHeigh);

  SDL_DisplayMode current_mode;
  SDL_GetCurrentDisplayMode(SDL_GetWindowDisplayIndex(m_pWindow), &current_mode);
  m_nFrameRate = std::min(current_mode.refresh_rate, 120);
}

SdlRenderer::~SdlRenderer()
{
  SDL_DestroyTexture(m_pTexture);
  SDL_DestroyRenderer(m_pRenderer);
  SDL_DestroyWindow(m_pWindow);
}

void SdlRenderer::UpdateTexture(const std::vector<CefRect>& dirtyRects, const void* buffer, int width, int height)
{
  int pitch{};
  void* texture_pixels{};
  if (SDL_LockTexture(m_pTexture, nullptr, &texture_pixels, &pitch) != 0)
  {
    return;
  }

  std::memcpy(texture_pixels, buffer, width * height * 4);
  SDL_UnlockTexture(m_pTexture);
}

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

    SDL_SetRenderDrawColor(m_pRenderer, 255, 255, 255, 255);
    SDL_RenderClear(m_pRenderer);

    SDL_Rect rect{ 0, 0, m_nWidth, m_nHeigh };
    SDL_RenderCopy(m_pRenderer, m_pTexture, nullptr, &rect);
    SDL_RenderPresent(m_pRenderer);

    frame_end = SDL_GetTicks();

    frame_time = frame_end - frame_beg;
    if (frame_delay > frame_time)
    {
      SDL_Delay(frame_delay - frame_time);
    }
  }
}
