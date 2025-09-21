#pragma once
#include <SDL.h>
#include "IRenderer.hpp"

#if defined(_WIN32)
#include <d3d11_1.h>
#include <dxgi.h>
#include <SDL_syswm.h>
#pragma comment(lib, "d3d11.lib")
#endif

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
  // Windows D3D specific
#if defined(_WIN32)
  IDXGISwapChain* m_pSwapChain{};
  ID3D11RenderTargetView* m_pBackBufferRTV{};
  ID3D11Texture2D* m_pBackBufferTexture{};
  // shared texture opened from CEF
  ID3D11Texture2D* m_pTextureD3D{};
  bool m_useD3DPresent{ false };
#endif

public:
  SdlRenderer();
  SdlRenderer(const SdlRenderer&) = delete;
  SdlRenderer(SdlRenderer&&) noexcept = delete;
  SdlRenderer& operator=(const SdlRenderer&) = delete;
  SdlRenderer& operator=(SdlRenderer&&) noexcept = delete;
  virtual ~SdlRenderer();

  // Open a shared texture handle (platform specific). On Windows this should
  // be a HANDLE-compatible pointer provided by CEF's CefAcceleratedPaintInfo.
  void UpdateSharedTextureFromHandle(const void* shared_handle, int width, int height) override;

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
