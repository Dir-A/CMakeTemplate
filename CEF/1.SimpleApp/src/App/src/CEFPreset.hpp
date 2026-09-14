#pragma once
#include "include/cef_app.h" // IWYU pragma: keep
#ifndef __linux__
#  include "include/wrapper/cef_library_loader.h"
#endif

class CEFPreset
{
public:
  CefMainArgs args;
  CefSettings settings;
#ifndef __linux__
  CefScopedLibraryLoader loader;
#endif

public:
  CEFPreset(const int argc, char** argv);
  CEFPreset(const CEFPreset&) = delete;
  CEFPreset(CEFPreset&&) noexcept = delete;
  CEFPreset& operator=(const CEFPreset&) = delete;
  CEFPreset& operator=(CEFPreset&&) noexcept = delete;
  ~CEFPreset();
};
