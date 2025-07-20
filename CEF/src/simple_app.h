#pragma once
#include "include/cef_app.h"


class SimpleApp : public CefApp, CefBrowserProcessHandler
{
public:
    SimpleApp() = default;

public:
    CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() override;

public:
    void OnContextInitialized() override;
    CefRefPtr<CefClient> GetDefaultClient() override;

private:
    IMPLEMENT_REFCOUNTING(SimpleApp);
};
