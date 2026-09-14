#include "CEFPreset.hpp"

#ifdef _WIN32
#  include <windows.h>
#  include <filesystem>
#elif __linux__
#  include <stdexcept>
#  include <filesystem>
#elif __APPLE__
#  include <filesystem>
#  include <stdexcept>
#  include "include/wrapper/cef_library_loader.h"
auto CEFMacOSEntryInit() -> void;
auto CEFMacOSEntryClean() -> void;
#endif

CEFPreset::CEFPreset([[maybe_unused]] const int argc, [[maybe_unused]] char** argv)
{
  this->settings.no_sandbox = 1;

#ifdef _WIN32
  this->args = CefMainArgs{ ::GetModuleHandleW(nullptr) };
  const auto runtime_dir = std::filesystem::path{ argv[0] }.parent_path() / L"ZQFCEFRuntime\\";
  cef_version_info_t version_info{};
  CEF_POPULATE_VERSION_INFO(&version_info);
  if (!this->loader.LoadInMainAssert((runtime_dir / L"libcef.dll").wstring().c_str(), nullptr, true, &version_info))
  {
    throw std::runtime_error("failed to load libcef.dll");
  }
  CefString(&this->settings.browser_subprocess_path) = (runtime_dir / L"ZQFCEFRuntime.exe").native();
#elif __linux__
  this->args = CefMainArgs{ argc, argv };
  CefString(&this->settings.browser_subprocess_path) = (std::filesystem::path{ argv[0] }.parent_path() / "ZQFCEFRuntime/ZQFCEFRuntime").c_str();
#elif __APPLE__
  this->args = CefMainArgs{ argc, argv };
  if (!loader.LoadInMain())
  {
    throw std::runtime_error("failed to load the CEF framework");
  }
  CefString(&this->settings.browser_subprocess_path) = std::filesystem::path{ argv[0] }.parent_path().parent_path() / "Frameworks/ZQFCEFRuntime Helper.app/Contents/MacOS/ZQFCEFRuntime Helper";
  ::CEFMacOSEntryInit();
#endif
}

CEFPreset::~CEFPreset()
{
#ifdef __APPLE__
  ::CEFMacOSEntryClean();
#endif
}
