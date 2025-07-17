#include "include/capi/cef_app_capi.h"

#ifdef _WIN32
#include <windows.h> 

INT APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE /* hPrevInstance */, _In_ LPWSTR /* lpCmdLine */, _In_ INT /* nShowCmd */)
{
    cef_main_args_t cef_arg{ hInstance };
    // check libcef and libcef_dll_wrapper version
    // const char* api_hash = cef_api_hash(CEF_API_VERSION, 0);
    // CHECK(!strcmp(api_hash, CEF_API_HASH_PLATFORM)) << "API hashes for libcef and libcef_dll_wrapper do not match.";
    return cef_execute_process(&cef_arg, nullptr, nullptr);
}

#elif __linux__

#include "include/base/cef_compiler_specific.h"

NO_STACK_PROTECTOR
int main(int argc, char* argv[])
{
    cef_main_args_t cef_arg{ argc, argv };
    return cef_execute_process(&cef_arg, nullptr, nullptr);
}

#endif

