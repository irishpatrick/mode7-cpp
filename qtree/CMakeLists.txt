cmake_minimum_required(VERSION 3.0)

project(qtree C CXX)

set(sources
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Box.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Bucket.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/DummyMember.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/QTree.cpp
)

set(incl
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set(libs
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