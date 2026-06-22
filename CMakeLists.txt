
cmake_minimum_required(VERSION 3.16) project(MAX_Antivirus VERSION 1.5.0)
Глобальные настройки
add_definitions(-D_CRT_SECURE_NO_WARNINGS) set(CMAKE_CXX_STANDARD 17)
Собираем основной файл программы
add_executable(MAX WIN32 max_gui.cpp max.rc)
Указываем, какие файлы должны попасть в архив
install(TARGETS MAX DESTINATION .) install(FILES ui_mockup.html DESTINATION .)
Настройка для создания архива
set(CPACK_PACKAGE_NAME "MAX-Antivirus-Result") include(CPack)
