/*
 * MAX Antivirus GUI - Windows WebView2 Host
 * Связка современного UI (HTML) и движка ClamAV
 */

#include <windows.h>
#include <stdlib.h>
#include <string>
#include <tchar.h>
#include <wrl.h>
#include <wil/com.h>
// Для компиляции требуется Microsoft.Web.WebView2 SDK
#include "WebView2.h"

using namespace Microsoft::WRL;

// Глобальные переменные для WebView
static wil::com_ptr<ICoreWebView2Controller> webviewController;
static wil::com_ptr<ICoreWebView2> webview;

// Прототип функции сканирования (из нашего движка)
extern "C" int run_max_scan(const char* path);

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Регистрация класса окна
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = _T("MAXAntivirusGUI");
    wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

    if (!RegisterClassEx(&wcex)) {
        MessageBox(NULL, _T("Ошибка регистрации окна!"), _T("MAX Antivirus"), NULL);
        return 1;
    }

    // Создание окна
    HWND hWnd = CreateWindow(_T("MAXAntivirusGUI"), _T("MAX Antivirus"), WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 1200, 800, NULL, NULL, hInstance, NULL);

    if (!hWnd) {
        MessageBox(NULL, _T("Ошибка создания окна!"), _T("MAX Antivirus"), NULL);
        return 1;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    // Инициализация WebView2
    CreateCoreWebView2EnvironmentWithOptions(nullptr, nullptr, nullptr,
        Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
            [hWnd](HRESULT result, ICoreWebView2Environment* env) -> HRESULT {
                env->CreateCoreWebView2Controller(hWnd, Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                    [hWnd](HRESULT result, ICoreWebView2Controller* controller) -> HRESULT {
                        if (controller != nullptr) {
                            webviewController = controller;
                            webviewController->get_CoreWebView2(&webview);
                        }

                        // Настройка WebView
                        ICoreWebView2Settings* Settings;
                        webview->get_Settings(&Settings);
                        Settings->put_IsScriptEnabled(TRUE);
                        Settings->put_IsWebMessageEnabled(TRUE);

                        // Получаем путь к папке с EXE
                        WCHAR szPath[MAX_PATH];
                        GetModuleFileNameW(NULL, szPath, MAX_PATH);
                        std::wstring path(szPath);
                        size_t pos = path.find_last_of(L"\\/");
                        std::wstring dir = path.substr(0, pos);
                        std::wstring htmlPath = L"file:///" + dir + L"\\ui_mockup.html";

                        // Загружаем локальный интерфейс из папки приложения
                        webview->Navigate(htmlPath.c_str());

                        // Обработка сообщений из JS (например, нажатие кнопки сканирования)
                        webview->add_WebMessageReceived(Callback<ICoreWebView2WebMessageReceivedEventHandler>(
                            [](ICoreWebView2* sender, ICoreWebView2WebMessageReceivedEventArgs* args) -> HRESULT {
                                LPWSTR message;
                                args->TryGetWebMessageAsString(&message);
                                // Если пришло сообщение "start_scan", запускаем логику
                                if (std::wstring(message) == L"start_scan") {
                                    // Здесь вызываем run_max_scan(...)
                                }
                                CoTaskMemFree(message);
                                return S_OK;
                            }).Get(), nullptr);

                        return S_OK;
                    }).Get());
                return S_OK;
            }).Get());

    // Цикл сообщений
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_SIZE:
        if (webviewController != nullptr) {
            RECT bounds;
            GetClientRect(hWnd, &bounds);
            webviewController->put_Bounds(bounds);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
