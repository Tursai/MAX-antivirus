# --- БЛОК СБОРКИ MAX ANTIVIRUS ---
add_definitions(-D_CRT_SECURE_NO_WARNINGS)

# Сборка ГРАФИЧЕСКОЙ версии (без черного окна)
add_executable(MAX WIN32 max_gui.cpp max.rc)
target_link_libraries(MAX PRIVATE libclamav)

# Сборка защитника
add_executable(MAX_Guard max_guard.c)

# Установка
install(TARGETS MAX MAX_Guard RUNTIME DESTINATION bin)
install(FILES ui_mockup.html DESTINATION bin)

# --- Настройка Ярлыков и Инсталлятора ---
set(CPACK_PACKAGE_NAME "MAX-Antivirus")
set(CPACK_PACKAGE_VENDOR "MAX-Security")
set(CPACK_PACKAGE_VERSION "1.1.0")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "MAX Antivirus")
set(CPACK_PACKAGE_EXECUTABLES "MAX" "MAX Antivirus")
set(CPACK_CREATE_DESKTOP_LINKS "MAX")

include(CPack)
