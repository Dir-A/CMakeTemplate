#include "AppMain.hpp"
#include "MyApp.hpp"
#include <exception>
#include <filesystem>
#include <print>

auto AppMain(const CefMainArgs& rfArgs, CefSettings& rfSettings) -> int
{
  try
  {
    rfSettings.no_sandbox = 1;
    CefString(&rfSettings.cache_path) = (std::filesystem::current_path() / ".cache").c_str();

    CefInitialize(rfArgs, rfSettings, CefRefPtr{ new MyApp{} }, nullptr);
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