#include <windows.h>
#include <wrl.h>
#include <wil/com.h>
#include <WebView2.h>
#include <dwmapi.h>
#pragma comment(lib, "dwmapi")

static wil::com_ptr<ICoreWebView2> sg_WebView;
static wil::com_ptr<ICoreWebView2Controller> sg_WebViewController;

auto main() -> int
{
  ::SetProcessDPIAware(); // for hidpi

  const WNDCLASSEXW wnd_class_ex
  {
    .cbSize = sizeof(WNDCLASSEX),
    .style = CS_HREDRAW | CS_VREDRAW,
    .lpfnWndProc = [](HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) -> LRESULT
    {
      switch (message)
      {
      case WM_SIZE:
      {
        if (sg_WebViewController != nullptr)
        {
          RECT bounds;
          ::GetClientRect(hWnd, &bounds);
          sg_WebViewController->put_Bounds(bounds);
        };
      }
      break;
      case WM_DESTROY: ::PostQuitMessage(0); break;
      }

      return ::DefWindowProcW(hWnd, message, wParam, lParam);
    },
    .hInstance = ::GetModuleHandleW(nullptr),
    .lpszClassName = L"DesktopApp"
  };

  ::RegisterClassExW(&wnd_class_ex);

  const auto hwnd = ::CreateWindowExW(WS_EX_NOREDIRECTIONBITMAP, wnd_class_ex.lpszClassName, L"WebView sample", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, nullptr, nullptr, wnd_class_ex.hInstance, nullptr);

  // for Windows 11 Build 22621
  // DWMSBT_MAINWINDOW      Mica
  // DWMSBT_TRANSIENTWINDOW Acrylic
  // DWMSBT_TABBEDWINDOW    Mica Alt
  const DWORD attribute = DWMSBT_TABBEDWINDOW;
  ::DwmSetWindowAttribute(hwnd, DWMWA_SYSTEMBACKDROP_TYPE, &attribute, sizeof(attribute));

  ::ShowWindow(hwnd, SW_NORMAL);
  ::UpdateWindow(hwnd);

  // WebView2 Create
  ::CreateCoreWebView2EnvironmentWithOptions(nullptr, nullptr, nullptr, Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
    [hwnd](HRESULT /*result*/, ICoreWebView2Environment* env) -> HRESULT
    {
      env->CreateCoreWebView2Controller(hwnd, Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
        [hwnd](HRESULT /*result*/, ICoreWebView2Controller* controller) -> HRESULT
        {
          sg_WebViewController = controller;
          sg_WebViewController->get_CoreWebView2(&sg_WebView);

          // transparent background
          Microsoft::WRL::ComPtr<ICoreWebView2Controller2> controller2;
          controller->QueryInterface(IID_PPV_ARGS(&controller2));
          controller2->put_DefaultBackgroundColor(COREWEBVIEW2_COLOR{});

          RECT bounds;
          ::GetClientRect(hwnd, &bounds);
          sg_WebViewController->put_Bounds(bounds);
          sg_WebView->Navigate(L"https://www.linkedin.com/");
          return S_OK;
        })
        .Get());
      return S_OK;
    })
    .Get()
  );

  MSG msg;
  while (::GetMessageW(&msg, nullptr, 0, 0))
  {
    ::TranslateMessage(&msg);
    ::DispatchMessageW(&msg);
  }

  return (int)msg.wParam;
}
