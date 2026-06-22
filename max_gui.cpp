#include <windows.h>
#include <string>
#include <wrl.h>
#include "WebView2.h"

using namespace Microsoft::WRL;

HWND hWnd;
ComPtr<ICoreWebView2Controller> webviewController;

// ВСТРОЕННЫЙ ИНТЕРФЕЙС (HTML)
const wchar_t* htmlContent = L"data:text/html,<!DOCTYPE html>"
L"<html lang='ru'><head><meta charset='UTF-8'>"
L"<link href='https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.0.0/css/all.min.css' rel='stylesheet'>"
L"<style>"
L"body { background: #0b0f19; color: white; font-family: 'Segoe UI', sans-serif; margin: 0; overflow: hidden; display: flex; height: 100vh; }"
L".sidebar { width: 240px; background: #111827; border-right: 1px solid #374151; padding: 30px 20px; }"
L".logo { font-size: 28px; font-weight: 900; color: #3b82f6; margin-bottom: 40px; }"
L".main { flex: 1; padding: 60px; position: relative; }"
L".hero { background: linear-gradient(135deg, #1e293b 0%, #0f172a 100%); padding: 50px; border-radius: 30px; border: 1px solid #334155; position: relative; min-height: 300px; }"
L".mascot { position: absolute; right: 0; top: 0; width: 450px; height: 100%; background: url('https://img.freepik.com/free-photo/view-futuristic-robot-cyborg_23-2150641525.jpg') no-repeat center right; background-size: cover; opacity: 0.7; mask-image: linear-gradient(to left, black, transparent); -webkit-mask-image: linear-gradient(to left, black, transparent); }"
L"h1 { font-size: 42px; margin: 0 0 10px 0; } p { color: #9ca3af; font-size: 18px; }"
L".btn { background: #3b82f6; color: white; border: none; padding: 18px 40px; border-radius: 15px; font-weight: bold; cursor: pointer; margin-top: 30px; box-shadow: 0 10px 20px rgba(59,130,246,0.3); }"
L"</style></head><body>"
L"<div class='sidebar'><div class='logo'>MAX.</div><div style='color:#9ca3af'>Защита активна</div></div>"
L"<div class='main'><div class='hero'><h1>MAX Antivirus</h1><p>Система под защитой робота MAX.</p>"
L"<button class='btn' onclick='alert(\"Сканирование запущено!\")'>БЫСТРАЯ ПРОВЕРКА</button>"
L"<div class='mascot'></div></div></div>"
L"</body></html>";

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
    wc.lpszClassName = L"MAX_ANTIVIRUS";
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    RegisterClassW(&wc);

    hWnd = CreateWindowExW(0, L"MAX_ANTIVIRUS", L"MAX Antivirus", WS_OVERLAPPEDWINDOW, 
        CW_USEDEFAULT, CW_USEDEFAULT, 1200, 800, 0, 0, hI, 0);
    ShowWindow(hWnd, nS);

    CreateCoreWebView2EnvironmentWithOptions(nullptr, nullptr, nullptr,
        Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>([](HRESULT res, ICoreWebView2Environment* env) -> HRESULT {
            env->CreateCoreWebView2Controller(hWnd, Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>([](HRESULT res, ICoreWebView2Controller* ctrl) -> HRESULT {
                if (ctrl) {
                    webviewController = ctrl;
                    ComPtr<ICoreWebView2> webview;
                    ctrl->get_CoreWebView2(&webview);
                    
                    // Устанавливаем размер сразу
                    RECT bounds; GetClientRect(hWnd, &bounds);
                    webviewController->put_Bounds(bounds);

                    // ЗАГРУЖАЕМ ВСТРОЕННЫЙ КОД
                    webview->Navigate(htmlContent);
                }
                return S_OK;
            }).Get());
            return S_OK;
        }).Get());

    MSG msg;
    while (GetMessage(&msg, 0, 0, 0)) { DispatchMessage(&msg); }
    return 0;
}
