#pragma once
#include <cstddef>
#include <list>
#include "Renderer/IRenderer.hpp"
#include "include/cef_client.h"

class MyClient : public CefClient, CefLifeSpanHandler, CefRenderHandler
{
private:
  IMPLEMENT_REFCOUNTING(MyClient);

private:
  bool m_isClosing{ false };
  std::list<CefRefPtr<CefBrowser>> m_BrowserList;
  IRenderer* m_pRenderer{};

private:
  static CefRefPtr<MyClient> m_upInstance;

private:
  MyClient() = default;

public:
  MyClient(const MyClient&) = delete;
  MyClient(MyClient&&) noexcept = delete;
  auto operator=(const MyClient&) -> MyClient& = delete;
  auto operator=(MyClient&&) noexcept -> MyClient& = delete;

public:
  virtual ~MyClient() = default;

public:
  static auto GetInstance() -> CefRefPtr<MyClient>;
  auto SetRenderer(IRenderer* pRenderer) -> void;
  auto GetMainBrowser() -> CefRefPtr<CefBrowser>;
  auto CloseAllBrowsers(const bool isForceClose) -> void;
  auto GetBrowserCount() -> size_t { return m_BrowserList.size(); }
  auto IsClosing() const -> bool;

  // CefLifeSpanHandler
public:
  CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override { return this; };
  void OnAfterCreated(CefRefPtr<CefBrowser> browser) override;
  bool DoClose(CefRefPtr<CefBrowser> browser) override;
  void OnBeforeClose(CefRefPtr<CefBrowser> browser) override;

  // CefRenderHandler
public:
  CefRefPtr<CefRenderHandler> GetRenderHandler() override { return this; }
  void GetViewRect(CefRefPtr<CefBrowser> browser, CefRect& rect) override;
  void OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList& dirtyRects, const void* buffer, int width, int height) override;
  void OnAcceleratedPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList& dirtyRects, const CefAcceleratedPaintInfo& info) override
  {
    // Forward the platform-specific shared handle to the renderer which will
    // open the shared resource and present it. This keeps D3D code inside the
    // renderer implementation.
    if (m_pRenderer)
    {
      m_pRenderer->UpdateSharedTextureFromHandle(info.shared_texture_handle, m_pRenderer->GetWidth(), m_pRenderer->GetHeigh());
    }
  }
};
