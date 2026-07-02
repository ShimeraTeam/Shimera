#ifndef SHIMERA_VEC2_INL
#define SHIMERA_VEC2_INL

namespace shimera {

template<typename T>
class Vec2 {
    public:
        T x, y;
        Vec2(T x, T y) : x(x), y(y) {}

        bool operator==(const Vec2<T>& other) const {
            return x == other.x && y == other.y;
        }
};

}

#endif //SHIMERA_VEC2_INL