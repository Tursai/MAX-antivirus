# 1. Заголовок (ваши 3 строчки)
cmake_minimum_required(VERSION 3.16)
project(MAX_Antivirus)
add_definitions(-D_CRT_SECURE_NO_WARNINGS)

# 2. Чертеж программы (команда создать MAX.exe)
# Флаг WIN32 нужен, чтобы программа открывалась красиво, без черного окна
add_executable(MAX WIN32 max_gui.cpp max.rc)

# 3. Упаковка (чтобы робот и кнопки попали в архив)
install(TARGETS MAX DESTINATION .)
install(FILES ui_mockup.html DESTINATION .)

# 4. Финальный штрих для инсталлятора
set(CPACK_PACKAGE_NAME "MAX-Antivirus")
include(CPack)
