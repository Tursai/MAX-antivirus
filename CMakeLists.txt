cmake_minimum_required(VERSION 3.16)
project(MAX_Antivirus VERSION 1.5.0)

add_definitions(-D_CRT_SECURE_NO_WARNINGS)
set(CMAKE_CXX_STANDARD 17)

# Находим путь к библиотеке (x64)
set(WV2_LIB "${CMAKE_CURRENT_SOURCE_DIR}/build/sdk/Microsoft.Web.WebView2.1.0.1264.42/build/native/x64/WebView2Loader.dll.lib")
set(WV2_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/build/sdk/Microsoft.Web.WebView2.1.0.1264.42/build/native/include")

include_directories("${WV2_INCLUDE}")

# Собираем окно
add_executable(MAX WIN32 max_gui.cpp max.rc)

# Подключаем библиотеку по ПРЯМОМУ пути
target_link_libraries(MAX PRIVATE "${WV2_LIB}")

install(TARGETS MAX DESTINATION .)
install(FILES ui_mockup.html DESTINATION .)

set(CPACK_PACKAGE_NAME "MAX-Antivirus")
include(CPack)
cmake_minimum_required(VERSION 3.16) project(MAX_Antivirus VERSION 1.5.0) add_definitions(-D_CRT_SECURE_NO_WARNINGS) set(CMAKE_CXX_STANDARD 17)
