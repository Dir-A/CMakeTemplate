#include "AppMain.hpp"
#include "MyApp.hpp"
#include "MyClient.hpp"
#include "Renderer/SdlRenderer.hpp"
#include "include/cef_app.h"
#include <print>
#include <exception>
#include <filesystem>

auto AppMain(const CefMainArgs& rfArgs, CefSettings& rfSettings) -> int
{
  try
  {
    rfSettings.no_sandbox = 1;
    rfSettings.windowless_rendering_enabled = 1; // for osr
    CefString(&rfSettings.cache_path) = (std::filesystem::current_path() / ".cache").c_str();

    CefInitialize(rfArgs, rfSettings, new MyApp{}, nullptr);
    {
      SdlRenderer renderer;
      MyClient::GetInstance()->SetRenderer(&renderer);
      {
        auto browser = MyClient::GetInstance()->GetMainBrowser();
        browser->GetHost()->SetWindowlessFrameRate(renderer.GetFrameRate());
        renderer.Run(browser);
        browser->GetHost()->CloseBrowser(true);
        // invoke CloseBrowser to remove self from the m_BrowserList of MyClient, otherwise the iterator will become invalid during invoke CloseAllBrowsers iteration process.
      }
      MyClient::GetInstance()->SetRenderer(nullptr);

      if (MyClient::GetInstance()->GetBrowserCount()) // NOLINT
      {
        MyClient::GetInstance()->CloseAllBrowsers(true);
        CefRunMessageLoop();
      }
    }

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