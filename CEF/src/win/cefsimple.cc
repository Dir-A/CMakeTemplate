#include <windows.h>
#include <filesystem>
#include <span>

#include "include/cef_command_line.h"
#include "include/cef_sandbox_win.h"
#include "include/cef_version_info.h"
#include "simple_app.h"

#include <print>
INT APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE /* hPrevInstance */, _In_ LPWSTR /* lpCmdLine */, _In_ INT /* nShowCmd */)
{
    ::AllocConsole();
    ::freopen("CONOUT$", "w", stdout);
    ::freopen("CONOUT$", "w", stderr);

    ::SetDllDirectoryW((std::filesystem::current_path() / CEF_RUNTIME_DIR).wstring().data());

    CefMainArgs main_args{ hInstance };

    const auto command_line = CefCommandLine::CreateCommandLine();
    command_line->InitFromString(::GetCommandLineW());

    CefSettings settings;
    settings.no_sandbox = true;
    CefString(&settings.browser_subprocess_path).FromWString(((std::filesystem::current_path() / CEF_RUNTIME_DIR / CEF_RUNTIME_NAME)).wstring());

    CefRefPtr<SimpleApp> app(new SimpleApp);

    if (!CefInitialize(main_args, settings, app.get(), nullptr))
    {
        return CefGetExitCode();
    }

    CefRunMessageLoop();

    CefShutdown();
    return 0;
}
