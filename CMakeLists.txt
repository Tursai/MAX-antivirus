cmake_minimum_required(VERSION 3.16)
project(MAX_Antivirus VERSION 1.5.0)

add_definitions(-D_CRT_SECURE_NO_WARNINGS)
set(CMAKE_CXX_STANDARD 17)

# Пути к WebView2 SDK (которые скачивает GitHub)
set(WV2_SDK "build/sdk/Microsoft.Web.WebView2.1.0.1264.42/build/native")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/${WV2_SDK}/include")
link_directories("${CMAKE_CURRENT_SOURCE_DIR}/${WV2_SDK}/x64")

# Собираем только окно (GUI)
add_executable(MAX WIN32 max_gui.cpp max.rc)

# Подключаем библиотеку WebView2 (БЕЗ расширения .lib, CMake добавит его сам)
target_link_libraries(MAX PRIVATE WebView2Loader.dll)

install(TARGETS MAX DESTINATION .)
install(FILES ui_mockup.html DESTINATION .)

set(CPACK_PACKAGE_NAME "MAX-Antivirus")
include(CPack)
