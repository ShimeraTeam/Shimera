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

#ifndef SHIMERA_GLMCONVERT_HPP
#define SHIMERA_GLMCONVERT_HPP

#include <glm/glm.hpp>

#include "shimera_api.h"
#include "uniform/Mat4.hpp"
#include "uniform/Vec3.inl"

namespace shimera {

SHIMERA_API Mat4 toShimMat4(const glm::mat4& mat);

SHIMERA_API glm::vec3 toGlmVec3(const Vec3<float>& v);

}

#endif //SHIMERA_GLMCONVERT_HPP
