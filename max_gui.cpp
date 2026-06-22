#include <windows.h>
#include <string>
#include <wrl.h>
#include <shlwapi.h>
#include "WebView2.h"

#pragma comment(lib, "shlwapi.lib")
using namespace Microsoft::WRL;

// Глобальные переменные
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
    wc.lpszClassName = L"MAX_ANTIVIRUS_UI";
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    RegisterClassW(&wc);

    hWnd = CreateWindowExW(0, L"MAX_ANTIVIRUS_UI", L"MAX Antivirus", 
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1200, 800, 0, 0, hInst, 0);
    
    if (!hWnd) return 0;
    ShowWindow(hWnd, nShow);

    // Инициализация WebView2
    CreateCoreWebView2EnvironmentWithOptions(nullptr, nullptr, nullptr,
        Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>([](HRESULT res, ICoreWebView2Environment* env) -> HRESULT {
            env->CreateCoreWebView2Controller(hWnd, Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>([](HRESULT res, ICoreWebView2Controller* ctrl) -> HRESULT {
                if (ctrl != nullptr) {
                    webviewController = ctrl;
                    ComPtr<ICoreWebView2> webview;
                    ctrl->get_CoreWebView2(&webview);

                    // Устанавливаем размер содержимого на все окно
                    RECT bounds; GetClientRect(hWnd, &bounds);
                    webviewController->put_Bounds(bounds);

                    // Получаем путь к HTML файлу
                    WCHAR szPath[MAX_PATH];
                    GetModuleFileNameW(NULL, szPath, MAX_PATH);
                    PathRemoveFileSpecW(szPath);
                    
                    std::wstring htmlPath = L"file:///";
                    htmlPath += szPath;
                    htmlPath += L"\\ui_mockup.html";
                    
                    // Обязательная замена слешей для Windows путей
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
