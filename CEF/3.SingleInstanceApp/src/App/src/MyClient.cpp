#include "MyClient.hpp"
#include "include/cef_app.h"
#include "include/cef_browser.h"
#include "include/base/cef_bind.h"
#include "include/base/cef_callback.h" // IWYU pragma: keep
#include "include/internal/cef_ptr.h"
#include "include/views/cef_browser_view.h"
#include "include/views/cef_window.h"
#include "include/wrapper/cef_closure_task.h"
#include "include/wrapper/cef_helpers.h"

namespace CEFDemo
{
  auto MyClient::GetInstance() -> CefRefPtr<MyClient>
  {
    static CefRefPtr<MyClient> instance = nullptr;

    if (instance == nullptr)
    {
      instance = CefRefPtr(new MyClient{});
    }

    return instance.get();
  }

  auto MyClient::ShowMainWindow() -> void
  {
    if (!CefCurrentlyOn(TID_UI))
    {
      CefPostTask(TID_UI, base::BindOnce(&MyClient::ShowMainWindow, this));
      return;
    }

    if (m_BrowserList.empty())
    {
      return;
    }

    if (auto browser_view = CefBrowserView::GetForBrowser(m_BrowserList.front()))
    {
      if (auto window = browser_view->GetWindow())
      {
        window->Show();
      }
    }
  }

  auto MyClient::CloseAllBrowsers(const bool isForceClose) -> void
  {
    if (!CefCurrentlyOn(TID_UI))
    {
      CefPostTask(TID_UI, base::BindOnce(&MyClient::CloseAllBrowsers, this, isForceClose));
      return;
    }

    if (m_BrowserList.empty())
    {
      return;
    }

    for (auto it = m_BrowserList.cbegin(); it != m_BrowserList.cend(); ++it) { (*it)->GetHost()->CloseBrowser(isForceClose); }
  }

  auto MyClient::IsClosing() const -> bool
  {
    return m_isClosing;
  }

  void MyClient::OnAfterCreated(CefRefPtr<CefBrowser> browser)
  {
    m_BrowserList.push_back(browser);
  }

  void MyClient::OnBeforeClose(CefRefPtr<CefBrowser> browser)
  {
    std::erase_if(m_BrowserList, [&browser](const CefRefPtr<CefBrowser>& brow) { return brow->IsSame(browser); });
    if (m_BrowserList.empty())
    {
      CefQuitMessageLoop();
    }
  }

  bool MyClient::DoClose(CefRefPtr<CefBrowser> browser)
  {
    CEF_REQUIRE_UI_THREAD();

    if (m_BrowserList.size() == 1)
    {
      m_isClosing = true;
    }

    return false;
  }
} // namespace CEFDemo
