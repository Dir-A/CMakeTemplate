#pragma once
#include "include/cef_app.h" // IWYU pragma: keep

class Preboot
{
public:
  CefMainArgs args;
  CefSettings settings;

public:
  Preboot(const int argc, char** argv);
  Preboot(const Preboot&) = delete;
  Preboot(Preboot&&) noexcept = delete;
  Preboot& operator=(const Preboot&) = delete;
  Preboot& operator=(Preboot&&) noexcept = delete;
  ~Preboot();
};
