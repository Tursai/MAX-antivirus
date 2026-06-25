#include <windows.h>
#include <string>
#include <wrl.h>
#include "WebView2.h"

using namespace Microsoft::WRL;

HWND hWnd;
ComPtr<ICoreWebView2Controller> webviewController;
ComPtr<ICoreWebView2> webview;

// ФУНКЦИЯ ДЛЯ СВЯЗИ: когда жмешь кнопку в HTML, срабатывает этот код C++
void HandleWebMessage(const wchar_t* message) {
    if (std::wstring(message) == L"start_scan") {
        // Здесь мы имитируем работу движка
        MessageBoxW(hWnd, L"Движок MAX Antivirus запущен!\nНачинаю поиск вирусов...", L"Ядро системы", MB_OK | MB_ICONINFORMATION);
        
        // Посылаем сигнал обратно роботу, чтобы он сказал "Готово"
        webview->PostWebMessageAsString(L"scan_completed");
    }
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
    if (msg == WM_SIZE && webviewController) { RECT b; GetClientRect(hWnd, &b); webviewController->put_Bounds(b); }
    if (msg == WM_DESTROY) PostQuitMessage(0);
    return DefWindowProc(hWnd, msg, wp, lp);
}

int WINAPI WinMain(HINSTANCE hI, HINSTANCE hP, LPSTR lpCmd, int nS) {
    WNDCLASSW wc = {0}; wc.lpfnWndProc = WndProc; wc.hInstance = hI; wc.lpszClassName = L"MAX_PRO_V3";
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH); RegisterClassW(&wc);

    hWnd = CreateWindowExW(0, L"MAX_UI_V2", L"MAX Antivirus Professional", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 
        CW_USEDEFAULT, CW_USEDEFAULT, 1150, 800, 0, 0, hI, 0);

    CreateCoreWebView2EnvironmentWithOptions(nullptr, nullptr, nullptr,
        Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>([](HRESULT res, ICoreWebView2Environment* env) -> HRESULT {
            env->CreateCoreWebView2Controller(hWnd, Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>([](HRESULT res, ICoreWebView2Controller* ctrl) -> HRESULT {
                if (ctrl) {
                    webviewController = ctrl;
                    webviewController->get_CoreWebView2(&webview);
                    
                    ComPtr<ICoreWebView2Settings> settings;
                    webview->get_Settings(&settings);
                    settings->put_IsWebMessageEnabled(TRUE); // ВКЛЮЧАЕМ СВЯЗЬ

                    // Слушаем нажатия кнопок из интерфейса
                    webview->add_WebMessageReceived(Callback<ICoreWebView2WebMessageReceivedEventHandler>([](ICoreWebView2* sender, ICoreWebView2WebMessageReceivedEventArgs* args) -> HRESULT {
                        LPWSTR msg;
                        args->TryGetWebMessageAsString(&msg);
                        HandleWebMessage(msg);
                        CoTaskMemFree(msg);
                        return S_OK;
                    }).Get(), nullptr);

                    RECT b; GetClientRect(hWnd, &b);
                    webviewController->put_Bounds(b);
                    
                    // Загружаем интерфейс
                    WCHAR szPath[MAX_PATH]; GetModuleFileNameW(NULL, szPath, MAX_PATH);
                    std::wstring path(szPath);
                    std::wstring htmlPath = path.substr(0, path.find_last_of(L"\\/")) + L"\\ui_mockup.html";
                    webview->Navigate((L"file:///" + htmlPath).c_str());
                }
                return S_OK;
            }).Get());
            return S_OK;
        }).Get());

    MSG msg; while (GetMessage(&msg, 0, 0, 0)) { TranslateMessage(&msg); DispatchMessage(&msg); }
    return 0;
}
