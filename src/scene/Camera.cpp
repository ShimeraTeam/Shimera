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

#include "Camera.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <algorithm>

using shimera::Camera;

namespace {

shimera::Mat4 toMat4(const glm::mat4& source) {
    shimera::Mat4 result{};
    const float* values = glm::value_ptr(source);
    std::copy_n(values, 16, result.m);
    return result;
}

glm::vec3 toGlm(const shimera::Vec3<float>& v) {
    return {v.x, v.y, v.z};
}

}

Camera Camera::fromMatrices(const Mat4& view, const Mat4& projection, const Vec3<float>& position) {
    Camera camera;
    camera.view = view;
    camera.projection = projection;
    camera.position = position;
    return camera;
}

Camera Camera::perspective(const Vec3<float>& position,
                           const Vec3<float>& target,
                           const Vec3<float>& up,
                           const float fovYDegrees,
                           const float aspect,
                           const float nearPlane,
                           const float farPlane) {
    Camera camera;
    camera.position = position;
    camera.view = toMat4(glm::lookAt(toGlm(position), toGlm(target), toGlm(up)));
    camera.projection = toMat4(glm::perspective(glm::radians(fovYDegrees),
        aspect > 0.0f ? aspect : 1.0f, nearPlane, farPlane));
    return camera;
}
