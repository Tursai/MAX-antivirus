#include <windows.h>
#include <string>
#include <wrl.h>
#include <shlwapi.h>
#include "WebView2.h"

#pragma comment(lib, "shlwapi.lib")
using namespace Microsoft::WRL;

HWND hWnd;
ComPtr<ICoreWebView2Controller> webviewController;

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
    if (msg == WM_SIZE && webviewController) {
        RECT bounds; GetClientRect(hWnd, &bounds);
        webviewController->put_Bounds(bounds);
    }
    if (msg == WM_DESTROY) PostQuitMessage(0);
    return DefWindowProc(hWnd, msg, wp, lp);
}

int WINAPI WinMain(HINSTANCE hI, HINSTANCE hP, LPSTR lp, int nS) {
    WNDCLASSW wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hI;
    wc.lpszClassName = L"MAX_UI";
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    RegisterClassW(&wc);

    hWnd = CreateWindowExW(0, L"MAX_UI", L"MAX Antivirus", WS_OVERLAPPEDWINDOW, 
        CW_USEDEFAULT, CW_USEDEFAULT, 1200, 800, 0, 0, hI, 0);
    ShowWindow(hWnd, nS);

    CreateCoreWebView2EnvironmentWithOptions(nullptr, nullptr, nullptr,
        Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>([](HRESULT res, ICoreWebView2Environment* env) -> HRESULT {
            env->CreateCoreWebView2Controller(hWnd, Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>([](HRESULT res, ICoreWebView2Controller* ctrl) -> HRESULT {
                if (ctrl) {
                    webviewController = ctrl;
                    ComPtr<ICoreWebView2> webview;
                    ctrl->get_CoreWebView2(&webview);

                    // Настройка путей
                    WCHAR szPath[MAX_PATH];
                    GetModuleFileNameW(NULL, szPath, MAX_PATH);
                    PathRemoveFileSpecW(szPath);
                    std::wstring htmlPath = L"file:///" + std::wstring(szPath) + L"\\ui_mockup.html";
                    for (auto& c : htmlPath) if (c == L'\\') c = L'/';

                    webview->Navigate(htmlPath.c_str());
                }
                return S_OK;
            }).Get());
            return S_OK;
        }).Get());

    MSG msg;
    while (GetMessage(&msg, 0, 0, 0)) { DispatchMessage(&msg); }
    return 0;
}
