#include <filesystem>
#include "MyApp.hpp"

NO_STACK_PROTECTOR
int main(int argc, char* argv[])
{
  CefSettings settings;
  settings.no_sandbox = true;
  CefString(&settings.cache_path) = ((std::filesystem::current_path() / ".cache")).native();
  CefString(&settings.browser_subprocess_path) = (std::filesystem::current_path() / "CEFRuntime/CEFRuntime").native();

  CefRefPtr<CefCommandLine> command_line = CefCommandLine::CreateCommandLine();
  command_line->InitFromArgv(argc, argv);

  CefInitialize(CefMainArgs{ argc, argv }, settings, new MyApp{}, nullptr);
  CefRunMessageLoop();
  CefShutdown();
  return 0;
}
