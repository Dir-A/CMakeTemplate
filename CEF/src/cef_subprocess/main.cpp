#include "include/cef_app.h"

#include <windows.h> 

INT APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE /* hPrevInstance */, _In_ LPWSTR /* lpCmdLine */, _In_ INT /* nShowCmd */)
{
    CefMainArgs main_args{ hInstance };
    return CefExecuteProcess(main_args, nullptr, nullptr);
}
