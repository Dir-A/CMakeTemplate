#pragma once
#include "include/cef_app.h" // IWYU pragma: keep

namespace CEFDemo
{
  class MyPreset
  {
  public:
    CefMainArgs args;
    CefSettings settings;

  public:
    MyPreset(const int argc, char** argv);
    MyPreset(const MyPreset&) = delete;
    MyPreset(MyPreset&&) noexcept = delete;
    MyPreset& operator=(const MyPreset&) = delete;
    MyPreset& operator=(MyPreset&&) noexcept = delete;
    ~MyPreset();
  };
} // namespace CEFDemo
