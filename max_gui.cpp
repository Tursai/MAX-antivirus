#include <windows.h>

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
    if (msg == WM_DESTROY) PostQuitMessage(0);
    return DefWindowProc(hWnd, msg, wp, lp);
}

int WINAPI WinMain(HINSTANCE hI, HINSTANCE hP, LPSTR lp, int nS) {
    WNDCLASSW wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hI;
    wc.lpszClassName = L"MAX";
    RegisterClassW(&wc);
    HWND hWnd = CreateWindowW(L"MAX", L"MAX Antivirus", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 200, 200, 800, 600, 0, 0, hI, 0);
    
    MessageBoxA(hWnd, "MAX Antivirus УСПЕШНО СОБРАН И ЗАПУЩЕН!", "MAX Status", MB_OK);

    MSG msg;
    while (GetMessage(&msg, 0, 0, 0)) {
        DispatchMessage(&msg);
    }
    return 0;
}
