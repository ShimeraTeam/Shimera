#include <GL/glew.h>
#include "../../memory/MemoryTest.hpp"
#include "../../TestRunner.hpp"
#include "BenchmarkSfml.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode({960, 540}), "Sfml shimera tests");
    window.setActive(true);
    glewInit();
    TestRunner runner;
    runner.add(new MemoryTest("Memory Tests"));
    runner.add(new BenchmarkSfml("Benchmark Sfml", window));
    runner.runAll();
    return 0;
}