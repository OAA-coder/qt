cmake_minimum_required(VERSION 3.16)
project(QtObjectTree LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

add_executable(QtObjectTree
    main.cpp
    mainwindow.cpp
    mainwindow.h
    widgets.h
)

if(WIN32)
    set_target_properties(QtObjectTree PROPERTIES WIN32_EXECUTABLE FALSE)
endif()

target_link_libraries(QtObjectTree PRIVATE Qt6::Core Qt6::Widgets)