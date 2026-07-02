#ifndef SHIMERA_MAT4_HPP
#define SHIMERA_MAT4_HPP

namespace shimera {

struct Mat4 {
    float m[16];
    bool operator==(const Mat4& o) const;
};

}

#endif //SHIMERA_MAT4_HPP
