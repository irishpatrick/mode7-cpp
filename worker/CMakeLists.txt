cmake_minimum_required(VERSION 3.0)

project(worker)

set(sources
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Scheduler.inl
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SyncData.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Worker.cpp
)

set(incl
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_library(${PROJECT_NAME} STATIC ${sources})

target_include_directories(${PROJECT_NAME} PRIVATE ${incl})