cmake_minimum_required(VERSION 3.16)
project(MAX_Antivirus VERSION 1.1.0)

# Определение функции HexVersion
macro(HexVersion var major minor patch)
    math(EXPR ${var} "(${major} << 16) | (${minor} << 8) | ${patch}")
endmacro()

# Включаем основные настройки ClamAV (чтобы работала libclamav)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
include(Version)
include(CMakeOptions)

# Отключаем предупреждения
add_definitions(-D_CRT_SECURE_NO_WARNINGS)

# Подключаем папки движка (обязательно!)
add_subdirectory(libclamav)

# --- СБОРКА MAX ANTIVIRUS ---
# Создаем цель MAX
add_executable(MAX WIN32 max_gui.cpp max.rc)
target_link_libraries(MAX PRIVATE libclamav)

# Создаем защитника MAX_Guard
add_executable(MAX_Guard max_guard.c)

# Установка
install(TARGETS MAX MAX_Guard DESTINATION .)
install(FILES ui_mockup.html DESTINATION .)

# --- Настройка CPack ---
set(CPACK_PACKAGE_NAME "MAX-Antivirus")
set(CPACK_PACKAGE_VENDOR "MAX-Security")
set(CPACK_PACKAGE_VERSION "1.1.0")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "MAX Antivirus")
set(CPACK_PACKAGE_EXECUTABLES "MAX" "MAX Antivirus")
set(CPACK_CREATE_DESKTOP_LINKS "MAX")

include(CPack)
