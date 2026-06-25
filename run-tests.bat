@echo off
Rem run-tests.bat

for /f %%A in ('echo prompt $E ^| cmd') do set "ESC=%%A"

cmd /c "xmake f -m debug -y"
cmd /c "xmake"

call :run_test "Build Shimera Opengl" "xmake build shimera-opengl"
call :run_test "Build Shimera Raylib" "xmake build shimera-raylib"
call :run_test "Build Shimera SFML" "xmake build shimera-sfml"
call :run_test "Build Opengl Tests" "xmake build opengl-tests"
call :run_test "Build Raylib Tests" "xmake build raylib-tests"
call :run_test "Build SFML Tests" "xmake build sfml-tests"
call :run_test "Build Opengl Resilience Tests" "xmake build opengl-resilience-tests"
call :run_test "Build Raylib Resilience Tests" "xmake build raylib-resilience-tests"
call :run_test "Build SFML Resilience Tests" "xmake build sfml-resilience-tests"

cd build/windows/x64/debug/

call :run_test "Run Opengl Tests" ".\opengl-tests"
call :run_test "Run Raylib Tests" ".\raylib-tests"
call :run_test "Run Sfml Tests" ".\sfml-tests"

echo "=====Resilience Tests====="
cmd /c ".\opengl-resilience-tests"
cmd /c ".\raylib-resilience-tests"
cmd /c ".\sfml-resilience-tests"

EXIT /B 0

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