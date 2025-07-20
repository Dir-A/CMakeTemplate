#include "simple_handler.h"

#include <windows.h>

#include <string>

#include "include/cef_browser.h"

void SimpleHandler::PlatformTitleChange(CefRefPtr<CefBrowser> browser, const CefString& title)
{
    if (CefWindowHandle hwnd = browser->GetHost()->GetWindowHandle();hwnd)
    {
        ::SetWindowTextW(hwnd, std::wstring(title).c_str());
    }
}
