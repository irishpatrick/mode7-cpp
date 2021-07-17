cmake_minimum_required(VERSION 3.0)

project(svg2track C)

set(sources
    ${CMAKE_CURRENT_SOURCE_DIR}/src/arraylist.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/assetpack.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/material.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/matrix.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/mesh.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/preview.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/track.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/wavefront.c
)

message(STATUS ${sources})

find_package(PkgConfig)

pkg_check_modules(CAIRO REQUIRED cairo)
pkg_check_modules(ZIP REQUIRED libzip)

find_library(LIBMSVG_L libmsvg.a REQUIRED PATHS ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/libmsvg/src)
find_path(LIBMSVG_I msvg.h REQUIRED PATHS ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/libmsvg/src)

set(cflags -D_GNU_SOURCE -Wall)
set(incl 
    ${LIBMSVG_I} 
    ${CAIRO_INCLUDE_DIRS}
    ${ZIP_INCLUDE_DIRS}
    ../../trackdata/include 
    ../../geometry/include
)

set(libs
    ${LIBMSVG_L}
    ${CAIRO_LIBRARIES}
    ${ZIP_LIBRARIES}
    m
    trackdata
)

add_executable(${PROJECT_NAME} ${sources})
target_compile_options(${PROJECT_NAME} PRIVATE ${cflags})
target_include_directories(${PROJECT_NAME} PRIVATE ${incl})
target_link_libraries(${PROJECT_NAME} PRIVATE ${libs})
