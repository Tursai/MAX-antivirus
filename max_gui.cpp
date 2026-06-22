#include <windows.h>
#include <string>

// Простая функция для запуска интерфейса
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpCmd, int nShow) {
    // Получаем путь к нашему HTML файлу
    WCHAR szPath[MAX_PATH];
    GetModuleFileNameW(NULL, szPath, MAX_PATH);
    std::wstring path(szPath);
    std::wstring dir = path.substr(0, path.find_last_of(L"\\/"));
    std::wstring htmlPath = dir + L"\\ui_mockup.html";

    // Запускаем наш интерфейс через системный браузер в режиме окна
    std::wstring command = L"msedge --app=\"file:///" + htmlPath + L"\"";
    
    STARTUPINFOW si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    if (CreateProcessW(NULL, (LPWSTR)command.c_str(), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    } else {
        MessageBoxA(NULL, "Ошибка: Не удалось загрузить интерфейс робота!", "MAX Error", MB_OK);
    }

    return 0;
}
