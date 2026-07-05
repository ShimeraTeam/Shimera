#ifndef SHIMERA_MEMORYTEST_HPP
#define SHIMERA_MEMORYTEST_HPP

#include "../ITest.hpp"

class MemoryTest: public ITest {
    public:
        MemoryTest(const std::string &testName);
        ~MemoryTest() override = default;

        std::string getName() const override;
        void run() override;
    
    private:
        std::string m_name;
};

#endif //SHIMERA_MEMORYTEST_HPP