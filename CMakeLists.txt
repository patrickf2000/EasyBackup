cmake_minimum_required(VERSION 3.0.0)
project(EasyBackup)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 REQUIRED COMPONENTS
    Core
    Widgets
)

add_compile_options(-std=c++11)

add_subdirectory(src)

if (UNIX)
    if (APPLE)
    else()
        install(FILES easy-backup.desktop DESTINATION /usr/share/applications)
        install(CODE "execute_process(COMMAND update-desktop-database)")
    endif()
endif()

