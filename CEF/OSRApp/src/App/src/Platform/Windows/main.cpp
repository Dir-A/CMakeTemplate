#include <windows.h>
#include <filesystem>
#include "AppMain.hpp"

INT APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPWSTR, _In_ INT)
{
  // for debug
  ::AllocConsole();
  ::freopen("CONOUT$", "w", stdout);
  ::freopen("CONOUT$", "w", stderr);

  const auto current_dir = std::filesystem::current_path();

  ::SetDllDirectoryW((current_dir / CEF_RUNTIME_DIR).wstring().data());

  CefSettings settings;
  CefString(&settings.browser_subprocess_path) = (current_dir / CEF_RUNTIME_DIR / CEF_RUNTIME_NAME).native();
  return AppMain(CefMainArgs{ hInstance }, settings);
}
