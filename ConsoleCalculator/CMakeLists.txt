cmake_minimum_required(VERSION 3.16)
project(ConsoleCalculator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Core)

add_executable(ConsoleCalculator
    main.cpp
    calculator.cpp
    calculator.h
)

target_link_libraries(ConsoleCalculator PRIVATE Qt6::Core)
