cmake_minimum_required(VERSION 3.0)

project(mode7 C CXX)

if (MSVC)
    set(flags /W3 /DEBUG /D_CRT_SECURE_NO_WARNINGS)
else()
    set(flags -Wall -g -std=c++17)
endif()

set(incl include)
set(libs "")

# using vcpkg
if (NOT ${CMAKE_TOOLCHAIN_FILE} STREQUAL "")

    find_package(SDL2 CONFIG REQUIRED)
    find_package(sdl2-image CONFIG REQUIRED)
    find_package(sdl2-ttf CONFIG REQUIRED)
    find_package(GLEW CONFIG REQUIRED)

    set(libs
        ${libs}
        SDL2::SDL2
        SDL2::SDL2main
        SDL2::SDL2_image
        SDL2::SDL2_ttf
        GLEW::GLEW
        OpenGL32
    )

else ()

    find_package(PkgConfig)

    pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
    pkg_check_modules(CAIRO REQUIRED cairo)

    set(incl
        ${incl}
        ${GTK3_INCLUDE_DIRS}
        ${CAIRO_INCLUDE_DIRS}
    )

    set(libs
        ${libs}
        ${GTK3_LIBRARIES}
        ${CAIRO_LIBRARIES}
	    m
    )
endif()

find_path(BOOST_INCL boost)
find_path(GLM_INCL glm)
find_path(JSON_INCL nlohmann)
set(incl ${incl} ${GLM_INCL} ${BOOST_INCL} ${JSON_INCL})

file(GLOB sources src/* include/*)
add_executable(${PROJECT_NAME} ${sources})

file(GLOB assets assets/*)
file(COPY ${assets} DESTINATION ./assets)

target_compile_options(${PROJECT_NAME} PRIVATE ${flags})
target_include_directories(${PROJECT_NAME} PRIVATE ${incl})
target_link_libraries(${PROJECT_NAME} PRIVATE ${libs})
