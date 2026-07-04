// SPDX-License-Identifier: GPL-3.0-only
//
// Shimera: a simple way to add visual effects without using any GPU knowledge
// Copyright (C) 2025-2026 The Shimera Authors
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, version 3 of the License.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#ifndef SHIMERA_UNIFORM_INL
#define SHIMERA_UNIFORM_INL

#include <GL/glew.h>
#include <string>
#include <glUtils.h>
#include <variant>
#include <stdexcept>

#include "Vec2.inl"
#include "Vec3.inl"
#include "Vec4.inl"
#include "Mat4.hpp"

namespace shimera {

using UniformValue = std::variant<float, int, Vec2<float>, Vec3<float>, Vec4<float>, Mat4>;

template <typename T>
class Uniform {
    public:
        Uniform(unsigned int shaderId, const std::string& name, T value)
            : m_name(name), m_value(value), m_location(-1) {
            GLC(m_location = glGetUniformLocation(shaderId, name.c_str()));
            if (m_location == -1) {
                throw std::runtime_error("Uniform '" + name + "' not found in shader program.");
            }
            setUniform(value);
        }

        Uniform& operator=(const T& newValue) {
            if (this->m_value == newValue)
                return *this;
            setUniform(newValue);
            this->m_value = newValue;
            return *this;
        }

        T operator+(const T& increment) {
            return this->m_value + increment;
        }

        Uniform& operator+=(const T& increment) {
            T newValue = *this + increment;
            setUniform(newValue);
            this->m_value = newValue;
            return *this;
        }

    private:
        std::string m_name;
        T m_value;
        int m_shaderId;
        int m_location;

        void setUniform(const T& newValue) {
            if constexpr (std::is_same_v<T, float>) {
                GLC(glUniform1f(m_location, newValue));
            } else if constexpr (std::is_same_v<T, int>) {
                GLC(glUniform1i(m_location, newValue));
            } else if constexpr (std::is_same_v<T, Vec2<float>>) {
                GLC(glUniform2f(m_location, newValue.x, newValue.y));
            } else if constexpr (std::is_same_v<T, Vec3<float>>) {
                GLC(glUniform3f(m_location, newValue.x, newValue.y, newValue.z));
            } else if constexpr (std::is_same_v<T, Vec4<float>>) {
                GLC(glUniform4f(m_location, newValue.x, newValue.y, newValue.z, newValue.w));
            } else if constexpr (std::is_same_v<T, Mat4>) {
                GLC(glUniformMatrix4fv(m_location, 1, GL_FALSE, newValue.m));
            }
            else {
                static_assert(sizeof(T) == 0, "Unsupported uniform type");
            }
        }
};

}


#endif //SHIMERA_UNIFORM_INL