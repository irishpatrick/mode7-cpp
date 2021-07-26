cmake_minimum_required(VERSION 3.0)

project(audio)

set(sources
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Master.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Mixer.cpp
)

set(incl
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/worker/include
)

set(audio_libs worker)

add_library(${PROJECT_NAME} STATIC ${sources})

target_include_directories(${PROJECT_NAME} PRIVATE ${incl})
target_link_libraries(${PROJECT_NAME} PRIVATE ${audio_libs})