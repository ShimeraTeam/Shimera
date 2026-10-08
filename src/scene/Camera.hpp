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

#include "uniform/Mat4.hpp"
#include "uniform/Vec3.inl"

namespace shimera {

/**
 * The two matrices a material needs, plus the eye position the Fresnel term needs.
 *
 * The beta split this across Camera plus a separate CameraFactory class. One type with a
 * named constructor is fewer things to explain and impossible to half-initialize.
 *
 * glm does the matrix maths, but only inside Camera.cpp, users never see a glm type,
 * which keeps them free to use their own maths library without a conversion layer.
 */
struct Camera {
    Mat4 view{};
    Mat4 projection{};
    Vec3<float> position{0.0f, 0.0f, 0.0f};

    /** Adopt the host's own matrices instead of computing our own.
     *
     * This allows to draw object inside a host lib's scene and also the ability
     * to use the host's depth buffer. */
    static Camera fromMatrices(const Mat4& view, const Mat4& projection, const Vec3<float>& position);

    // Builds its own matrices. Fine when nothing else draws 3D into the same target.
    static Camera perspective(const Vec3<float>& position,
                              const Vec3<float>& target,
                              const Vec3<float>& up,
                              float fovYDegrees,
                              float aspect,
                              float nearPlane = 0.1f,
                              float farPlane = 100.0f);
};

}
