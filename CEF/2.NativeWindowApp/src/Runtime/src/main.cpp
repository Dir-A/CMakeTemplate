#if defined(_WIN32)
#include <Windows.h>
#include "include/capi/cef_app_capi.h"

INT APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPWSTR, _In_ INT)
{
  cef_main_args_t cef_arg{ hInstance };
  return cef_execute_process(&cef_arg, nullptr, nullptr);
}

#elif __linux__
#include "include/base/cef_compiler_specific.h"
#include "include/capi/cef_app_capi.h"

NO_STACK_PROTECTOR
int main(int argc, char* argv[])
{
  cef_main_args_t cef_arg{ argc, argv };
  return cef_execute_process(&cef_arg, nullptr, nullptr);
}

#elif __APPLE__
#include "include/cef_app.h"
#include "include/wrapper/cef_library_loader.h"

int main(int argc, char* argv[])
{
  CefScopedLibraryLoader library_loader;
  if (!library_loader.LoadInHelper())
  {
    return 1;
  }

  return CefExecuteProcess(CefMainArgs{ argc, argv }, nullptr, nullptr);
}

#endif
