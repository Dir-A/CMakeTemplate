#pragma once
#include <SDL.h>
#include <mutex>
#include "IRenderer.hpp"

class SdlRenderer : public IRenderer
{
private:
  SDL_Window* m_pWindow{};
  SDL_Renderer* m_pRenderer{};
  SDL_Texture* m_pTexture{};
  SDL_Event m_Event{};
  int m_nWidth{ 1920 };
  int m_nHeigh{ 1080 };
  int m_nFrameRate{ 60 };

public:
  SdlRenderer();
  SdlRenderer(const SdlRenderer&) = delete;
  SdlRenderer(SdlRenderer&&) noexcept = delete;
  SdlRenderer& operator=(const SdlRenderer&) = delete;
  SdlRenderer& operator=(SdlRenderer&&) noexcept = delete;
  virtual ~SdlRenderer();

protected:
  int GetWidth() override { return m_nWidth; }
  int GetHeigh() override { return m_nHeigh; }
  void UpdateTexture(const std::vector<CefRect>& dirtyRects, const void* buffer, int width, int height) override;

private:
  static void HandleMouseEvent(SDL_Event& rfEvent, const CefRefPtr<CefBrowser>& rfBrowser);
  void HanldeWindwEvent(SDL_Event& rfEvent, const CefRefPtr<CefBrowser>& rfBrowser);
  bool HandleEvent(const CefRefPtr<CefBrowser>& rfBrowser);

public:
  int GetFrameRate() const { return m_nFrameRate; }
  void Run(const CefRefPtr<CefBrowser>& rfBrowser);
};
