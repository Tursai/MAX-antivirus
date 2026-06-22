cmake_minimum_required(VERSION 3.16)
project(MAX_Antivirus VERSION 1.5.0)

add_definitions(-D_CRT_SECURE_NO_WARNINGS)
set(CMAKE_CXX_STANDARD 17)

# Указываем пути к WebView2 (который скачает GitHub)
include_directories("build/sdk/Microsoft.Web.WebView2.1.0.1264.42/build/native/include")
link_directories("build/sdk/Microsoft.Web.WebView2.1.0.1264.42/build/native/x64")

# Собираем GUI версию
add_executable(MAX WIN32 max_gui.cpp max.rc)

# Подключаем саму библиотеку
target_link_libraries(MAX PRIVATE "build/sdk/Microsoft.Web.WebView2.1.0.1264.42/build/native/x64/WebView2Loader.dll.lib")

install(TARGETS MAX DESTINATION .)
install(FILES ui_mockup.html DESTINATION .)

set(CPACK_PACKAGE_NAME "MAX-Antivirus")
include(CPack)
