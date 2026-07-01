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

#ifndef OPENGL_EXEMPLE_VEC4_H
#define OPENGL_EXEMPLE_VEC4_H

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


#endif //OPENGL_EXEMPLE_VEC4_H