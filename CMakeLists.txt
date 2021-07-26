cmake_minimum_required(VERSION 3.2)

project(mode7 C CXX)

if (MSVC)
    set(flags ${flags} /W3 /D_CRT_SECURE_NO_WARNINGS)
    if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
        set(flags ${flags} /D_BUILD_DEBUG_TOOLS)
    endif()
else()
    set(flags ${flags} -Wall -std=c++17 -pthread)
    if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
        set(flags ${flags} -D_BUILD_DEBUG_TOOLS -pg)
    endif()
    set(libs X11)
endif()

find_program(PYTHON3 python3)
if (NOT PYTHON3)
    find_program(PYTHON3 python)
endif()

if (WIN32)
add_custom_target(copy_assets
    ALL
    COMMAND robocopy /MIR /NFL /NDL /NJH /NJS /NP /NS /NC ${CMAKE_SOURCE_DIR}/assets/ ${CMAKE_CURRENT_BINARY_DIR}/assets/ /XD gimp templates blender reference
    VERBATIM
)
endif (WIN32)

if (UNIX OR MINGW)
add_custom_target(copy_assets
    ALL
    COMMAND rsync -av -q ${CMAKE_SOURCE_DIR}/assets/ ${CMAKE_CURRENT_BINARY_DIR}/assets/ --exclude gimp/ --exclude blender/ --exclude templates/ --exclude reference/
    VERBATIM
)
endif()

set(incl ${incl} 
    include 
    src 
    ./worker/include
    ./worker/src
    ./trackdata/include
    ./geometry/include
    ./qtree/include
)

set(libs ${libs} worker trackdata geometry qtree audio)

# using vcpkg
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

    set(libs
        ${libs}
        SDL2::SDL2
        SDL2::SDL2main
        SDL2::SDL2_image
        SDL2::SDL2_ttf
        GLEW::GLEW
        assimp::assimp
        LinearMath
        Bullet3Common
        BulletDynamics
        BulletSoftBody
        glm::glm
        portaudio
    )
else ()

    find_package(PkgConfig)

    pkg_check_modules(SDL2 REQUIRED sdl2)
    pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image)
    pkg_check_modules(SDL2_TTF REQUIRED SDL2_ttf)
    #pkg_check_modules(CAIRO REQUIRED cairo)
    pkg_check_modules(GLEW REQUIRED glew)
    pkg_check_modules(ASSIMP REQUIRED assimp)
    pkg_check_modules(BULLET REQUIRED bullet)

    set(incl
        ${incl}
        ${SDL2_INCLUDE_DIRS}
        ${SDL2_IMAGE_INCLUDE_DIRS}
        ${SDL2_TTF_INCLUDE_DIRS}
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
        ${SDL2_TTF_LIBRARIES}
        ${GLEW_LIBRARIES}
        #${CAIRO_LIBRARIES}
        ${ASSIMP_LIBRARIES}
        ${BULLET_LIBRARIES}
        m
        -pg
        -pthread
    )
endif()

find_path(BOOST_INCL boost)
find_path(GLM_INCL glm)
find_path(JSON_INCL nlohmann)
set(incl ${incl} ${GLM_INCL} ${BOOST_INCL} ${JSON_INCL})

set(sources
    src/AI.cpp
    src/Animation.cpp
    src/BBox.cpp
    src/Camera.cpp
    src/Car.cpp
    src/Clock.cpp
    src/Collisions.cpp
    src/ControlMap.cpp
    src/ControlSlider.cpp
    src/DebugPath.cpp
    src/DebugText.cpp
    src/DropShadow.cpp
    src/FrameBuffer.cpp
    src/Group.cpp
    src/HUD.cpp
    src/Keyboard.cpp
    src/Light.cpp
    src/Line2D.cpp
    src/main.cpp
    src/Material.cpp
    src/Mesh.cpp
    src/MeshUpdateWorker.cpp
    src/ModelLoader.cpp
    src/Object.cpp
    src/Particle.cpp
    src/ParticleEmitter.cpp
    src/ParticleUpdateWorker.cpp
    src/RacingLine.cpp
    src/ResponseCurve.cpp
    src/Scene.cpp
    src/Screen.cpp
    src/Shader.cpp
    src/TexCache.cpp
    src/Texture.cpp
    src/Timer.cpp
    src/Track.cpp
    src/Tree.cpp
    src/Util.cpp
)

add_subdirectory(${CMAKE_SOURCE_DIR}/3rdparty/Catch2)
add_executable(tests tests/test.cpp ${sources})
target_compile_options(tests PRIVATE -DTESTS)
target_include_directories(tests PRIVATE ${incl} ${CMAKE_SOURCE_DIR}/3rdparty/Catch2/src/catch2)
target_link_libraries(tests PRIVATE Catch2::Catch2 ${libs})

add_subdirectory(./geometry)
add_subdirectory(./worker)
add_subdirectory(./audio)
add_subdirectory(./qtree)
add_subdirectory(./trackdata)

add_executable(${PROJECT_NAME} ${sources})

file(GLOB assets assets/*)
file(COPY ${assets} DESTINATION ./assets)

target_compile_options(${PROJECT_NAME} PRIVATE ${CMAKE_CXX_FLAGS} ${flags})
target_include_directories(${PROJECT_NAME} PRIVATE ${incl})
target_link_directories(${PROJECT_NAME} PRIVATE ${CMAKE_BINARY_DIR}/worker)
target_link_libraries(${PROJECT_NAME} PRIVATE ${libs})

if (${CMAKE_BUILD_TYPE} STREQUAL "Debug" AND NOT MSVC)
    add_subdirectory(./tools/svg2track)
endif()
