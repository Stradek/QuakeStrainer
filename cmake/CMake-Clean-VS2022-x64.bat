@echo off
setlocal

rem Copyright (c) 2024 Piotr Stradowski. All rights reserved.
rem Software distributed under the permissive MIT License.

set myPath=%~dp0

if [%sourceDir%] == [] set sourceDir="%myPath%\.."

cd %sourceDir%

echo ---- Cleaning CMake artifacts...

set "extensionsToDelete=.vcxproj;.filters;cmake_install.cmake;.sln"
set "filesToDelete=CMakeCache.txt"
set "directoriesToDelete=x64;x86;bin;CMakeFiles"

for %%i in (%extensionsToDelete%) do (
    for %%f in (%sourceDir%\*) do (
        if /i "%%~xf" == "%%~xi" (
            echo Deleting "%%~ff"
            del /q "%%~ff"
        )
    )
)

for %%f in (%filesToDelete%) do (
    echo Deleting "%%f"
    del /q "%sourceDir%\%%f"
)

for %%d in (%directoriesToDelete%) do (
    echo Deleting directory "%%d"
    if exist "%sourceDir%\%%d" rmdir /s /q "%sourceDir%\%%d"
)

for /d %%d in ("%sourceDir%\*.dir") do (
    echo Deleting directory "%%~nxd"
    rmdir /s /q "%%d"
)

pause
exit

:Error_CMakeDirectoryNotExist
echo "Could not find CMake directory."
pause
goto:eof