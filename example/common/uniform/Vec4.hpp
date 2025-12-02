#ifndef OPENGL_EXEMPLE_VEC4_H
#define OPENGL_EXEMPLE_VEC4_H

template <typename T>
class Vec4 {
    public:
        T x, y, z, w;
        Vec4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}

        bool operator==(const Vec4<T>& other) const {
            return x == other.x && y == other.y && z == other.z && w == other.w;
        }
};


#endif //OPENGL_EXEMPLE_VEC4_H