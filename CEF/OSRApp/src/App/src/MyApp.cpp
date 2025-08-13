#include "MyApp.hpp"
#include "MyClient.hpp"

#include "include/wrapper/cef_helpers.h"

// CefBrowserProcessHandler
CefRefPtr<CefBrowserProcessHandler> MyApp::GetBrowserProcessHandler()
{
  return this;
};

// CefBrowserProcessHandler
void MyApp::OnContextInitialized()
{
  CEF_REQUIRE_UI_THREAD();
  CefBrowserSettings browser_settings;
  browser_settings.windowless_frame_rate = 60;

  CefWindowInfo window_info;
  window_info.shared_texture_enabled = 0;
  window_info.SetAsWindowless(0);

  const auto client = this->GetDefaultClient();
  const auto url = "https://vuejs.org/";
  CefBrowserHost::CreateBrowserSync(window_info, client, url, browser_settings, nullptr, nullptr);
}

// CefBrowserProcessHandler
CefRefPtr<CefClient> MyApp::GetDefaultClient()
{
  return MyClient::GetInstance();
};