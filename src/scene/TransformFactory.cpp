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

#include "scene/TransformFactory.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include "converts/GlmConvert.hpp"


shimera::Mat4 shimera::TransformFactory::model(const Vec3<float>& position, const Vec3<float>& rotationEuler,
    const Vec3<float>& scale) {
    glm::mat4 m(1.0f);
    m = glm::translate(m, toGlmVec3(position));
    m = glm::rotate(m, glm::radians(rotationEuler.z), {0.0f, 0.0f, 1.0f});
    m = glm::rotate(m, glm::radians(rotationEuler.y), {0.0f, 1.0f, 0.0f});
    m = glm::rotate(m, glm::radians(rotationEuler.x), {1.0f, 0.0f, 0.0f});
    m = glm::scale(m, toGlmVec3(scale));
    return toShimMat4(m);
}
