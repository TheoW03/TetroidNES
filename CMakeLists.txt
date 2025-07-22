cmake_minimum_required(VERSION 3.16)

project(tetroidnes VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(EXECUTABLE_NAME TetroidNES)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
set(VERSION_DEF "#define VERSION \"${PROJECT_VERSION}-Debug\"")
else()
set(VERSION_DEF "#define VERSION \"${PROJECT_VERSION}\"")
endif()

set(QT_REQUIRED_COMPONENTS Widgets Core Gui LinguistTools)

file(GLOB QT_TRANSLATION_FILES "./src/gui/qt_tetroidnes/translations/tetroidnes_*.ts")
file(GLOB QT_RCC_FILES "./src/gui/qt_tetroidnes/resources/*.qrc")
file(GLOB EMULATOR_HEADERS "./src/include/Emulator/*.h")
file(GLOB EMULATOR_SOURCES "./src/emulator/*.cpp")
file(GLOB_RECURSE QT_PROJECT_SOURCES "./src/gui/qt_tetroidnes/*.cpp")
file(GLOB_RECURSE QT_HEADERS "./src/include/Qt/*.h")
set(MAIN_FILE "./src/gui/qt_tetroidnes/Main.cpp")

find_package(OpenGL REQUIRED COMPONENTS OpenGL)

if(UNIX)
    find_package(SFML 3 REQUIRED COMPONENTS Graphics Audio)
endif()

find_package(Qt6 REQUIRED COMPONENTS
    ${QT_REQUIRED_COMPONENTS}
)


include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.x
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

#include_directories(${EXECUTABLE_NAME} PRIVATE src/include/Qt)
include_directories(${EXECUTABLE_NAME} PRIVATE src/include/)

qt_add_executable(${EXECUTABLE_NAME}
    MANUAL_FINALIZATION
    ${QT_RCC_FILES}
    ${QT_PROJECT_SOURCES}
    ${EMULATOR_SOURCES}
    ${QT_HEADERS}
)

qt_add_translations(${EXECUTABLE_NAME} TS_FILES ${QT_TRANSLATION_FILES})

if(ANDROID)
    set_property(TARGET ${EXECUTABLE_NAME} APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
        ${CMAKE_CURRENT_SOURCE_DIR}/android)
endif()

target_link_libraries(${EXECUTABLE_NAME} PRIVATE Qt6::Widgets Qt6::Gui Qt6::Core)
target_link_libraries(${EXECUTABLE_NAME} PRIVATE SFML::Graphics SFML::Window SFML::Audio)

#target_link_libraries(${EXECUTABLE_NAME} PRIVATE OpenGL::GLES2)

set_target_properties(${EXECUTABLE_NAME} PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS ${EXECUTABLE_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
install(CODE
    file(GET_RUNTIME_DEPENDENCIES DIRECTORY ${QT_REQUIRED_COMPONENTS})
)

# Update version macro if needed
file(READ ${MAIN_FILE} FILE_CONTENT)
string(REGEX MATCH "#define VERSION \"[^\"]*\"" FIRST_LINE "${FILE_CONTENT}")
#message(STATUS "First line: ${FIRST_LINE} Replacing to: ${VERSION_DEF}")
string(REPLACE "${FIRST_LINE}" "${VERSION_DEF}" NEW_CONTENT "${FILE_CONTENT}")
#message(STATUS "New Content:\n${NEW_CONTENT}")

if(NOT NEW_CONTENT STREQUAL FILE_CONTENT)
    file(WRITE ${MAIN_FILE}
        "${NEW_CONTENT}"
    )

else()
    message(STATUS "Version line not found or is the same")
endif()

qt_finalize_executable(${EXECUTABLE_NAME})