#pragma once
#include "include/cef_app.h" // IWYU pragma: keep

class CEFPreset
{
public:
  CefMainArgs args;
  CefSettings settings;

public:
  CEFPreset(const int argc, char** argv);
  CEFPreset(const CEFPreset&) = delete;
  CEFPreset(CEFPreset&&) noexcept = delete;
  CEFPreset& operator=(const CEFPreset&) = delete;
  CEFPreset& operator=(CEFPreset&&) noexcept = delete;
  ~CEFPreset();
};
