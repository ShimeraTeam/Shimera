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

status=0

run_test "Configure and Build" "xmake f -y -c && xmake require -y && xmake" || exit 1

run_test "Build Shimera GLFW" "xmake build shimera-glfw" || status=1
run_test "Build Shimera Raylib" "xmake build shimera-raylib" || status=1
run_test "Build Shimera SFML" "xmake build shimera-sfml" || status=1
run_test "Build Shimera SDL" "xmake build shimera-sdl" || status=1
run_test "Build Glsl Shaders" "xmake build shaders" || exit 1
run_test "Build Glfw Resilience Tests" "xmake build glfw-resilience-tests" || status=1
run_test "Build Raylib Resilience Tests" "xmake build raylib-resilience-tests" || status=1
run_test "Build SFML Resilience Tests" "xmake build sfml-resilience-tests" || status=1
run_test "Build SDL Resilience Tests" "xmake build sdl-resilience-tests" || status=1

if ! cd build/linux/x86_64/release/; then
  exit 1
fi

echo "=====Resilience Tests====="
./glfw-resilience-tests || status=1
./raylib-resilience-tests || status=1
./sfml-resilience-tests || status=1
./sdl-resilience-tests || status=1

exit $status
