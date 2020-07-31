cmake_minimum_required(VERSION 3.0)

project(sim C)

file(GLOB sources *.c *.h)
add_executable(${PROJECT_NAME} ${sources})
target_compile_options(${PROJECT_NAME} PRIVATE -g -Wall)
target_link_libraries(${PROJECT_NAME} PRIVATE m)