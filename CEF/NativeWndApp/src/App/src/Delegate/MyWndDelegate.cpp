#include "MyWndDelegate.hpp"

MyWndDelegate::MyWndDelegate(const CefRefPtr<CefBrowserView>& browser_view, const bool isDevtools)
    : m_isDevtools{ isDevtools }, m_BrowserView{ browser_view }
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
  CefRefPtr<CefBrowser> browser = m_BrowserView->GetBrowser();
  return browser ? browser->GetHost()->TryCloseBrowser() : true;
}

bool MyWndDelegate::CanResize(CefRefPtr<CefWindow> window)
{
  return true;
}
