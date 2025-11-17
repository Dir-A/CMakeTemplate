#include "MyApp.hpp"
#include "MyClient.hpp"
#include "MyWindowDelegate.hpp"

namespace CEFDemo
{
  CefRefPtr<CefClient> MyApp::GetDefaultClient()
  {
    return MyClient::GetInstance();
  }

  void MyApp::OnContextInitialized()
  {
    CefBrowserSettings browser_settings;
    const auto client = this->GetDefaultClient();
    const auto url = "https://vuejs.org/";

    const auto browser_view = CefBrowserView::CreateBrowserView(client, url, browser_settings, nullptr, nullptr, new MyBrowserViewDelegate{});
    const auto cef_window = CefWindow::CreateTopLevelWindow(new MyWndDelegate{ browser_view });
    cef_window->CenterWindow({ 1280, 720 });
    cef_window->SetTitle("My Native Window App");
  }

} // namespace CEFDemo
