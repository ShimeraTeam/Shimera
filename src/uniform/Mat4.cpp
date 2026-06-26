#include "uniform/Mat4.hpp"

bool shimera::Mat4::operator==(const Mat4& o) const {
    for (int i = 0; i < 16; i++)
        if (m[i] != o.m[i])
            return false;
    return true;
}
