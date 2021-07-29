# This file is part of mode7-cpp.
# Copyright (C) 2021  Patrick Roche
# 
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

cmake_minimum_required(VERSION 3.0)

project(audio)

set(sources
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Master.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Mixer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/WaveformGenerator.cpp
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