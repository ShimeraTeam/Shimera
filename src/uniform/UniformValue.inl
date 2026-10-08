// SPDX-License-Identifier: GPL-3.0-only
//
// Shimera: a simple way to add visual effects without any GPU knowledge
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

#pragma once

#include <variant>

#include "Vec2.inl"
#include "Vec3.inl"
#include "Vec4.inl"
#include "Mat4.hpp"

namespace shimera {

/* Anything an effect can hand to a shader.
 *
 * Split out of Uniform.inl so public headers can name it without dragging in GL. */
using UniformValue = std::variant<float, int, Vec2<float>, Vec3<float>, Vec4<float>, Mat4>;

}
