#pragma once
#include "include/cef_client.h"
#include <list>

class MyClient : public CefClient, CefLifeSpanHandler, CefDisplayHandler
{
private:
  IMPLEMENT_REFCOUNTING(MyClient);

private:
  bool m_isClosing{ false };
  std::list<CefRefPtr<CefBrowser>> m_BrowserList;

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
  auto ShowMainWindow() -> void;
  auto CloseAllBrowsers(const bool isForceClose) -> void;
  auto IsClosing() const -> bool;
  static auto GetInstance() -> CefRefPtr<MyClient>;

  // CefLifeSpanHandler
public:
  CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override { return this; };
  void OnAfterCreated(CefRefPtr<CefBrowser> browser) override;
  bool DoClose(CefRefPtr<CefBrowser> browser) override;
  void OnBeforeClose(CefRefPtr<CefBrowser> browser) override;

  // CefDisplayHandler
public:
  CefRefPtr<CefDisplayHandler> GetDisplayHandler() override { return this; }
  void OnTitleChange(CefRefPtr<CefBrowser> browser, const CefString& title) override;
};
