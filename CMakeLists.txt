cmake_minimum_required(VERSION 3.16)

project(tetroidnes VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(EXECUTABLE_NAME qt_tetroidnes)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(WIN32)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
set(VERSION_DEF "#define VERSION \"${PROJECT_VERSION}-Debug\"")
set(DLL_DEPENDENCIES
    ${CMAKE_CURRENT_BINARY_DIR}/_deps/sfml-build/lib/sfml-graphics-d-2.dll
    ${CMAKE_CURRENT_BINARY_DIR}/_deps/sfml-build/lib/sfml-system-d-2.dll
    ${CMAKE_CURRENT_BINARY_DIR}/_deps/sfml-build/lib/sfml-window-d-2.dll
)
else()
set(VERSION_DEF "#define VERSION \"${PROJECT_VERSION}\"")
set(DLL_DEPENDENCIES
    ${CMAKE_CURRENT_BINARY_DIR}/_deps/sfml-build/lib/sfml-graphics-2.dll
    ${CMAKE_CURRENT_BINARY_DIR}/_deps/sfml-build/lib/sfml-system-2.dll
    ${CMAKE_CURRENT_BINARY_DIR}/_deps/sfml-build/lib/sfml-window-2.dll
)
endif()
endif()
set(QT_REQUIRED_COMPONENTS Widgets Core Gui LinguistTools)

set(QT_TRANSLATION_FILES
    src/gui/qt_tetroidnes/translations/tetroidnes_en_US.ts
)
set(EMULATOR_SOURCES 
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

set(QT_PROJECT_SOURCES
    src/gui/qt_tetroidnes/main.cpp
    src/gui/qt_tetroidnes/mainwindow.cpp
	src/gui/qt_tetroidnes/menubar.cpp
	src/gui/qt_tetroidnes/romlist.cpp
	src/gui/qt_tetroidnes/filtercontrolframe.cpp
	src/gui/qt_tetroidnes/flowlayout.cpp
	src/gui/qt_tetroidnes/gamedisplay.cpp

    
)

set(QT_RCC_FILES
    src/gui/qt_tetroidnes/resources.qrc
)

set(QT_HEADERS
    src/include/Qt/filtercontrolframe.h
    src/include/Qt/flowlayout.h
    src/include/Qt/gamedisplay.h
    src/include/Qt/mainwindow.h
    src/include/Qt/menubar.h
    src/include/Qt/romlist.h
)

set(QT_UI_FILES
    src/gui/qt_tetroidnes/mainwindow.ui
)

find_package(OpenGL REQUIRED COMPONENTS OpenGL)

find_package(Qt6 REQUIRED COMPONENTS
    ${QT_REQUIRED_COMPONENTS}
)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

include_directories(${EXECUTABLE_NAME} PRIVATE src/include/Qt)

qt_add_executable(${EXECUTABLE_NAME}
    MANUAL_FINALIZATION
    ${QT_RCC_FILES}
    ${QT_PROJECT_SOURCES}
    ${EMULATOR_SOURCES}
    ${QT_HEADERS}

    ${QT_UI_FILES}
)

qt_add_translations(${EXECUTABLE_NAME} TS_FILES ${QT_TRANSLATION_FILES})

if(ANDROID)
    set_property(TARGET ${EXECUTABLE_NAME} APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
        ${CMAKE_CURRENT_SOURCE_DIR}/android)
endif()

include_directories(${EXECUTABLE_NAME} PRIVATE src/include/)

target_link_libraries(${EXECUTABLE_NAME} PRIVATE Qt6::Widgets Qt6::Gui Qt6::Core)
target_link_libraries(${EXECUTABLE_NAME} PRIVATE sfml-system sfml-graphics sfml-window)

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
if(WIN32)
# Copy DLL Depencencies
add_custom_command(TARGET ${EXECUTABLE_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${DLL_DEPENDENCIES} $<TARGET_FILE_DIR:${EXECUTABLE_NAME}>
)
endif()

# Update version macro if needed
file(READ src/gui/qt_tetroidnes/main.cpp FILE_CONTENT)
string(REGEX MATCH "#define VERSION \"[^\"]*\"" FIRST_LINE "${FILE_CONTENT}")
#message(STATUS "First line: ${FIRST_LINE} Replacing to: ${VERSION_DEF}")
string(REPLACE "${FIRST_LINE}" "${VERSION_DEF}" NEW_CONTENT "${FILE_CONTENT}")
#message(STATUS "New Content:\n${NEW_CONTENT}")

if(NOT NEW_CONTENT STREQUAL FILE_CONTENT)
    file(WRITE src/gui/qt_tetroidnes/main.cpp
        "${NEW_CONTENT}"
    )
else()
    message(STATUS "Version line not found or is the same")
endif()
qt_finalize_executable(${EXECUTABLE_NAME})
