#include <windows.h>
#include <string>

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpCmd, int nShow) {
    // 1. Get current folder path
    WCHAR szPath[MAX_PATH];
    GetModuleFileNameW(NULL, szPath, MAX_PATH);
    std::wstring path(szPath);
    size_t pos = path.find_last_of(L"\\/");
    std::wstring dir = (pos != std::string::npos) ? path.substr(0, pos) : L".";
    
    // 2. Path to the UI file
    std::wstring htmlPath = dir + L"\\ui_mockup.html";

    // 3. Check if file exists
    if (GetFileAttributesW(htmlPath.c_str()) == INVALID_FILE_ATTRIBUTES) {
        MessageBoxA(NULL, "ERROR: ui_mockup.html not found!\nPlease extract ALL files from ZIP to the same folder and try again.", "MAX Antivirus", MB_OK | MB_ICONERROR);
        return 1;
    }

    // 4. Run UI in App Mode (No address bar, looks like real program)
    std::wstring command = L"msedge.exe --app=\"file:///" + htmlPath + L"\"";
    
    STARTUPINFOW si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    if (!CreateProcessW(NULL, (LPWSTR)command.c_str(), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        // Fallback: try opening with default browser if Edge App mode fails
        ShellExecuteW(NULL, L"open", htmlPath.c_str(), NULL, NULL, SW_SHOWNORMAL);
    } else {
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }
    
    return 0;
}
