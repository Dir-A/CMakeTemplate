#pragma once
#include "include/views/cef_browser_view.h"
#include "include/views/cef_window.h"

class MyWndDelegate : public CefWindowDelegate
{
private:
  IMPLEMENT_REFCOUNTING(MyWndDelegate);

private:
  bool m_isDevtools;
  CefRefPtr<CefBrowserView> m_BrowserView;

public:
  MyWndDelegate() = delete;
  MyWndDelegate(CefRefPtr<CefBrowserView>& browser_view, const bool isDevtools = false);
  MyWndDelegate(const MyWndDelegate&) = delete;
  MyWndDelegate(MyWndDelegate&&) noexcept = delete;
  MyWndDelegate& operator=(const MyWndDelegate&) = delete;
  MyWndDelegate& operator=(MyWndDelegate&&) noexcept = delete;
  virtual ~MyWndDelegate() = default;

public:
  void OnWindowCreated(CefRefPtr<CefWindow> window) override;
  void OnWindowDestroyed(CefRefPtr<CefWindow> window) override;
  bool CanClose(CefRefPtr<CefWindow> window) override;
  bool CanResize(CefRefPtr<CefWindow> window) override;
};