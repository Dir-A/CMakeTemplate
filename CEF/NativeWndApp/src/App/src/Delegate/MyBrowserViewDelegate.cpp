#include "MyBrowserViewDelegate.hpp"
#include "MyWndDelegate.hpp"

bool MyBrowserViewDelegate::OnPopupBrowserViewCreated(CefRefPtr<CefBrowserView> browser_view, CefRefPtr<CefBrowserView> popup_browser_view, bool is_devtools)
{
  CefWindow::CreateTopLevelWindow({ new MyWndDelegate{ popup_browser_view, is_devtools } });
  return true;
}