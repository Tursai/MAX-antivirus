#include <windows.h>
#include <string>
#include <wrl.h>
#include "WebView2.h"

using namespace Microsoft::WRL;

// Глобальные переменные
ComPtr<ICoreWebView2Controller> webviewController;
ComPtr<ICoreWebView2> webview;

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

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpCmd, int nShow) {
    WNDCLASSEXW wcex = {sizeof(WNDCLASSEXW)};
    wcex.lpfnWndProc = WndProc;
    wcex.hInstance = hInst;
    wcex.hIcon = LoadIcon(hInst, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.lpszClassName = L"MAXAntivirus";
    RegisterClassExW(&wcex);

    HWND hWnd = CreateWindowW(L"MAXAntivirus", L"MAX Antivirus", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 1100, 750, NULL, NULL, hInst, NULL);

    ShowWindow(hWnd, nShow);
    UpdateWindow(hWnd);

    // Инициализация WebView2
    CreateCoreWebView2EnvironmentWithOptions(nullptr, nullptr, nullptr,
        Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
            [hWnd](HRESULT res, ICoreWebView2Environment* env) -> HRESULT {
                env->CreateCoreWebView2Controller(hWnd, Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                    [hWnd](HRESULT res, ICoreWebView2Controller* ctrl) -> HRESULT {
                        if (ctrl != nullptr) {
                            webviewController = ctrl;
                            webviewController->get_CoreWebView2(&webview);
                        }

                        // Находим путь к HTML файлу рядом с EXE
                        WCHAR szPath[MAX_PATH];
                        GetModuleFileNameW(NULL, szPath, MAX_PATH);
                        std::wstring path(szPath);
                        std::wstring dir = path.substr(0, path.find_last_of(L"\\/"));
                        std::wstring htmlPath = L"file:///" + dir + L"\\ui_mockup.html";

                        webview->Navigate(htmlPath.c_str());
                        return S_OK;
                    }).Get());
                return S_OK;
            }).Get());

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}
