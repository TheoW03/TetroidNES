cmake_minimum_required(VERSION 3.16)
project(CMakeSFMLProject LANGUAGES CXX)

set(CMAKE_EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/..)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/..)
set(EXECUTABLE_NAME Tetroid)
set(UNIT_TEST_EXE Emulator_tests)
string(APPEND CMAKE_CXX_FLAGS "-Wall")
# string(APPEND CMAKE_CXX_FLAGS "-g")
# set(CMAKE_BUILD_TYPE Debug)
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)



if(DEFINED UNITTESTS AND UNITTESTS STREQUAL "ON")
  enable_testing()
  add_executable(
  ${UNIT_TEST_EXE}
  src/unitTests/RunUnitTests.cc
  src/unitTests/emulator_test.cc
    src/SetupSFML.cpp
    src/emulator/APU.cpp
    src/emulator/AddressMode.cpp
    src/emulator/BitOperations.cpp
    src/emulator/Bus.cpp
    src/emulator/Computer.cpp
    src/emulator/Instructions.cpp
    src/emulator/LoadRom.cpp
    src/emulator/PPU.cpp
    src/emulator/StatusRegister.cpp
    src/emulator/NESError.cpp
    src/emulator/InstructionMap.cpp
)
target_link_libraries(
  ${UNIT_TEST_EXE}

  GTest::gtest_main
)
target_include_directories(${UNIT_TEST_EXE} PRIVATE ${CMAKE_SOURCE_DIR}/src/include)

target_link_libraries(
  ${UNIT_TEST_EXE}
    sfml-graphics
)
include(GoogleTest)
gtest_discover_tests(  ${UNIT_TEST_EXE})
target_compile_features(  ${UNIT_TEST_EXE} PRIVATE cxx_std_20)


install(TARGETS   ${UNIT_TEST_EXE}
)
else()
add_executable(${EXECUTABLE_NAME} src/main.cpp
    src/SetupSFML.cpp
    src/emulator/APU.cpp
    src/emulator/AddressMode.cpp
    src/emulator/BitOperations.cpp
    src/emulator/Bus.cpp
    src/emulator/Computer.cpp
    src/emulator/Instructions.cpp
    src/emulator/LoadRom.cpp
    src/emulator/PPU.cpp
    src/emulator/StatusRegister.cpp
    src/emulator/NESError.cpp
    src/emulator/InstructionMap.cpp
)

target_include_directories(${EXECUTABLE_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src/include)

target_link_libraries(${EXECUTABLE_NAME} PRIVATE sfml-graphics)
target_compile_features(${EXECUTABLE_NAME} PRIVATE cxx_std_20)
install(TARGETS ${EXECUTABLE_NAME})


endif()



# if(WIN32)
# add_custom_command(
# TARGET ${EXECUTABLE_NAME}
# COMMENT "Copy OpenAL DLL"
# PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>
# VERBATIM)
# endif()
# uncomment if you are running this in Visual Studio or if you get an error on windows with the current setup
