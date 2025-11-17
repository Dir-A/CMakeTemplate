#include <print>
#include "MyApp.hpp"
#include "MyPreset.hpp"

auto main(const int argc, char** argv) -> int
{
  try
  {
    CEFDemo::MyPreset preset{ argc, argv };
    CefMainArgs& args = preset.args;
    CefSettings& settings = preset.settings;

    const auto status = CefInitialize(args, settings, CefRefPtr{ new CEFDemo::MyApp{} }, nullptr);
    if (!status)
    {
      return -1;
    }

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
