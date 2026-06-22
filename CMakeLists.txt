cmake_minimum_required(VERSION 3.16)
project(MAX_Antivirus VERSION 1.1.0)

# Макрос для версий
macro(HexVersion var major minor patch)
    math(EXPR ${var} "(${major} << 16) | (${minor} << 8) | ${patch}")
endmacro()

# Базовые настройки
set(PACKAGE_VERSION "1.1.0")
add_definitions(-D_CRT_SECURE_NO_WARNINGS)
set(CMAKE_CXX_STANDARD 17)

# Ищем зависимости через vcpkg
find_package(CURL REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(ZLIB REQUIRED)
find_package(JSONC REQUIRED)

# Включаем основные папки (если они есть)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/libclamav/CMakeLists.txt")
    add_subdirectory(libclamav)
    set(CLAMAV_LIB libclamav)
else()
    # Если папки нет, попробуем собрать только интерфейс для теста
    message(WARNING "Папка libclamav не найдена. Сборка только оболочки.")
endif()

# --- СБОРКА MAX ANTIVIRUS ---
# Основное окно (GUI)
add_executable(MAX WIN32 max_gui.cpp max.rc)
if(TARGET libclamav)
    target_link_libraries(MAX PRIVATE libclamav)
endif()

# Защитник удаления
add_executable(MAX_Guard max_guard.c)

# Установка
install(TARGETS MAX MAX_Guard DESTINATION .)
install(FILES ui_mockup.html DESTINATION .)

# --- Инсталлятор ---
set(CPACK_PACKAGE_NAME "MAX-Antivirus")
set(CPACK_PACKAGE_VENDOR "MAX-Security")
set(CPACK_PACKAGE_VERSION ${PACKAGE_VERSION})
set(CPACK_PACKAGE_INSTALL_DIRECTORY "MAX Antivirus")
set(CPACK_PACKAGE_EXECUTABLES "MAX" "MAX Antivirus")
set(CPACK_CREATE_DESKTOP_LINKS "MAX")

include(CPack)
