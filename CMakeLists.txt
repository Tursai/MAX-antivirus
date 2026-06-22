cmake_minimum_required(VERSION 3.16)
project(MAX_Antivirus VERSION 1.1.0)

# Отключаем предупреждения
add_definitions(-D_CRT_SECURE_NO_WARNINGS)

# Ищем зависимости через vcpkg (исправленные имена)
find_package(CURL REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(ZLIB REQUIRED)
find_package(json-c CONFIG REQUIRED) # Исправлено: строчные буквы и CONFIG

# Сборка ГРАФИЧЕСКОЙ версии MAX
add_executable(MAX WIN32 max_gui.cpp max.rc)

# Подключаем библиотеки. Если libclamav собрана, линкуем её, 
# если нет - используем заглушку для интерфейса.
if(TARGET libclamav)
    target_link_libraries(MAX PRIVATE libclamav)
else()
    target_link_libraries(MAX PRIVATE OpenSSL::SSL OpenSSL::Crypto CURL::libcurl json-c::json-c ZLIB::ZLIB)
endif()

# Защитник удаления
add_executable(MAX_Guard max_guard.c)

# Установка
install(TARGETS MAX MAX_Guard DESTINATION .)
install(FILES ui_mockup.html DESTINATION .)

# Настройка инсталлятора
set(CPACK_PACKAGE_NAME "MAX-Antivirus")
set(CPACK_PACKAGE_VERSION "1.1.0")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "MAX Antivirus")
set(CPACK_CREATE_DESKTOP_LINKS "MAX")
include(CPack)
