cmake_minimum_required(VERSION 3.16)
project(MAX_Antivirus VERSION 1.5.0)

add_definitions(-D_CRT_SECURE_NO_WARNINGS)
set(CMAKE_CXX_STANDARD 17)

# Собираем только GUI версию. Флаг WIN32 убирает черное окно.
add_executable(MAX WIN32 max_gui.cpp max.rc)

# Указываем, что файлы должны быть вместе
install(TARGETS MAX DESTINATION .)
install(FILES ui_mockup.html DESTINATION .)

set(CPACK_PACKAGE_NAME "MAX-Antivirus")
include(CPack)
