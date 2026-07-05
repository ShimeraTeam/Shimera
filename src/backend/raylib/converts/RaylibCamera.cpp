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

#include "backend/raylib/converts/RaylibCamera.hpp"

#include "scene/CameraFactory.hpp"

shimera::Camera shimera::RaylibCamera::toShimera(const Camera3D& cam, float nearPlane, float farPlane, float aspect) {
    if (aspect <= 0.0f) {
        aspect = static_cast<float>(GetScreenWidth()) /
                 static_cast<float>(GetScreenHeight());
    }

    return CameraFactory::perspective(
        {cam.position.x, cam.position.y, cam.position.z},
        {cam.target.x, cam.target.y, cam.target.z},
        {cam.up.x, cam.up.y, cam.up.z},
        cam.fovy, aspect, nearPlane, farPlane);
}
