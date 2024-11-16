cmake_minimum_required(VERSION 3.16)
project(untitled)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "/home/gms/app/Qt/6.7.3/gcc_64")

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(untitled main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        circularprogressbar.cpp
        circularprogressbar.h
        floatingwidget.cpp
        floatingwidget.h
        resources.qrc)

target_link_libraries(untitled
        Qt::Core
        Qt::Gui
        Qt::Widgets
)
