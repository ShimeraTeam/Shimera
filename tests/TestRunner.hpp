#ifndef SHIMERA_TESTRUNNER_H
#define SHIMERA_TESTRUNNER_H

#include "ITest.hpp"
#include <vector>
#include <memory>

class TestRunner {
public:
    TestRunner() = default;
    ~TestRunner() = default;

    void add(std::unique_ptr<ITest> test);
    void runAll();

    private:
    std::vector<std::unique_ptr<ITest>> m_tests;
};

#endif //SHIMERA_TESTRUNNER_H