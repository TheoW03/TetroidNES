@echo off

set QT_WIN_DEPLOY_PATH="%QT_PATH%\bin\windeployqt6.exe"

cmake -G "MinGW Makefiles" -S . -B build
cmake --build build

::IF %ERRORLEVEL% GEQ 1 {
::    echo Build Failed! Error Level: %ERRORLEVEL%
::    exit
::}

cd build

IF EXIST "%QT_WIN_DEPLOY_PATH%" (
    %QT_WIN_DEPLOY_PATH% TetroidNES.exe
) ELSE (
    echo WARNING: CANNOT FIND 'windeployqt6.exe'
    echo IN: %QT_WIN_DEPLOY_PATH%
    echo Without this path, TetroidNES may build correctly but without the required DLL files crashing on startup
    echo Set env variable QT_PATH to the correct path
    echo Example: 'path-to-qt-root\\Qt\\6.9.1\\mingw_64'
)

TetroidNES.exe