#ifndef OPENGL_LEARNING_UNIFORM_H
#define OPENGL_LEARNING_UNIFORM_H
#include <string>

#include <glUtils.h>
#include <uniform/Vec4.hpp>

template <typename T>
class Uniform {
    public:
        Uniform(unsigned int shaderId, std::string name, T value)
            : name(name), value(value), location(-1) {
            GLC(location = glGetUniformLocation(shaderId, name.c_str()));
            if (location == -1) {
                throw std::runtime_error("Uniform '" + name + "' not found in shader program.");
            }
            setUniform(value);
        }

        Uniform& operator=(const T& newValue) {
            if (this->value == newValue)
                return *this;
            setUniform(newValue);
            this->value = newValue;
            return *this;
        }

        T operator+(const T& increment) {
            return this->value + increment;
        }

        Uniform& operator+=(const T& increment) {
            T newValue = *this + increment;
            setUniform(newValue);
            this->value = newValue;
            return *this;
        }

    private:
        std::string name;
        T value;
        int shaderId;
        int location;

        void setUniform(const T& newValue) {
            if constexpr (std::is_same_v<T, float>) {
                GLC(glUniform1f(location, newValue));
            } else if constexpr (std::is_same_v<T, int>) {
                GLC(glUniform1i(location, newValue));
            } else if constexpr (std::is_same_v<T, Vec4<float>>) {
                GLC(glUniform4f(location, newValue.x, newValue.y, newValue.z, newValue.w));
            } else {
                static_assert(sizeof(T) == 0, "Unsupported uniform type");
            }
        }
};


#endif //OPENGL_LEARNING_UNIFORM_H