#ifndef SHIMERA_BACKENDFACTORY_HPP
#define SHIMERA_BACKENDFACTORY_HPP

#include "IBackend.hpp"

class BackendFactory {
    public:
        static IBackend* create();
};

#endif //SHIMERA_BACKENDFACTORY_HPP