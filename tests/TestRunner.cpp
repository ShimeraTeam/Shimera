#include "TestRunner.hpp"

void TestRunner::add(std::unique_ptr<ITest> test) {
    m_tests.emplace_back(std::move(test));
}

void TestRunner::runAll() {
    for (auto &test : m_tests) {
        test->run();
    }
}