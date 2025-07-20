#include <windows.h> 
#include "include/capi/cef_app_capi.h"


INT APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE /* hPrevInstance */, _In_ LPWSTR /* lpCmdLine */, _In_ INT /* nShowCmd */)
{
    // check libcef and libcef_dll_wrapper version
    const char* api_hash = cef_api_hash(CEF_API_VERSION, 0);
    if (std::strcmp(api_hash, CEF_API_HASH_PLATFORM) != 0)
    {
        ::OutputDebugStringW(L"CEFRuntime: API hashes for libcef and libcef_dll_wrapper do not match.");
    }

    cef_main_args_t cef_arg{ hInstance };
    return cef_execute_process(&cef_arg, nullptr, nullptr);
}

