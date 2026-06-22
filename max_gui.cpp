#include <windows.h>
#include <string>
#include <wrl.h>
#include "WebView2.h"

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
    // 1. Создаем класс окна
    WNDCLASSW wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInst;
    wc.lpszClassName = L"MAX_ANTIVIRUS_WINDOW";
    wc.hIcon = LoadIcon(hInst, IDI_APPLICATION);
    RegisterClassW(&wc);

    // 2. Создаем само окно
    hWnd = CreateWindowExW(0, L"MAX_ANTIVIRUS_WINDOW", L"MAX Antivirus", 
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1200, 800, 0, 0, hInst, 0);
    
    ShowWindow(hWnd, nShow);
    UpdateWindow(hWnd);

    // 3. Запускаем встроенный движок WebView2
    CreateCoreWebView2EnvironmentWithOptions(nullptr, nullptr, nullptr,
        Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>([](HRESULT res, ICoreWebView2Environment* env) -> HRESULT {
            env->CreateCoreWebView2Controller(hWnd, Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>([](HRESULT res, ICoreWebView2Controller* ctrl) -> HRESULT {
                if (ctrl != nullptr) {
                    webviewController = ctrl;
                    ComPtr<ICoreWebView2> webview;
                    ctrl->get_CoreWebView2(&webview);
                    
                    // Загружаем интерфейс из папки с программой
                    WCHAR szPath[MAX_PATH]; GetModuleFileNameW(NULL, szPath, MAX_PATH);
                    std::wstring path(szPath);
                    std::wstring htmlPath = L"file:///" + path.substr(0, path.find_last_of(L"\\/")) + L"\\ui_mockup.html";
                    webview->Navigate(htmlPath.c_str());
                }
                return S_OK;
            }).Get());
            return S_OK;
        }).Get());

    // 4. Цикл работы программы
    MSG msg;
    while (GetMessage(&msg, 0, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}
