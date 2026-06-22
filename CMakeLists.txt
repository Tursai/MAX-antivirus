cmake_minimum_required(VERSION 3.16)
project(MAX_Antivirus)

# ПРИНУДИТЕЛЬНО ВКЛЮЧАЕМ ПОДДЕРЖКУ РУССКОГО ЯЗЫКА (UTF-8)
if(MSVC)
    add_compile_options(/utf-8)
endif()

add_definitions(-D_CRT_SECURE_NO_WARNINGS)
set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(webview2 URL https://www.nuget.org/api/v2/package/Microsoft.Web.WebView2/1.0.1264.42)
FetchContent_MakeAvailable(webview2)

add_executable(MAX WIN32 max_gui.cpp max.rc)
target_include_directories(MAX PRIVATE "${webview2_SOURCE_DIR}/build/native/include")
target_link_libraries(MAX PRIVATE "${webview2_SOURCE_DIR}/build/native/x64/WebView2Loader.dll.lib")

install(TARGETS MAX DESTINATION .)
