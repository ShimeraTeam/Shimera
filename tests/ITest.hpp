#ifndef SHIMERA_ITEST_HPP
#define SHIMERA_ITEST_HPP

#include <string>

class ITest {
    public:
        virtual std::string getName() const = 0;
        virtual void run() = 0;
        virtual ~ITest() = default;
};

#endif SHIMERA_ITEST_HPP