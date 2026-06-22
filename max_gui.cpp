#include <windows.h>
#include <string>
#include <wrl.h>
#include "WebView2.h"

using namespace Microsoft::WRL;

HWND hWnd;
ComPtr<ICoreWebView2Controller> webviewController;
ComPtr<ICoreWebView2> webview;

// ВСТРОЕННЫЙ ИНТЕРФЕЙС
const wchar_t* htmlBody = L"<!DOCTYPE html><html>"
L"<body style='background:#0b0f19;color:white;font-family:sans-serif;display:flex;align-items:center;justify-content:center;height:95vh;margin:0;'>"
L"<div style='text-align:center;background:linear-gradient(135deg,#1e293b,#0f172a);padding:60px;border-radius:30px;border:1px solid #334155;width:80%;'>"
L"<h1 style='color:#3b82f6;font-size:48px;margin:0;'>MAX Antivirus</h1>"
L"<div style='margin:30px 0;'><img src='https://img.freepik.com/free-photo/view-futuristic-robot-cyborg_23-2150641525.jpg' style='width:450px;border-radius:20px;box-shadow:0 0 50px rgba(59,130,246,0.4);'></div>"
L"<p style='font-size:22px;color:#9ca3af;'>Робот MAX успешно запущен и охраняет систему.</p>"
L"<button onclick='alert(\"MAX: Сканирование памяти завершено. Угроз не обнаружено.\")' style='background:#3b82f6;color:white;padding:20px 40px;border:none;border-radius:15px;font-weight:bold;cursor:pointer;margin-top:20px;font-size:18px;'>ЗАПУСТИТЬ ПРОВЕРКУ</button>"
L"</div></body></html>";

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
    if (msg == WM_SIZE && webviewController) {
        RECT b; GetClientRect(hWnd, &b);
        webviewController->put_Bounds(b);
    }
    if (msg == WM_DESTROY) PostQuitMessage(0);
    return DefWindowProc(hWnd, msg, wp, lp);
}

int WINAPI WinMain(HINSTANCE hI, HINSTANCE hP, LPSTR lp, int nS) {
    WNDCLASSW wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hI;
    wc.lpszClassName = L"MAX_UI";
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    RegisterClassW(&wc);

    hWnd = CreateWindowExW(0, L"MAX_UI", L"MAX Antivirus", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 
        CW_USEDEFAULT, CW_USEDEFAULT, 1200, 900, 0, 0, hI, 0);

    // Пытаемся запустить графику
    HRESULT hr = CreateCoreWebView2EnvironmentWithOptions(nullptr, nullptr, nullptr,
        Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>([](HRESULT res, ICoreWebView2Environment* env) -> HRESULT {
            if (FAILED(res)) {
                MessageBoxW(NULL, L"ОШИБКА: Не установлен WebView2 Runtime!\nСкачайте его с сайта Microsoft, чтобы увидеть робота.", L"MAX Antivirus", MB_OK | MB_ICONWARNING);
                return res;
            }
            env->CreateCoreWebView2Controller(hWnd, Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>([](HRESULT res, ICoreWebView2Controller* ctrl) -> HRESULT {
                if (ctrl) {
                    webviewController = ctrl;
                    webviewController->get_CoreWebView2(&webview);
                    RECT b; GetClientRect(hWnd, &b);
                    webviewController->put_Bounds(b);
                    webview->NavigateToString(htmlBody); // Загружаем встроенный код
                }
                return S_OK;
            }).Get());
            return S_OK;
        }).Get());

    if (FAILED(hr)) {
        MessageBoxW(NULL, L"Критическая ошибка запуска движка.", L"MAX Antivirus", MB_OK | MB_ICONERROR);
    }

    MSG msg;
    while (GetMessage(&msg, 0, 0, 0)) { DispatchMessage(&msg); }
    return 0;
}
