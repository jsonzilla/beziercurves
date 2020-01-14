cmake_minimum_required(VERSION 3.1.0)

project(simple_gui_template)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 COMPONENTS Widgets REQUIRED)

get_target_property(QtCore_location Qt5::Core LOCATION)

add_executable(simple_gui_template
    mainwindow.ui
    mainwindow.cpp
    main.cpp
    resources.qrc
)

target_link_libraries(simple_gui_template Qt5::Widgets)