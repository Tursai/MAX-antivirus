#include <windows.h>
#include <string>
#include <wrl.h>
#include "WebView2.h"

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
    wc.lpszClassName = L"MAX_WINDOW";
    wc.hIcon = LoadIcon(hInst, IDI_APPLICATION);
    RegisterClassW(&wc);

    hWnd = CreateWindowExW(0, L"MAX_WINDOW", L"MAX Antivirus", WS_OVERLAPPEDWINDOW, 
        CW_USEDEFAULT, CW_USEDEFAULT, 1100, 750, 0, 0, hInst, 0);
    
    ShowWindow(hWnd, nShow);

    CreateCoreWebView2EnvironmentWithOptions(nullptr, nullptr, nullptr,
        Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>([](HRESULT res, ICoreWebView2Environment* env) -> HRESULT {
            env->CreateCoreWebView2Controller(hWnd, Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>([](HRESULT res, ICoreWebView2Controller* ctrl) -> HRESULT {
                webviewController = ctrl;
                ComPtr<ICoreWebView2> webview;
                ctrl->get_CoreWebView2(&webview);
                
                WCHAR szPath[MAX_PATH]; GetModuleFileNameW(NULL, szPath, MAX_PATH);
                std::wstring path(szPath);
                std::wstring htmlPath = L"file:///" + path.substr(0, path.find_last_of(L"\\/")) + L"\\ui_mockup.html";
                webview->Navigate(htmlPath.c_str());
                return S_OK;
            }).Get());
            return S_OK;
        }).Get());

    MSG msg;
    while (GetMessage(&msg, 0, 0, 0)) { DispatchMessage(&msg); }
    return 0;
}
