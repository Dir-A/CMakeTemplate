#include "MyWindowDelegate.hpp"

namespace CEFDemo
{
  MyWndDelegate::MyWndDelegate(const CefRefPtr<CefBrowserView>& browser_view, const bool /*isDevtools*/)
      : m_BrowserView{ browser_view }
  {
  }

  void MyWndDelegate::OnWindowCreated(CefRefPtr<CefWindow> window)
  {
    window->AddChildView(m_BrowserView);
    window->Show();
  }

  void MyWndDelegate::OnWindowDestroyed(CefRefPtr<CefWindow> window)
  {
    m_BrowserView = nullptr;
  }

  bool MyWndDelegate::CanClose(CefRefPtr<CefWindow> window)
  {
    const auto browser = m_BrowserView->GetBrowser();
    return browser ? browser->GetHost()->TryCloseBrowser() : true;
  }

  bool MyWndDelegate::CanResize(CefRefPtr<CefWindow> window)
  {
    return true;
  }

  bool MyBrowserViewDelegate::OnPopupBrowserViewCreated(CefRefPtr<CefBrowserView> browser_view, CefRefPtr<CefBrowserView> popup_browser_view, bool is_devtools)
  {
    const CefRefPtr<MyWndDelegate> popup_wnd_delegate{ new MyWndDelegate{ popup_browser_view, is_devtools } };

    const auto parent_wnd = browser_view->GetWindow();
    const auto parent_wnd_size = parent_wnd->GetSize();

    const auto popup_wnd = CefWindow::CreateTopLevelWindow(popup_wnd_delegate);
    popup_wnd->CenterWindow({ parent_wnd_size.width / 2, parent_wnd_size.height / 2 });
    return true;
  }
} // namespace CEFDemo
