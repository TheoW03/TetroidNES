cmake_minimum_required(VERSION 3.16)
project(CMakeSFMLProject LANGUAGES CXX)

set(CMAKE_EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/..)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/..)
set(EXECUTABLE_NAME output)

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

add_executable(${EXECUTABLE_NAME} src/main.cpp
    src/emulator/AddressMode.cpp
    src/emulator/BitOperations.cpp
    src/emulator/Bus.cpp
    src/emulator/Computer.cpp
    src/emulator/Instructions.cpp
    src/emulator/LoadRom.cpp
    src/emulator/Memory.cpp
    src/emulator/PPU.cpp
    src/emulator/StatusRegister.cpp
)
target_link_libraries(${EXECUTABLE_NAME} PRIVATE sfml-graphics)
target_compile_features(${EXECUTABLE_NAME} PRIVATE cxx_std_17)

if(WIN32)
    add_custom_command(
        TARGET ${EXECUTABLE_NAME}
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>
        VERBATIM)
endif()

install(TARGETS ${EXECUTABLE_NAME})