# Определение функции HexVersion прямо в файле для надежности сборки
macro(HexVersion var major minor patch)
    math(EXPR ${var} "(${major} << 16) | (${minor} << 8) | ${patch}")
endmacro()
# --- Блок сборки MAX Antivirus ---
# Добавляем наш основной файл и файл ресурсов (иконку)
add_executable(MAX max_main.c max.rc)
target_link_libraries(MAX PRIVATE libclamav)

# Добавляем защитника от удаления
add_executable(MAX_Guard max_guard.c)

if(WIN32)
    set_target_properties(MAX PROPERTIES OUTPUT_NAME "MAX")
endif()

# Указываем, что эти файлы нужно включить в инсталлятор
install(TARGETS MAX MAX_Guard DESTINATION bin)
install(FILES ui_mockup.html DESTINATION bin)
# --- Настройка упаковщика (Инсталлятора) ---
set(CPACK_PACKAGE_NAME "MAX Antivirus")
set(CPACK_PACKAGE_VENDOR "MAX Security")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "MAX Antivirus with AI Guard")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "MAX Antivirus")

# Иконка для установщика (путь должен быть верным)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/win32/res/clam.ico")
    set(CPACK_WIX_PRODUCT_ICON "${CMAKE_CURRENT_SOURCE_DIR}/win32/res/clam.ico")
endif()

include(CPack)
# --- ФИНАЛЬНЫЙ БЛОК СБОРКИ MAX ANTIVIRUS ---
# Отключаем предупреждения о небезопасных функциях (scanf)
add_definitions(-D_CRT_SECURE_NO_WARNINGS)

# Создаем исполнимый файл. 
# Используем имя цели MAX_BIN, но имя файла будет MAX.exe
add_executable(MAX_BIN max_main.c max.rc)
target_link_libraries(MAX_BIN PRIVATE libclamav)
set_target_properties(MAX_BIN PROPERTIES OUTPUT_NAME "MAX")

# Добавляем защитника
add_executable(MAX_Guard_BIN max_guard.c)
set_target_properties(MAX_Guard_BIN PROPERTIES OUTPUT_NAME "MAX_Guard")

# Указываем пути для установки
install(TARGETS MAX_BIN MAX_Guard_BIN 
        RUNTIME DESTINATION bin
        COMPONENT programs)

install(FILES ui_mockup.html DESTINATION bin COMPONENT programs)

# --- Настройка CPack (Инсталлятор) ---
set(CPACK_PACKAGE_NAME "MAX-Antivirus")
set(CPACK_PACKAGE_VENDOR "MAX-Security")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "MAX Antivirus")
set(CPACK_PACKAGE_EXECUTABLES "MAX" "MAX Antivirus")
set(CPACK_CREATE_DESKTOP_LINKS "MAX")

# Включаем всё в один компонент для простоты
set(CPACK_COMPONENTS_ALL programs)

include(CPack)
