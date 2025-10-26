#include <list>
#include <print>
#include "Preboot.hpp"
#include "include/cef_app.h"
#include "include/cef_client.h"
#include "include/cef_browser.h"
#include "include/internal/cef_ptr.h"

class MyApp : public CefApp, CefBrowserProcessHandler
{
private:
  IMPLEMENT_REFCOUNTING(MyApp);

private:
  class MyClient : public CefClient, CefLifeSpanHandler
  {
  private:
    IMPLEMENT_REFCOUNTING(MyClient);

  private:
    std::list<CefRefPtr<CefBrowser>> m_BrowserList;

  public:
    MyClient() = default;
    MyClient(const MyClient&) = delete;
    MyClient(MyClient&&) noexcept = delete;
    MyClient& operator=(const MyClient&) = delete;
    MyClient& operator=(MyClient&&) noexcept = delete;
    virtual ~MyClient() = default;

    // CefLifeSpanHandler
  public:
    CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override { return this; };
    void OnAfterCreated(CefRefPtr<CefBrowser> browser) override { m_BrowserList.push_back(browser); }
    void OnBeforeClose(CefRefPtr<CefBrowser> browser) override
    {
      std::erase_if(m_BrowserList, [&browser](const CefRefPtr<CefBrowser>& brow) { return brow->IsSame(browser); });
      if (m_BrowserList.empty())
      {
        CefQuitMessageLoop();
      }
    }
  };

public:
  MyApp() = default;
  MyApp(const MyApp&) = delete;
  MyApp(MyApp&&) noexcept = delete;
  MyApp& operator=(const MyApp&) = delete;
  MyApp& operator=(MyApp&&) noexcept = delete;
  virtual ~MyApp() = default;

  // CefBrowserProcessHandler
public:
  CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() override { return this; }
  void OnContextInitialized() override { CefBrowserHost::CreateBrowser({}, { new MyClient{} }, "https://vuejs.org/", {}, nullptr, nullptr); }
};

auto main(const int argc, char** argv) -> int
{

  try
  {
    Preboot preboot{ argc, argv };

    CefInitialize(preboot.args, preboot.settings, CefRefPtr{ new MyApp{} }, nullptr);
    CefRunMessageLoop();
    CefShutdown();

    return 0;
  }
  catch (const std::exception& err)
  {
    std::println("std::exception:{}", err.what());
  }
  catch (...)
  {
    std::println("unknown exception");
  }

  return -1;
}
