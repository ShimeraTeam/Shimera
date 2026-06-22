#include "TestRunner.hpp"

void TestRunner::add(ITest *test) {
    m_tests.push_back(test);
}

void TestRunner::runAll() {
    for (auto test : m_tests) {
        test->run();
    }
}