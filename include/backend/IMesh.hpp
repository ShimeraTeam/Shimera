#ifndef TEST_MEMORY_SFML_IMESH_HPP
#define TEST_MEMORY_SFML_IMESH_HPP

#include "shimera_api.h"

namespace shimera {

class SHIMERA_API IMesh {
    public:
        virtual ~IMesh() = default;

        virtual void draw() const = 0;

        [[nodiscard]] virtual unsigned int getIndexCount() const = 0;
};

}

#endif //TEST_MEMORY_SFML_IMESH_HPP
