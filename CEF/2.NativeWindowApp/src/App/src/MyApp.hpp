#pragma once
#include "include/cef_app.h"

namespace CEFDemo
{
  class MyApp : public CefApp, CefBrowserProcessHandler
  {
  private:
    IMPLEMENT_REFCOUNTING(MyApp);

  private:
  public:
    MyApp() = default;
    MyApp(const MyApp&) = delete;
    MyApp(MyApp&&) noexcept = delete;
    MyApp& operator=(const MyApp&) = delete;
    MyApp& operator=(MyApp&&) noexcept = delete;
    virtual ~MyApp() = default;

    // CefBrowserProcessHandler
  public:
    CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() override { return this; }
    CefRefPtr<CefClient> GetDefaultClient() override;
    void OnContextInitialized() override;
  };

} // namespace CEFDemo
