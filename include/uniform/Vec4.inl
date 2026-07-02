#ifndef SHIMERA_VEC4_INL
#define SHIMERA_VEC4_INL

namespace shimera {

template <typename T>
class Vec4 {
    public:
        T x, y, z, w;
        Vec4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}

        bool operator==(const Vec4<T>& other) const {
            return x == other.x && y == other.y && z == other.z && w == other.w;
        }
};

}


#endif //SHIMERA_VEC4_INL