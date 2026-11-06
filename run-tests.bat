@echo off
Rem run-tests.bat

for /f %%A in ('echo prompt $E ^| cmd') do set "ESC=%%A"

set "status=0"

cmd /c "xmake f -m debug -c -y" || set "status=1"
cmd /c "xmake require -y" || set "status=1"
cmd /c "xmake" || set "status=1"

call :run_test "Build Shimera GLFW" "xmake build shimera-glfw" || set "status=1"
call :run_test "Build Shimera Raylib" "xmake build shimera-raylib" || set "status=1"
call :run_test "Build Shimera SFML" "xmake build shimera-sfml" || set "status=1"
call :run_test "Build Shimera SDL" "xmake build shimera-sdl" || set "status=1"
call :run_test "Build Glsl Shaders" "xmake build shaders" || EXIT /B 1
call :run_test "Build Glfw Resilience Tests" "xmake build glfw-resilience-tests" || set "status=1"
call :run_test "Build Raylib Resilience Tests" "xmake build raylib-resilience-tests" || set "status=1"
call :run_test "Build SFML Resilience Tests" "xmake build sfml-resilience-tests" || set "status=1"
call :run_test "Build SDL Resilience Tests" "xmake build sdl-resilience-tests" || set "status=1"

cd build/windows/x64/debug/ || EXIT /B 1

echo "=====Resilience Tests====="
cmd /c ".\glfw-resilience-tests" || set "status=1"
cmd /c ".\raylib-resilience-tests" || set "status=1"
cmd /c ".\sfml-resilience-tests" || set "status=1"
cmd /c ".\sdl-resilience-tests" || set "status=1"

EXIT /B %status%

:run_test
set name=%~1
set cmd=%~2
set filter=%~3

set "errorfile=%TEMP%\shimera_test_%RANDOM%.txt"
if "%filter%"=="" (
    cmd /c %cmd% > "%errorfile%" 2>&1
) else (
    cmd /c %cmd% > %errorfile% 2>&1
    findstr "%filter%" "%errorfile%"
)
set exit_code=%ERRORLEVEL%
if %exit_code% NEQ 0 (
    echo %name% ^> %ESC%[31mFAILED%ESC%[0m
    type "%errorfile%"
    del "%errorfile%"
    EXIT /B 1
) else (
    echo %name% ^> %ESC%[32mOK%ESC%[0m
    del "%errorfile%"
    EXIT /B 0
)