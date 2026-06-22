cmake_minimum_required(VERSION 3.16)
project(MAX_Antivirus)

set(CMAKE_CXX_STANDARD 17)
add_definitions(-D_CRT_SECURE_NO_WARNINGS)

# Авто-скачивание графического движка WebView2
include(FetchContent)
FetchContent_Declare(
  webview2
  URL https://www.nuget.org/api/v2/package/Microsoft.Web.WebView2/1.0.1264.42
)
FetchContent_MakeAvailable(webview2)

# Сборка программы
add_executable(MAX WIN32 max_gui.cpp max.rc)

# Подключение библиотек
target_include_directories(MAX PRIVATE "${webview2_SOURCE_DIR}/build/native/include")
target_link_libraries(MAX PRIVATE "${webview2_SOURCE_DIR}/build/native/x64/WebView2Loader.dll.lib" shlwapi)

# Установка (все файлы в одну папку)
install(TARGETS MAX DESTINATION .)
install(FILES ui_mockup.html mascot.png DESTINATION .)
install(FILES "${webview2_SOURCE_DIR}/build/native/x64/WebView2Loader.dll" DESTINATION .)
