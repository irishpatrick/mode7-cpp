# mode7

## System Requirements

- OpenGL 3.3
- 1G RAM
- 1G Storage

## Game Dependencies

- SDL2
- SDL2_image
- SDL2_ttf
- GLEW
- GLM
- Assimp
- Bullet
- Nlohmann JSON
- Boost

## Testing and Devtool Dependencies

- Go
- Rust
- Cairo

## Build Dependencies

- C++ compiler
- CMake + preferred buildsystem
- Python3
- rsync (linux)
- robocopy (windows)

## Installing Dependencies

### Libraries

- Vcpkg (windows or linux)
- Any other way, as long as they\'re available through pkg-config (linux)

### Toolchains

- Any way

## Building

### Linux

#### Using vcpkg

- `mkdir build`
- `cd build`
- `cmake .. -DCMAKE_TOOLCHAIN_FILE=<path to vcpkg.cmake> -DCMAKE_BUILD_TYPE=<preferred build type>`
- `make`

#### Finding packages with pkg-config

- `mkdir build`
- `cd build`
- `cmake .. -DCMAKE_BUILD_TYPE=<preferred build type>`
- `make`

### Windows

- Using Visual Studio
- Go to File -> Open -> CMake...
- Select CMakeLists.txt in root directory
- Right click CMakeLists.txt, select CMake Settings for ...
- Set CMAKE_TOOLCHAIN_FILE to path to vcpkg.cmake

Devtools won\'t be built on windows, but can be built with MINGW, WSL, etc.