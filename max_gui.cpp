#include <windows.h>
#include <string>
#include <wrl.h>
#include <shlwapi.h>
#include "WebView2.h"

#pragma comment(lib, "shlwapi.lib")
using namespace Microsoft::WRL;

HWND hWnd;
ComPtr<ICoreWebView2Controller> webviewController;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    if (message == WM_SIZE && webviewController != nullptr) {
        RECT bounds; GetClientRect(hWnd, &bounds);
        webviewController->put_Bounds(bounds);
    }
    if (message == WM_DESTROY) PostQuitMessage(0);
    return DefWindowProc(hWnd, message, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpCmd, int nShow) {
    WNDCLASSW wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInst;
    wc.lpszClassName = L"MAX_ANTIVIRUS_WINDOW";
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    RegisterClassW(&wc);

    hWnd = CreateWindowExW(0, L"MAX_ANTIVIRUS_WINDOW", L"MAX Antivirus", 
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1200, 800, 0, 0, hInst, 0);
    
    if (!hWnd) return 0;
    ShowWindow(hWnd, nShow);

    // Получаем путь к папке с программой
    WCHAR szPath[MAX_PATH];
    GetModuleFileNameW(NULL, szPath, MAX_PATH);
    PathRemoveFileSpecW(szPath);

    CreateCoreWebView2EnvironmentWithOptions(nullptr, nullptr, nullptr,
        Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>([szPath](HRESULT res, ICoreWebView2Environment* env) -> HRESULT {
            env->CreateCoreWebView2Controller(hWnd, Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>([env, szPath](HRESULT res, ICoreWebView2Controller* ctrl) -> HRESULT {
                if (ctrl != nullptr) {
                    webviewController = ctrl;
                    ComPtr<ICoreWebView2> webview;
                    ctrl->get_CoreWebView2(&webview);

                    // МАГИЯ: Привязываем папку к виртуальному адресу
                    webview->SetVirtualHostNameToFolderMapping(
                        L"max.app", szPath,
                        COREWEBVIEW2_HOST_RESOURCE_ACCESS_KIND_ALLOW
                    );

                    // Теперь загружаем как настоящий сайт
                    webview->Navigate(L"https://max.app/ui_mockup.html");
                }
                return S_OK;
            }).Get());
            return S_OK;
        }).Get());

    MSG msg;
    while (GetMessage(&msg, 0, 0, 0)) { DispatchMessage(&msg); }
    return 0;
}
