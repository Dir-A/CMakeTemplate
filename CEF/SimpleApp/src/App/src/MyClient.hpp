#pragma once
#include "include/cef_client.h"
#include <list>

class MyClient : public CefClient, CefLifeSpanHandler
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
  static auto GetInstance() -> CefRefPtr<MyClient>;
  virtual ~MyClient() override;

  // CefLifeSpanHandler
public:
  CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override { return this; };
  void OnAfterCreated(CefRefPtr<CefBrowser> browser) override;
  bool DoClose(CefRefPtr<CefBrowser> browser) override;
  void OnBeforeClose(CefRefPtr<CefBrowser> browser) override;

public:
  auto ShowMainWindow() -> void;
  auto CloseAllBrowsers(const bool isForceClose) -> void;
  auto IsClosing() const -> bool;
};
