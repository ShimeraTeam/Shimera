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

#include "converts/GlmConvert.hpp"

#include <cstring>
#include <glm/gtc/type_ptr.hpp>

namespace shimera {

Mat4 toShimMat4(const glm::mat4& mat) {
    Mat4 result{};
    std::memcpy(result.m, glm::value_ptr(mat), 16 * sizeof(float));
    return result;
}

glm::vec3 toGlmVec3(const Vec3<float>& v) {
    return {v.x, v.y, v.z};
}

}
