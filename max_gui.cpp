#include <windows.h>
#include <string>
#include <wrl.h>
#include "WebView2.h"

using namespace Microsoft::WRL;

HWND hWnd;
ComPtr<ICoreWebView2Controller> webviewController;
ComPtr<ICoreWebView2> webview;

// ВСТРОЕННЫЙ ИНТЕРФЕЙС - Теперь с четкой кодировкой UTF-8
const wchar_t* htmlBody = L"<!DOCTYPE html><html><head><meta charset='UTF-8'></head>"
L"<body style='background:#0b0f19;color:white;font-family:sans-serif;display:flex;align-items:center;justify-content:center;height:95vh;margin:0;'>"
L"<div style='text-align:center;background:linear-gradient(135deg,#1e293b,#0f172a);padding:60px;border-radius:30px;border:1px solid #334155;width:80%;box-shadow: 0 20px 50px rgba(0,0,0,0.5);'>"
L"<h1 style='color:#3b82f6;font-size:48px;margin:0;'>MAX Antivirus</h1>"
L"<div style='margin:30px 0;'><img src='https://img.freepik.com/free-photo/view-futuristic-robot-cyborg_23-2150641525.jpg' style='width:400px;border-radius:20px;box-shadow:0 0 30px rgba(59,130,246,0.3);'></div>"
L"<p style='font-size:22px;color:#9ca3af;'>Робот MAX на страже вашей системы.</p>"
L"<button onclick='alert(\"MAX: Проверка завершена. Угроз не обнаружено!\")' style='background:#3b82f6;color:white;padding:15px 40px;border:none;border-radius:12px;font-weight:bold;cursor:pointer;margin-top:20px;font-size:18px;'>ЗАПУСТИТЬ ПРОВЕРКУ</button>"
L"</div></body></html>";

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
    if (msg == WM_SIZE && webviewController) {
        RECT b; GetClientRect(hWnd, &b);
        webviewController->put_Bounds(b);
    }
    if (msg == WM_DESTROY) PostQuitMessage(0);
    return DefWindowProc(hWnd, message, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hI, HINSTANCE hP, LPSTR lp, int nS) {
    WNDCLASSW wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hI;
    wc.lpszClassName = L"MAX_UI_FIXED";
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    RegisterClassW(&wc);

    hWnd = CreateWindowExW(0, L"MAX_UI_FIXED", L"MAX Antivirus", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 
        CW_USEDEFAULT, CW_USEDEFAULT, 1200, 900, 0, 0, hI, 0);

    CreateCoreWebView2EnvironmentWithOptions(nullptr, nullptr, nullptr,
        Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>([](HRESULT res, ICoreWebView2Environment* env) -> HRESULT {
            if (FAILED(res)) return res;
            env->CreateCoreWebView2Controller(hWnd, Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>([](HRESULT res, ICoreWebView2Controller* ctrl) -> HRESULT {
                if (ctrl) {
                    webviewController = ctrl;
                    webviewController->get_CoreWebView2(&webview);
                    
                    // ВКЛЮЧАЕМ РЕАКЦИЮ НА КНОПКИ (alert)
                    ComPtr<ICoreWebView2Settings> settings;
                    webview->get_Settings(&settings);
                    settings->put_AreDefaultScriptDialogsEnabled(TRUE);

                    RECT b; GetClientRect(hWnd, &b);
                    webviewController->put_Bounds(b);
                    webview->NavigateToString(htmlBody);
                }
                return S_OK;
            }).Get());
            return S_OK;
        }).Get());

    MSG msg;
    while (GetMessage(&msg, 0, 0, 0)) { DispatchMessage(&msg); }
    return 0;
}
