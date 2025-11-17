#pragma once
#include <list>
#include "include/cef_client.h"

namespace CEFDemo
{
  class MyClient : public CefClient, CefLifeSpanHandler
  {
  private:
    IMPLEMENT_REFCOUNTING(MyClient);

  private:
    bool m_isClosing{ false };
    std::list<CefRefPtr<CefBrowser>> m_BrowserList;

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
    auto ShowMainWindow() -> void;
    auto CloseAllBrowsers(const bool isForceClose) -> void;
    auto IsClosing() const -> bool;
    static auto GetInstance() -> CefRefPtr<MyClient>;

    // CefLifeSpanHandler
  public:
    CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override { return this; };
    void OnAfterCreated(CefRefPtr<CefBrowser> browser) override;
    void OnBeforeClose(CefRefPtr<CefBrowser> browser) override;
    bool DoClose(CefRefPtr<CefBrowser> browser) override;
  };

} // namespace CEFDemo
