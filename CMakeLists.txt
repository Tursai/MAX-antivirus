cmake_minimum_required(VERSION 3.16)

project(MAX_Antivirus)

add_definitions(-D_CRT_SECURE_NO_WARNINGS)

add_executable(MAX WIN32 max_gui.cpp)

install(TARGETS MAX DESTINATION .)
install(FILES ui_mockup.html DESTINATION .)
