#ifndef TEST_MEMORY_SFML_RAYLIBCONVERT_HPP
#define TEST_MEMORY_SFML_RAYLIBCONVERT_HPP

#include <raylib.h>

#include "shimera_api.h"
#include "Color.inl"
#include "uniform/Mat4.hpp"

namespace shimera {
    SHIMERA_API Matrix toRaylibMatrix(const Mat4& s);
    SHIMERA_API ::Color toRaylibColor(const shimera::Color& c);
}

#endif //TEST_MEMORY_SFML_RAYLIBCONVERT_HPP
