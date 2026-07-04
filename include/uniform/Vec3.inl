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

#ifndef SHIMERA_VEC3_INL
#define SHIMERA_VEC3_INL

namespace shimera {

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

}

#endif //SHIMERA_VEC3_INL
