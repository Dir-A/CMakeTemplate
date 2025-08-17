#include "MyApp.hpp"
#include "MyClient.hpp"
#include "Delegate/MyWndDelegate.hpp"
#include "Delegate/MyBrowserViewDelegate.hpp"

#include "include/internal/cef_ptr.h"
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
  const auto client = this->GetDefaultClient();
  const auto url = "https://vuejs.org/";

  const auto browser_view = CefBrowserView::CreateBrowserView(client, url, browser_settings, nullptr, nullptr, new MyBrowserViewDelegate{});
  const auto cef_window = CefWindow::CreateTopLevelWindow(new MyWndDelegate{ browser_view });
  cef_window->CenterWindow({ 1920, 1080 });
}

// CefBrowserProcessHandler
CefRefPtr<CefClient> MyApp::GetDefaultClient()
{
  return MyClient::GetInstance();
};