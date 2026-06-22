#include "raylib.h"
#include <iostream>
#include <GL/glew.h>
#include "../../memory/MemoryTest.hpp"
#include "../../TestRunner.hpp"
#include "BenchmarkRaylib.hpp"

int main() {
    InitWindow(960, 540, "Raylib shimera tests");
    glewInit();
    TestRunner runner;
    runner.add(new MemoryTest("Memory Tests"));
    runner.add(new BenchmarkRaylib("Benchmark Raylib"));
    runner.runAll();
    return 0;
}
