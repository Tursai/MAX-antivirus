#include <windows.h>
#include <string>

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpCmd, int nShow) {
    WCHAR szPath[MAX_PATH];
    GetModuleFileNameW(NULL, szPath, MAX_PATH);
    std::wstring path(szPath);
    size_t pos = path.find_last_of(L"\\/");
    std::wstring dir = (pos != std::string::npos) ? path.substr(0, pos) : L".";
    std::wstring htmlPath = dir + L"\\ui_mockup.html";

    if (GetFileAttributesW(htmlPath.c_str()) == INVALID_FILE_ATTRIBUTES) {
        MessageBoxW(NULL, L"Ошибка: Файл ui_mockup.html не найден!\nРаспакуйте ВСЕ файлы из архива в одну папку.", L"MAX Antivirus", MB_OK | MB_ICONERROR);
        return 1;
    }

    std::wstring command = L"msedge.exe --app=\"file:///" + htmlPath + L"\"";
    
    STARTUPINFOW si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    if (CreateProcessW(NULL, (LPWSTR)command.c_str(), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    } else {
        MessageBoxW(NULL, L"Ошибка: Не удалось запустить интерфейс!", L"MAX Antivirus", MB_OK | MB_ICONERROR);
    }
    return 0;
}
