#!/bin/bash
# run-tests.sh

run_test() {
  name=$1
  cmd=$2
  pattern=$3

  errorfile=$(mktemp)
  eval "$cmd" >"$errorfile" 2>&1
  if [ $? -eq 0 ]; then
    echo -e "$name -> \033[32mOK\033[0m"
    [ -n "$pattern" ] && grep "$pattern" "$errorfile"
    rm -f "$errorfile"
    return 0
  else
    echo -e "$name -> \033[31mFAILED\033[0m"
    cat "$errorfile"
    rm -f "$errorfile"
    return 1
  fi
}

xmake f -c && xmake

run_test "Build Shimera Opengl" "xmake build shimera-opengl"
run_test "Build Shimera Raylib" "xmake build shimera-raylib"
run_test "Build Shimera SFML" "xmake build shimera-sfml"
run_test "Build Memory Test Opengl" "xmake build test-memory-opengl"
run_test "Build Memory Test Raylib" "xmake build test-memory-raylib"
run_test "Build Memory Test SFML" "xmake build test-memory-sfml"
run_test "Build Benchmark Opengl" "xmake build bench-opengl"
run_test "Build Benchmark Raylib" "xmake build bench-raylib"
run_test "Build Benchmark Sfml" "xmake build bench-sfml"

if ! cd build/linux/x86_64/release/; then
  exit 1
fi

run_test "Memory Opengl" "LSAN_OPTIONS=suppressions=../../../../tests/shimera_asan.supp ./test-memory-opengl"
run_test "Memory Raylib" "LSAN_OPTIONS=suppressions=../../../../tests/shimera_asan.supp ./test-memory-raylib"
run_test "Memory Sfml" "LSAN_OPTIONS=suppressions=../../../../tests/shimera_asan.supp ./test-memory-sfml"
run_test "Benchmark Opengl" "./bench-opengl" "BENCH"
run_test "Benchmark Raylib" "./bench-raylib" "BENCH"
run_test "Benchmark Sfml" "./bench-sfml" "BENCH"
