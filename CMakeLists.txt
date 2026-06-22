# --- ГАРАНТИРОВАННАЯ СБОРКА MAX ANTIVIRUS ---
add_definitions(-D_CRT_SECURE_NO_WARNINGS)

# Создаем цель с уникальным именем MAX_TARGET
add_executable(MAX_TARGET WIN32 max_gui.cpp max.rc)
target_link_libraries(MAX_TARGET PRIVATE libclamav)
# А файл на выходе все равно будет называться MAX.exe
set_target_properties(MAX_TARGET PROPERTIES OUTPUT_NAME "MAX")

# То же самое для защитника
add_executable(MAX_GUARD_TARGET max_guard.c)
set_target_properties(MAX_GUARD_TARGET PROPERTIES OUTPUT_NAME "MAX_Guard")

# Установка (теперь без лишних подпапок для надежности)
install(TARGETS MAX_TARGET MAX_GUARD_TARGET DESTINATION .)
install(FILES ui_mockup.html DESTINATION .)

# --- Настройка CPack ---
set(CPACK_PACKAGE_NAME "MAX-Antivirus")
set(CPACK_PACKAGE_VENDOR "MAX-Security")
set(CPACK_PACKAGE_VERSION "1.1.0")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "MAX Antivirus")
set(CPACK_PACKAGE_EXECUTABLES "MAX" "MAX Antivirus")
set(CPACK_CREATE_DESKTOP_LINKS "MAX")



# Настройка CPack
set(CPACK_PACKAGE_NAME "MAX-Antivirus")
set(CPACK_PACKAGE_VENDOR "MAX-Security")
set(CPACK_PACKAGE_VERSION "1.1.0")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "MAX Antivirus")
set(CPACK_PACKAGE_EXECUTABLES "MAX" "MAX Antivirus")
set(CPACK_CREATE_DESKTOP_LINKS "MAX")

include(CPack)
