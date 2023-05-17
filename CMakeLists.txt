cmake_minimum_required(VERSION 3.24)
project(SeedRipper)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "/usr/lib/x86_64-linux-gnu/cmake")

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(SeedRipper main.cpp DropArea.cpp DropArea.h DropSiteWindow.cpp DropSiteWindow.h)
target_link_libraries(SeedRipper
        Qt::Core
        Qt::Gui
        Qt::Widgets
        )
