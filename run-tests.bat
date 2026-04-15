@echo off
Rem run-tests.bat

for /f %%A in ('echo prompt $E ^| cmd') do set "ESC=%%A"

cmd /c "xmake f -m debug -c"
cmd /c "xmake"

call :run_test "Build Shimera Opengl" "xmake build shimera-opengl"
call :run_test "Build Shimera Raylib" "xmake build shimera-raylib"
call :run_test "Build Shimera SFML" "xmake build shimera-sfml"
call :run_test "Build Memory Test Opengl" "xmake build test-memory-opengl"
call :run_test "Build Memory Test Raylib" "xmake build test-memory-raylib"
call :run_test "Build Memory Test SFML" "xmake build test-memory-sfml"
call :run_test "Build Benchmark Opengl" "xmake build bench-opengl"
call :run_test "Build Benchmark Raylib" "xmake build bench-raylib"
call :run_test "Build Benchmark Sfml" "xmake build bench-sfml"

cd build/windows/x64/debug/

call :run_test "Memory Opengl" ".\test-memory-opengl"
call :run_test "Memory Raylib" ".\test-memory-raylib"
call :run_test "Memory Sfml" ".\test-memory-sfml"
call :run_test "Benchmark Opengl" ".\bench-opengl" "BENCH"
call :run_test "Benchmark Raylib" ".\bench-raylib" "BENCH"
call :run_test "Benchmark Sfml" ".\bench-sfml" "BENCH"

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