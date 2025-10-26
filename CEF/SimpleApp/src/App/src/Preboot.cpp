#include "Preboot.hpp"

#ifdef _WIN32
#include <windows.h>
#include <filesystem>
#elif __liunx__
#include <filesystem>
#elif __APPLE__
#include "include/cef_application_mac.h"
#include "include/wrapper/cef_helpers.h"
#include "include/wrapper/cef_library_loader.h"
auto CEFMacOSEntryInit() -> void;
auto CEFMacOSEntryClean() -> void;
#endif

Preboot::Preboot([[maybe_unused]] const int argc, [[maybe_unused]] char** argv)
{
  this->settings.no_sandbox = 1;

#ifdef _WIN32
  // for debug
  ::AllocConsole();
  ::freopen("CONOUT$", "w", stdout);
  ::freopen("CONOUT$", "w", stderr);

  const auto current_dir = std::filesystem::current_path();
  ::SetDllDirectoryW((current_dir / "CEFRuntime").wstring().data());

  this->args = CefMainArgs{ ::GetModuleHandleW(nullptr) };
  CefString(&this->settings.browser_subprocess_path) = (current_dir / L"CEFRuntime/CEFRuntime.exe").native();
#elif __liunx__
  this->args = CefMainArgs{ argc, argv };
  CefString(&this->settings.browser_subprocess_path) = (std::filesystem::current_path() / "CEFRuntime/CEFRuntime").c_str();
#elif __APPLE__
  this->args = CefMainArgs{ argc, argv };
  CefScopedLibraryLoader library_loader;
  if (!library_loader.LoadInMain())
  {
    return 1;
  }
  ::CEFMacOSEntryInit();
#endif
}

Preboot::~Preboot()
{
#ifdef __APPLE__
  ::CEFMacOSEntryClean();
#endif
}
