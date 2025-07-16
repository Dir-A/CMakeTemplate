#include <windows.h>
#include <filesystem>
#include <span>

#include "include/cef_command_line.h"
#include "include/cef_sandbox_win.h"
#include "include/cef_version_info.h"
#include "simple_app.h"


// INT APIENTRY wWinMain(_In_ HINSTANCE /*hInstance*/, _In_opt_ HINSTANCE /* hPrevInstance */, _In_ LPWSTR /* lpCmdLine */, _In_ INT /* nShowCmd */)
auto main() -> int
{
    const auto sub_process_dir = std::filesystem::current_path() / "cef_subprocess";
    const auto sub_process_path = sub_process_dir / "cef_subprocess.exe";

    ::SetDllDirectoryW(sub_process_dir.wstring().data());

    CefMainArgs main_args{ ::GetModuleHandleW(nullptr) };

    const auto command_line = CefCommandLine::CreateCommandLine();
    command_line->InitFromString(::GetCommandLineW());

    CefSettings settings;
    settings.no_sandbox = true;
    CefString(&settings.browser_subprocess_path).FromWString(sub_process_path.wstring());

    CefRefPtr<SimpleApp> app(new SimpleApp);

    if (!CefInitialize(main_args, settings, app.get(), nullptr))
    {
        return CefGetExitCode();
    }

    CefRunMessageLoop();

    CefShutdown();
    return 0;
}
