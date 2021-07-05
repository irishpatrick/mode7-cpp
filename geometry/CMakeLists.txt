cmake_minimum_required(VERSION 3.0)

project(geometry C CXX)

set(sources
    ${CMAKE_CURRENT_SOURCE_DIR}/src/bezier.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/geometry.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/line.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Line.cpp
)

set(incl
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_library(${PROJECT_NAME} STATIC ${sources})

target_include_directories(${PROJECT_NAME} PRIVATE ${incl})