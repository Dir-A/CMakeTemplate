#pragma once
#include "include/cef_app.h"

class MyApp : public CefApp, CefBrowserProcessHandler
{
private:
  IMPLEMENT_REFCOUNTING(MyApp);

public:
  MyApp() = default;

  // CefBrowserProcessHandler
public:
  CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() override;
  void OnContextInitialized() override;
  CefRefPtr<CefClient> GetDefaultClient() override;
};
