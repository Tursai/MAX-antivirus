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
