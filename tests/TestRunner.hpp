#ifndef SHIMERA_TESTRUNNER_H
#define SHIMERA_TESTRUNNER_H

#include "ITest.hpp"
#include <vector>

class TestRunner {
public:
    TestRunner() = default;
    ~TestRunner() = default;

    void add(ITest* test);
    void runAll();

    private:
    std::vector<ITest*> m_tests;
};

#endif //SHIMERA_TESTRUNNER_H