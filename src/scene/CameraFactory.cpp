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

#include "scene/CameraFactory.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include "converts/GlmConvert.hpp"

shimera::Camera shimera::CameraFactory::perspective(const Vec3<float>& position, const Vec3<float>& target,
                                                    const Vec3<float>& up, float fovYDegrees, float aspect, float nearPlane, float farPlane) {
    const glm::mat4 view = glm::lookAt(toGlmVec3(position), toGlmVec3(target), toGlmVec3(up));
    const glm::mat4 proj = glm::perspective(glm::radians(fovYDegrees), aspect, nearPlane, farPlane);

    Camera cam;
    cam.view = toShimMat4(view);
    cam.projection = toShimMat4(proj);
    cam.position = position;
    return cam;
}
