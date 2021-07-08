cmake_minimum_required(VERSION 3.0)

project(trackdata C CXX)

set(sources
    ${CMAKE_CURRENT_SOURCE_DIR}/src/trackdata.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/TrackData.cpp
)

set(incl
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ../geometry/include
)

set(libs
    geometry
)

if (NOT ${CMAKE_TOOLCHAIN_FILE} STREQUAL "")
    find_package(glm CONFIG REQUIRED)
    set(libs
        ${libs}
        glm::glm
    )
endif()

add_library(${PROJECT_NAME} STATIC ${sources})

target_include_directories(${PROJECT_NAME} PRIVATE ${incl})
target_link_libraries(${PROJECT_NAME} PRIVATE ${libs})