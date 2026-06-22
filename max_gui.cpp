#include <windows.h>
#include <string>
#include <wrl.h>
#include "WebView2.h"

using namespace Microsoft::WRL;

HWND hWnd;
ComPtr<ICoreWebView2Controller> webviewController;
ComPtr<ICoreWebView2> webview;

// ВСТРОЕННЫЙ ДИЗАЙН (HTML + CSS + РОБОТ)
const wchar_t* htmlInterface = L"<!DOCTYPE html><html><head><meta charset='UTF-8'>"
L"<link href='https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.0.0/css/all.min.css' rel='stylesheet'>"
L"<style>"
L"body { background: #0b0f19; color: white; font-family: 'Segoe UI', sans-serif; margin: 0; display: flex; height: 100vh; overflow: hidden; }"
L".sidebar { width: 220px; background: #111827; border-right: 1px solid #374151; padding: 30px 20px; }"
L".logo { font-size: 26px; font-weight: 900; color: #3b82f6; margin-bottom: 40px; }"
L".main { flex: 1; padding: 40px; position: relative; }"
L".hero { background: linear-gradient(135deg, #1e293b 0%, #0f172a 100%); padding: 50px; border-radius: 25px; border: 1px solid #334155; position: relative; min-height: 280px; }"
L"h1 { font-size: 36px; margin: 0; } p { color: #9ca3af; font-size: 18px; margin: 10px 0 30px 0; }"
L".btn { background: #3b82f6; color: white; border: none; padding: 16px 32px; border-radius: 12px; font-weight: bold; cursor: pointer; transition: 0.3s; }"
L".btn:hover { background: #2563eb; transform: scale(1.05); }"
L".mascot { position: absolute; right: 0; top: 0; width: 400px; height: 100%; background: url('https://img.freepik.com/free-photo/view-futuristic-robot-cyborg_23-2150641525.jpg') no-repeat center; background-size: cover; opacity: 0.7; mask-image: linear-gradient(to left, black 50%, transparent); -webkit-mask-image: linear-gradient(to left, black 50%, transparent); }"
L"</style></head><body>"
L"<div class='sidebar'><div class='logo'>MAX.</div><div style='color:#3b82f6;font-weight:bold;'><i class='fas fa-shield-alt'></i> ЗАЩИТА АКТИВНА</div></div>"
L"<div class='main'><div class='hero'><h1>MAX Antivirus</h1><p>Ваша система под интеллектуальной защитой робота.</p>"
L"<button class='btn' onclick='alert(\"MAX: Сканирование запущено! Угроз не обнаружено.\")'>ЗАПУСТИТЬ ПРОВЕРКУ</button>"
L"<div class='mascot'></div></div></div>"
L"</body></html>";

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
    wc.lpszClassName = L"MAX_ANTIVIRUS_FINAL";
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    RegisterClassW(&wc);

    hWnd = CreateWindowExW(0, L"MAX_ANTIVIRUS_FINAL", L"MAX Antivirus Professional", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 
        CW_USEDEFAULT, CW_USEDEFAULT, 1100, 750, 0, 0, hI, 0);

    CreateCoreWebView2EnvironmentWithOptions(nullptr, nullptr, nullptr,
        Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>([](HRESULT res, ICoreWebView2Environment* env) -> HRESULT {
            env->CreateCoreWebView2Controller(hWnd, Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>([](HRESULT res, ICoreWebView2Controller* ctrl) -> HRESULT {
                if (ctrl) {
                    webviewController = ctrl;
                    webviewController->get_CoreWebView2(&webview);
                    ComPtr<ICoreWebView2Settings> s;
                    webview->get_Settings(&s);
                    s->put_AreDefaultScriptDialogsEnabled(TRUE); // Оживляем кнопки
                    RECT b; GetClientRect(hWnd, &b);
                    webviewController->put_Bounds(b);
                    webview->NavigateToString(htmlInterface);
                }
                return S_OK;
            }).Get());
            return S_OK;
        }).Get());

    MSG msg;
    while (GetMessage(&msg, 0, 0, 0)) { DispatchMessage(&msg); }
    return 0;
}
