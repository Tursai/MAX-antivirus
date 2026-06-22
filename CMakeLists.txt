cmake_minimum_required(VERSION 3.16)
project(MAX_Antivirus VERSION 1.2.0)

add_definitions(-D_CRT_SECURE_NO_WARNINGS)
set(CMAKE_CXX_STANDARD 17)

# Настройка vcpkg
set(X_VCPKG_APPLOCAL_DEPS_INSTALL ON)

find_package(CURL REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(ZLIB REQUIRED)
find_package(json-c CONFIG REQUIRED)

# Сборка MAX (с поддержкой GUI)
add_executable(MAX WIN32 max_gui.cpp max.rc)
target_link_libraries(MAX PRIVATE OpenSSL::SSL OpenSSL::Crypto CURL::libcurl json-c::json-c ZLIB::ZLIB)

# Сборка защитника
add_executable(MAX_Guard max_guard.c)


# Установка программы и всех ресурсов для интерфейса
install(TARGETS MAX MAX_Guard DESTINATION bin)
install(FILES ui_mockup.html mascot.png DESTINATION bin) # Добавили mascot.png здесь!
