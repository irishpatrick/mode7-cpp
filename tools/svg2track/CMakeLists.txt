cmake_minimum_required(VERSION 3.0)

project(svg2track C)

file(GLOB sources ./src/*)

find_library(LIBMSVG_L libmsvg.a REQUIRED PATHS ${CMAKE_SOURCE_DIR}/3rdparty/libmsvg/src)
find_path(LIBMSVG_I msvg.h REQUIRED PATHS ${CMAKE_SOURCE_DIR}/3rdparty/libmsvg/src)

set(cflags -D_GNU_SOURCE -g -Wall)
set(incl ${LIBMSVG_I})
set(libs ${LIBMSVG_L})

add_executable(${PROJECT_NAME} ${sources})
target_compile_options(${PROJECT_NAME} PRIVATE ${cflags})
target_include_directories(${PROJECT_NAME} PRIVATE ${incl})
target_link_libraries(${PROJECT_NAME} PRIVATE ${libs} -lm)