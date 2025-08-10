#include <windows.h>
#include <filesystem>

#include "MyApp.hpp"

INT APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPWSTR, _In_ INT)
{
  ::AllocConsole();
  ::freopen("CONOUT$", "w", stdout);
  ::freopen("CONOUT$", "w", stderr);

  ::SetDllDirectoryW((std::filesystem::current_path() / "CEFRuntime").wstring().data());

  CefSettings settings;
  settings.no_sandbox = 1;
  CefString(&settings.cache_path) = (std::filesystem::current_path() / ".cache").native();
  CefString(&settings.browser_subprocess_path) = (std::filesystem::current_path() / "CEFRuntime/CEFRuntime.exe").native();

  CefInitialize(CefMainArgs{ hInstance }, settings, CefRefPtr{ new MyApp{} }, nullptr);
  CefRunMessageLoop();
  CefShutdown();

  return 0;
}
