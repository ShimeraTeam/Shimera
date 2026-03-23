#ifndef SHIMERA_VEC3_HPP
#define SHIMERA_VEC3_HPP

template <typename T>
class Vec3 {
    public:
        T x, y, z;
        Vec3(T x, T y, T z) : x(x), y(y), z(z) {}
        explicit Vec3(T v) : x(v), y(v), z(v) {}

        bool operator==(const Vec3<T>& other) const {
            return x == other.x && y == other.y && z == other.z;
        }
};

#endif //SHIMERA_VEC3_HPP
