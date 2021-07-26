cmake_minimum_required(VERSION 3.0)

project(audio)

set(sources
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Master.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Mixer.cpp
)

set(audio_incl
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/worker/include
)

if (NOT ${CMAKE_TOOLCHAIN_FILE} STREQUAL "")
    find_package(SDL2 CONFIG REQUIRED)
    find_package(sdl2-image CONFIG REQUIRED)
    find_package(sdl2-ttf CONFIG REQUIRED)
    find_package(GLEW CONFIG REQUIRED)
    find_package(assimp CONFIG REQUIRED)
    find_package(Bullet CONFIG REQUIRED)
    find_package(glm CONFIG REQUIRED)
    find_package(portaudio CONFIG REQUIRED)

    set(incl
        ${incl}
    )

    set(audio_libs
        ${audio_libs}
        portaudio
    )
else ()

    find_package(PkgConfig)
    pkg_check_modules(PA REQUIRED portaudio-2.0)

    set(incl
        ${incl}
        ${PA_INCLUDE_DIRS}
    )

    set(libs
        ${libs}
        ${PA_LIBRARIES}
        m
        -pg
    )
endif()

set(audio_libs ${audio_libs} worker)

add_library(${PROJECT_NAME} STATIC ${sources})

target_include_directories(${PROJECT_NAME} PRIVATE ${audio_incl})
target_link_libraries(${PROJECT_NAME} PRIVATE ${audio_libs})