cmake_minimum_required(VERSION 3.16)
project(MAX_Antivirus VERSION 1.2.1)

add_definitions(-D_CRT_SECURE_NO_WARNINGS)

# Теперь собираем консольное приложение для отладки (убрали WIN32)
add_executable(MAX max_gui.cpp max.rc)

# Пока отключим линковку с ClamAV, чтобы проверить только запуск интерфейса
# target_link_libraries(MAX PRIVATE libclamav)

install(TARGETS MAX DESTINATION .)
install(FILES ui_mockup.html DESTINATION .)

set(CPACK_PACKAGE_NAME "MAX-Antivirus-DEBUG")
include(CPack)
