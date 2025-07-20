#include "simple_app.h"

#include <string>

#include "include/cef_browser.h"
#include "include/cef_command_line.h"
#include "include/views/cef_browser_view.h"
#include "include/views/cef_window.h"
#include "include/wrapper/cef_helpers.h"
#include "simple_handler.h"

class SimpleWindowDelegate : public CefWindowDelegate
{
public:
    SimpleWindowDelegate(CefRefPtr<CefBrowserView> browser_view,
        cef_runtime_style_t runtime_style,
        cef_show_state_t initial_show_state)
        : browser_view_(browser_view),
        runtime_style_(runtime_style),
        initial_show_state_(initial_show_state)
    {
    }

    void OnWindowCreated(CefRefPtr<CefWindow> window) override
    {
        // Add the browser view and show the window.
        window->AddChildView(browser_view_);

        if (initial_show_state_ != CEF_SHOW_STATE_HIDDEN)
        {
            window->Show();
        }
    }

    void OnWindowDestroyed(CefRefPtr<CefWindow> window) override
    {
        browser_view_ = nullptr;
    }

    bool CanClose(CefRefPtr<CefWindow> window) override
    {
        // Allow the window to close if the browser says it's OK.
        CefRefPtr<CefBrowser> browser = browser_view_->GetBrowser();
        if (browser) {
            return browser->GetHost()->TryCloseBrowser();
        }
        return true;
    }

    CefSize GetPreferredSize(CefRefPtr<CefView> view) override
    {
        return CefSize(800, 600);
    }

    cef_show_state_t GetInitialShowState(CefRefPtr<CefWindow> window) override
    {
        return initial_show_state_;
    }

    cef_runtime_style_t GetWindowRuntimeStyle() override
    {
        return runtime_style_;
    }

private:
    CefRefPtr<CefBrowserView> browser_view_;
    const cef_runtime_style_t runtime_style_;
    const cef_show_state_t initial_show_state_;

    IMPLEMENT_REFCOUNTING(SimpleWindowDelegate);
    DISALLOW_COPY_AND_ASSIGN(SimpleWindowDelegate);
};

class SimpleBrowserViewDelegate : public CefBrowserViewDelegate
{
public:
    explicit SimpleBrowserViewDelegate(cef_runtime_style_t runtime_style)
        : runtime_style_(runtime_style)
    {
    }

    bool OnPopupBrowserViewCreated(CefRefPtr<CefBrowserView> browser_view, CefRefPtr<CefBrowserView> popup_browser_view, bool is_devtools) override
    {
        // Create a new top-level Window for the popup. It will show itself after
        // creation.
        CefWindow::CreateTopLevelWindow(new SimpleWindowDelegate(popup_browser_view, runtime_style_, CEF_SHOW_STATE_NORMAL));

        // We created the Window.
        return true;
    }

    cef_runtime_style_t GetBrowserRuntimeStyle() override
    {
        return runtime_style_;
    }

private:
    const cef_runtime_style_t runtime_style_;

    IMPLEMENT_REFCOUNTING(SimpleBrowserViewDelegate);
    DISALLOW_COPY_AND_ASSIGN(SimpleBrowserViewDelegate);
};


CefRefPtr<CefBrowserProcessHandler> SimpleApp::GetBrowserProcessHandler()
{
    return this;
}

void SimpleApp::OnContextInitialized()
{
    CEF_REQUIRE_UI_THREAD();

    // Check if Alloy style will be used.
    cef_runtime_style_t runtime_style = CEF_RUNTIME_STYLE_DEFAULT;

    // SimpleHandler implements browser-level callbacks.
    CefRefPtr<SimpleHandler> handler(new SimpleHandler(false));

    // Specify CEF browser settings here.
    CefBrowserSettings browser_settings;

    std::string url{ "https://www.baidu.com" };

    // Create the BrowserView.
    CefRefPtr<CefBrowserView> browser_view = CefBrowserView::CreateBrowserView
    (
        handler, url, browser_settings, nullptr, nullptr,
        new SimpleBrowserViewDelegate(runtime_style)
    );

    // Create the Window. It will show itself after creation.
    CefWindow::CreateTopLevelWindow
    (
        new SimpleWindowDelegate(browser_view, runtime_style, CEF_SHOW_STATE_NORMAL)
    );
}

CefRefPtr<CefClient> SimpleApp::GetDefaultClient()
{
    // Called when a new browser window is created via Chrome style UI.
    return SimpleHandler::GetInstance();
}
