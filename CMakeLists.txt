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

file(GLOB SOURCES
    src/*.h
    src/*.cpp
    src/*.qrc
)

file(GLOB HEADERS
    src/*.h
)

file(GLOB SRC
    src/*.cpp
)

SET(TARGET_H
    ${HEADERS}
   )

SET(TARGET_SRC
    ${SRC}
   )

add_executable(simple_gui_template ${SOURCES})

target_link_libraries(simple_gui_template Qt5::Widgets)