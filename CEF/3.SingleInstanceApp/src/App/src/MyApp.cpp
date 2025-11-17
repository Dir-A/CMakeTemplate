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

  bool MyApp::OnAlreadyRunningAppRelaunch(CefRefPtr<CefCommandLine> command_line, const CefString& current_directory)
  {
// we can focus on the existing window
#ifdef _WIN32
    if (const auto handle = ::FindWindowW(nullptr, L"My Native Window App"); handle)
    {
      if (::IsIconic(handle))
      {
        ::ShowWindow(handle, SW_RESTORE);
      }
      else
      {
        ::SetForegroundWindow(handle);
      }
    }
#endif
    return true;
  }

} // namespace CEFDemo
