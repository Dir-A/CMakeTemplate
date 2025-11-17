#pragma once
#include "include/views/cef_window.h"
#include "include/views/cef_browser_view.h"

namespace CEFDemo
{
  class MyWndDelegate : public CefWindowDelegate
  {
  private:
    IMPLEMENT_REFCOUNTING(MyWndDelegate);

  private:
    [[maybe_unused]] bool m_isDevtools;
    CefRefPtr<CefBrowserView> m_BrowserView;

  public:
    MyWndDelegate() = delete;
    MyWndDelegate(const CefRefPtr<CefBrowserView>& browser_view, const bool isDevtools = false);
    MyWndDelegate(const MyWndDelegate&) = delete;
    MyWndDelegate(MyWndDelegate&&) noexcept = delete;
    MyWndDelegate& operator=(const MyWndDelegate&) = delete;
    MyWndDelegate& operator=(MyWndDelegate&&) noexcept = delete;
    virtual ~MyWndDelegate() = default;

    // CefWindowDelegate
  public:
    void OnWindowCreated(CefRefPtr<CefWindow> window) override;
    void OnWindowDestroyed(CefRefPtr<CefWindow> window) override;
    bool CanClose(CefRefPtr<CefWindow> window) override;
    bool CanResize(CefRefPtr<CefWindow> window) override;
  };

  class MyBrowserViewDelegate : public CefBrowserViewDelegate
  {
  private:
    IMPLEMENT_REFCOUNTING(MyBrowserViewDelegate);

  public:
    MyBrowserViewDelegate() = default;
    MyBrowserViewDelegate(const MyBrowserViewDelegate&) = delete;
    MyBrowserViewDelegate(MyBrowserViewDelegate&&) noexcept = delete;
    MyBrowserViewDelegate& operator=(MyBrowserViewDelegate&&) = delete;
    MyBrowserViewDelegate& operator=(const MyBrowserViewDelegate&) = delete;
    virtual ~MyBrowserViewDelegate() = default;

    // CefBrowserViewDelegate
  public:
    bool OnPopupBrowserViewCreated(CefRefPtr<CefBrowserView> browser_view, CefRefPtr<CefBrowserView> popup_browser_view, bool is_devtools) override;
  };

} // namespace CEFDemo
