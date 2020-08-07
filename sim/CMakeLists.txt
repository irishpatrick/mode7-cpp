cmake_minimum_required(VERSION 3.0)

project(sim C)

file(GLOB sources *.c *.h)

find_package(PkgConfig)
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(CAIRO REQUIRED cairo)

add_executable(${PROJECT_NAME} ${sources})
target_compile_options(${PROJECT_NAME} PRIVATE -g -Wall)
target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_INCLUDE_DIRS} ${CAIRO_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE m ${SDL2_LIBRARIES} ${CAIRO_LIBRARIES})