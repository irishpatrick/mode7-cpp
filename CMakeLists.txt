cmake_minimum_required(VERSION 3.0)

project(mode7 C CXX)

if (MSVC)
    set(flags /W3 /DEBUG /D_CRT_SECURE_NO_WARNINGS)
else()
    set(flags -Wall -g -std=c++17)
    set(libs -lX11)
endif()

set(incl include)

# using vcpkg
if (NOT ${CMAKE_TOOLCHAIN_FILE} STREQUAL "")

    message(STATUS "Using vcpkg")

    find_package(SDL2 CONFIG REQUIRED)
    find_package(sdl2-image CONFIG REQUIRED)
    #find_package(sdl2-ttf CONFIG REQUIRED)
    find_package(GLEW CONFIG REQUIRED)
    find_package(assimp CONFIG REQUIRED)
    find_package(Bullet CONFIG REQUIRED)

    set(libs
        ${libs}
        SDL2::SDL2
        SDL2::SDL2main
        SDL2::SDL2_image
	#SDL2::SDL2_ttf
        GLEW::GLEW
        assimp::assimp
        LinearMath
        Bullet3Common
        BulletDynamics
        BulletSoftBody
    )

else ()

    find_package(PkgConfig)

    pkg_check_modules(SDL2 REQUIRED sdl2)
    pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image)
    #pkg_check_modules(SDL2_TTF REQUIRED SDL2_ttf)
    #pkg_check_modules(CAIRO REQUIRED cairo)
    pkg_check_modules(GLEW REQUIRED glew)
    pkg_check_modules(ASSIMP REQUIRED assimp)
    pkg_check_modules(BULLET REQUIRED bullet)

    set(incl
        ${incl}
        ${SDL2_INCLUDE_DIRS}
        ${SDL2_IMAGE_INCLUDE_DIRS}
	#${SDL2_TTF_INCLUDE_DIRS}
	#${CAIRO_INCLUDE_DIRS}
	${GLEW_INCLUDE_DIRS}
        ${NLOHMANN_JSON_INCL}
        ${ASSIMP_INCLUDE_DIRS}
        ${BULLET_INCLUDE_DIRS}
    )

    set(libs
        ${libs}
        ${SDL2_LIBRARIES}
        ${SDL2_IMAGE_LIBRARIES}
	#${SDL2_TTF_LIBRARIES}
	${GLEW_LIBRARIES}
	#${CAIRO_LIBRARIES}
        ${ASSIMP_LIBRARIES}
        ${BULLET_LIBRARIES}
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

add_subdirectory(./tools/sim)
add_subdirectory(./tools/editor)
add_subdirectory(./tools/tasm)
