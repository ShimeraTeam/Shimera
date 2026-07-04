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