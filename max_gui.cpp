#include <windows.h>
#include <stdio.h>
#include <string>
#include <fstream>
#include "WebView2.h"
#include <wil/com.h>
#include <wrl.h>

using namespace Microsoft::WRL;

// Функция для записи лога
void write_log(std::string text) {
    std::ofstream log_file("max_debug_log.txt", std::ios::app);
    log_file << text << std::endl;
    log_file.close();
    printf("%s\n", text.c_str());
}

static wil::com_ptr<ICoreWebView2Controller> webviewController;
static wil::com_ptr<ICoreWebView2> webview;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int main() {
    // Получаем Handle текущего экземпляра
    HINSTANCE hInstance = GetModuleHandle(NULL);

    write_log("--- ЗАПУСК MAX ANTIVIRUS DEBUG ---");

    // Проверка наличия HTML интерфейса
    if (GetFileAttributes(L"ui_mockup.html") == INVALID_FILE_ATTRIBUTES) {
        MessageBox(NULL, L"Критическая ошибка: Файл ui_mockup.html не найден в папке с программой!", L"MAX Error", MB_OK | MB_ICONERROR);
        write_log("Ошибка: ui_mockup.html не найден.");
        return 1;
    }

    WNDCLASSEX wcex = {sizeof(WNDCLASSEX)};
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.hInstance = hInstance;
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszClassName = L"MAXAntivirusGUI";

    if (!RegisterClassEx(&wcex)) {
        write_log("Ошибка регистрации класса окна.");
        return 1;
    }

    HWND hWnd = CreateWindow(L"MAXAntivirusGUI", L"MAX Antivirus - DEBUG MODE", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 1100, 750, NULL, NULL, hInstance, NULL);

    if (!hWnd) {
        write_log("Ошибка создания окна.");
        return 1;
    }

    write_log("Окно создано. Инициализация WebView2...");
    ShowWindow(hWnd, SW_SHOW);
    UpdateWindow(hWnd);

    HRESULT hr = CreateCoreWebView2EnvironmentWithOptions(nullptr, nullptr, nullptr,
        Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
            [hWnd](HRESULT result, ICoreWebView2Environment* env) -> HRESULT {
                if (FAILED(result)) {
                    write_log("ОШИБКА: WebView2 Environment не создан. Код: " + std::to_string(result));
                    MessageBox(hWnd, L"Ошибка: WebView2 Runtime не установлен. Пожалуйста, скачайте его с сайта Microsoft.", L"MAX Error", MB_OK);
                    return result;
                }
                env->CreateCoreWebView2Controller(hWnd, Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                    [hWnd](HRESULT result, ICoreWebView2Controller* controller) -> HRESULT {
                        if (FAILED(result)) {
                            write_log("ОШИБКА: Контроллер WebView2 не создан.");
                            return result;
                        }
                        webviewController = controller;
                        webviewController->get_CoreWebView2(&webview);

                        RECT bounds;
                        GetClientRect(hWnd, &bounds);
                        webviewController->put_Bounds(bounds);

                        write_log("WebView2 готов. Загрузка интерфейса...");
                        webview->Navigate(L"https://www.google.com"); // Сначала проверим на внешнем сайте
                        return S_OK;
                    }).Get());
                return S_OK;
            }).Get());

    if (FAILED(hr)) {
        write_log("CreateCoreWebView2EnvironmentWithOptions вернул ошибку.");
    }

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    if (message == WM_DESTROY) PostQuitMessage(0);
    return DefWindowProc(hWnd, message, wParam, lParam);
}
