#include "MyApp.hpp"
#include "MyClient.hpp"
#include "Delegate/MyWndDelegate.hpp"
#include "Delegate/MyBrowserViewDelegate.hpp"

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
  const auto url = "https://www.baidu.com/";

  CefRefPtr<CefBrowserViewDelegate> browser_view_delegate{ new MyBrowserViewDelegate{} };
  CefRefPtr<CefBrowserView> browser_view = CefBrowserView::CreateBrowserView(client, url, browser_settings, nullptr, nullptr, browser_view_delegate);
  CefRefPtr<CefWindowDelegate> wnd_delegate{ new MyWndDelegate{ browser_view } };
  CefWindow::CreateTopLevelWindow(wnd_delegate);
}

// CefBrowserProcessHandler
CefRefPtr<CefClient> MyApp::GetDefaultClient()
{
  return MyClient::GetInstance();
};