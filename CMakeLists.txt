cmake_minimum_required(VERSION 3.16)
project(MAX_Antivirus VERSION 1.1.0)

# Определение HexVersion для надежности
macro(HexVersion var major minor patch)
    math(EXPR ${var} "(${major} << 16) | (${minor} << 8) | ${patch}")
endmacro()

# Отключаем предупреждения компилятора
add_definitions(-D_CRT_SECURE_NO_WARNINGS)

# Сборка ГРАФИЧЕСКОЙ версии MAX (без черного окна)
# Используем WIN32 чтобы приложение было оконным
add_executable(MAX_BIN WIN32 max_gui.cpp max.rc)
target_link_libraries(MAX_BIN PRIVATE libclamav)
set_target_properties(MAX_BIN PROPERTIES OUTPUT_NAME "MAX")

# Сборка защитника (консольный)
add_executable(MAX_Guard_BIN max_guard.c)
set_target_properties(MAX_Guard_BIN PROPERTIES OUTPUT_NAME "MAX_Guard")

# Установка файлов в папку bin
install(TARGETS MAX_BIN MAX_Guard_BIN RUNTIME DESTINATION bin)
install(FILES ui_mockup.html DESTINATION bin)

# --- Настройка Ярлыков и Инсталлятора ---
set(CPACK_PACKAGE_NAME "MAX-Antivirus")
set(CPACK_PACKAGE_VENDOR "MAX-Security")
set(CPACK_PACKAGE_VERSION "1.1.0")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "MAX Antivirus")

# Создаем ярлык на рабочем столе и в Пуске
set(CPACK_PACKAGE_EXECUTABLES "MAX" "MAX Antivirus")
set(CPACK_CREATE_DESKTOP_LINKS "MAX")

include(CPack)
