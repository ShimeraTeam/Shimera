#ifndef TEST_MEMORY_SFML_MAT4_INL
#define TEST_MEMORY_SFML_MAT4_INL

namespace shimera {

struct Mat4 {
    float m[16];
    bool operator==(const Mat4& o) const;
};

}

#endif //TEST_MEMORY_SFML_MAT4_INL
