#include "MyApp.hpp"
#include "MyClient.hpp"
#include "include/wrapper/cef_helpers.h"

// CefBrowserProcessHandler
CefRefPtr<CefBrowserProcessHandler> MyApp::GetBrowserProcessHandler() { return this; };

// CefBrowserProcessHandler
void MyApp::OnContextInitialized()
{
  CEF_REQUIRE_UI_THREAD();
  CefBrowserSettings browser_settings;
  const auto client = this->GetDefaultClient();
  const auto url = "https://www.baidu.com/";
  CefBrowserHost::CreateBrowser({}, client, url, browser_settings, nullptr, nullptr);
}

// CefBrowserProcessHandler
CefRefPtr<CefClient> MyApp::GetDefaultClient() { return MyClient::GetInstance(); };
