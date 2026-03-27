#ifndef SHIMERA_BACKENDFACTORY_HPP
#define SHIMERA_BACKENDFACTORY_HPP

#include <shimera_api.h>
#include "IBackend.hpp"

namespace shimera {

class SHIMERA_API BackendFactory {
    public:
        static IBackend* create();
};

}

#endif //SHIMERA_BACKENDFACTORY_HPP