#pragma once

#include "include/cef_render_handler.h" // IWYU pragma: keep

class IRenderer
{
public:
  virtual ~IRenderer() {};

public:
  virtual int GetWidth() = 0;
  virtual int GetHeigh() = 0;
  virtual void UpdateTexture(const std::vector<CefRect>& dirtyRects, const void* buffer, int width, int height) = 0;
  // On Windows accelerated paint provides a shared handle. The renderer should
  // open the shared resource and present it. Use void* to avoid pulling in
  // Windows headers here.
  virtual void UpdateSharedTextureFromHandle(const void* shared_handle, int width, int height) = 0;
};
