cmake_minimum_required(VERSION 3.16)
project(MAX_Antivirus)

# Отключаем лишние предупреждения
add_definitions(-D_CRT_SECURE_NO_WARNINGS)

# Собираем основной файл программы
add_executable(MAX WIN32 max_gui.cpp max.rc)

# Указываем, какие файлы должны попасть в установщик
install(TARGETS MAX DESTINATION .)
install(FILES ui_mockup.html DESTINATION .)

# --- НАСТРОЙКА ИНСТАЛЛЯТОРА (MSI) ---
set(CPACK_PACKAGE_NAME "MAX-Antivirus")
set(CPACK_PACKAGE_VENDOR "MAX-Security")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_GENERATOR "WIX")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "MAX Antivirus")

# Включаем модуль создания инсталлятора
include(CPack)
