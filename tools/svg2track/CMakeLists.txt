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
    ${CMAKE_CURRENT_SOURCE_DIR}/src/svg.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/track.c
    ${CMAKE_CURRENT_SOURCE_DIR}/src/wavefront.c
)

message(STATUS ${sources})

find_package(PkgConfig)

pkg_check_modules(CAIRO REQUIRED cairo)

find_library(LIBMSVG_L libmsvg.a NO_CACHE REQUIRED PATHS ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/libmsvg/src)
find_path(LIBMSVG_I msvg.h REQUIRED PATHS ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/libmsvg/src)
find_path(LIBZIP_I zip.h REQUIRED PATHS ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/libzip/lib)
find_path(LIBZIP_ZIPCONF zipconf.h REQUIRED PATHS ${CMAKE_CURRENT_BINARY_DIR}/3rdparty/libzip)
find_library(LIBZIP_L libzip.so NO_CACHE REQUIRED PATHS ${CMAKE_CURRENT_BINARY_DIR}/3rdparty/libzip/lib)

set(cflags -D_GNU_SOURCE -Wall)
set(incl 
    ${LIBMSVG_I} 
    ${LIBZIP_I}
    ${LIBZIP_ZIPCONF}
    ${CAIRO_INCLUDE_DIRS}
    ${ZIP_INCLUDE_DIRS}
    ../../trackdata/include 
    ../../geometry/include
)

set(libs
    ${LIBMSVG_L}
    ${LIBZIP_L}
    ${CAIRO_LIBRARIES}
    m
    trackdata
)

set(BUILD_TOOLS OFF CACHE BOOL "Build tools in the src directory (zipcmp, zipmerge, ziptool)" FORCE)
set(BUILD_REGRESS OFF CACHE BOOL "Build regression tests" FORCE)
set(BUILD_EXAMPLES OFF CACHE BOOL "Build examples" FORCE)
set(BUILD_DOC OFF CACHE BOOL "Build documentation" FORCE)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/libzip)

add_executable(${PROJECT_NAME} ${sources})
target_compile_options(${PROJECT_NAME} PRIVATE ${cflags})
target_include_directories(${PROJECT_NAME} PRIVATE ${incl})
target_link_libraries(${PROJECT_NAME} PRIVATE ${libs})
