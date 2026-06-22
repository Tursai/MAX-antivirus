cmake_minimum_required(VERSION 3.16)
project(MAX_Antivirus)

add_definitions(-D_CRT_SECURE_NO_WARNINGS)
set(CMAKE_CXX_STANDARD 17)

# СКАЧИВАЕМ ДВИЖОК ДЛЯ НАСТОЯЩЕГО ОКНА
include(FetchContent)
FetchContent_Declare(
  webview2
  URL https://www.nuget.org/api/v2/package/Microsoft.Web.WebView2/1.0.1264.42
)
FetchContent_MakeAvailable(webview2)

# СОБИРАЕМ ПРОГРАММУ
add_executable(MAX WIN32 max_gui.cpp max.rc)

# ПОДКЛЮЧАЕМ ДВИЖОК К ПРОГРАММЕ
target_include_directories(MAX PRIVATE "${webview2_SOURCE_DIR}/build/native/include")
target_link_libraries(MAX PRIVATE "${webview2_SOURCE_DIR}/build/native/x64/WebView2Loader.dll.lib")

# УКАЗЫВАЕМ, ЧТО НУЖНО ПОЛОЖИТЬ В АРХИВ
install(TARGETS MAX DESTINATION .)
install(FILES ui_mockup.html DESTINATION .)
install(FILES "${webview2_SOURCE_DIR}/build/native/x64/WebView2Loader.dll" DESTINATION .)

# НАСТРОЙКА УСТАНОВЩИКА
set(CPACK_PACKAGE_NAME "MAX-Antivirus")
set(CPACK_GENERATOR "WIX")
include(CPack)
