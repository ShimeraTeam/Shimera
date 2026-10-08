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
 * A target independent world position information.
 */
struct Transform {
    Vec3<float> position{0.0f, 0.0f, 0.0f};
    Vec3<float> rotationEuler{0.0f, 0.0f, 0.0f}; // degrees, applied Z then Y then X
    Vec3<float> scale{1.0f, 1.0f, 1.0f};

    [[nodiscard]] Mat4 toMatrix() const;
};

}
