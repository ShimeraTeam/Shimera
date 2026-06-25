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

run_test "Configure and Build" "xmake f -y -c && xmake require && xmake"

run_test "Build Shimera Opengl" "xmake build shimera-opengl"
run_test "Build Shimera Raylib" "xmake build shimera-raylib"
run_test "Build Shimera SFML" "xmake build shimera-sfml"
run_test "Build Opengl Tests" "xmake build opengl-tests"
run_test "Build Raylib Tests" "xmake build raylib-tests"
run_test "Build SFML Tests" "xmake build sfml-tests"
run_test "Build Opengl Resilience Tests" "xmake build opengl-resilience-tests"
run_test "Build Raylib Resilience Tests" "xmake build raylib-resilience-tests"
run_test "Build SFML Resilience Tests" "xmake build sfml-resilience-tests"

if ! cd build/linux/x86_64/release/; then
  exit 1
fi

run_test "Run Opengl Tests" "LSAN_OPTIONS=suppressions=../../../../tests/memory/shimera_asan.supp ./opengl-tests"
run_test "Run Raylib Tests" "LSAN_OPTIONS=suppressions=../../../../tests/memory/shimera_asan.supp ./raylib-tests"
run_test "Run SFML Tests" "LSAN_OPTIONS=suppressions=../../../../tests/memory/shimera_asan.supp ./sfml-tests"
echo "=====Resilience Tests====="
./opengl-resilience-tests
./raylib-resilience-tests
./sfml-resilience-tests