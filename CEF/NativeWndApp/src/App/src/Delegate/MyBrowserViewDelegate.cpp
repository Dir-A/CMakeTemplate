#include "MyBrowserViewDelegate.hpp"
#include "MyWndDelegate.hpp"
#include "include/views/cef_window.h"
#include "include/views/cef_window_delegate.h"

bool MyBrowserViewDelegate::OnPopupBrowserViewCreated(CefRefPtr<CefBrowserView> browser_view, CefRefPtr<CefBrowserView> popup_browser_view, bool is_devtools)
{
  const CefRefPtr<MyWndDelegate> popup_wnd_delegate{ new MyWndDelegate{ popup_browser_view, is_devtools } };

  const auto parent_wnd = browser_view->GetWindow();
  const auto parent_wnd_size = parent_wnd->GetSize();

  const auto popup_wnd = CefWindow::CreateTopLevelWindow(popup_wnd_delegate);
  popup_wnd->CenterWindow({ parent_wnd_size.width / 2, parent_wnd_size.height / 2 });
  return true;
}