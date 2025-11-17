#include "Preboot.hpp"
#include <stdexcept>

#ifdef _WIN32
#include <windows.h>
#include <filesystem>
#elif __linux__
#include <filesystem>
#elif __APPLE__
#include <filesystem>
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
#elif __linux__
  this->args = CefMainArgs{ argc, argv };
  CefString(&this->settings.browser_subprocess_path) = (std::filesystem::current_path() / "CEFRuntime/CEFRuntime").c_str();
#elif __APPLE__
  const auto current_path = std::filesystem::path{ argv[0] }.parent_path();
  const auto framework_dir = current_path / "CEFRuntime/Chromium Embedded Framework.framework/";
  const auto subprocess_path = current_path / "CEFRuntime/CEFRuntime Helper.app/Contents/MacOS/CEFRuntime Helper";
  if (cef_load_library((framework_dir / "Chromium Embedded Framework").c_str()) != 1)
  {
    throw std::runtime_error("error");
  }
  this->args = CefMainArgs{ argc, argv };
  CefString(&this->settings.framework_dir_path) = framework_dir;
  CefString(&this->settings.browser_subprocess_path) = subprocess_path;
  ::CEFMacOSEntryInit();
#endif
}

Preboot::~Preboot()
{
#ifdef __APPLE__
  ::CEFMacOSEntryClean();
#endif
}
