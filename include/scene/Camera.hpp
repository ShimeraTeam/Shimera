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

#ifndef SHIMERA_CAMERA_HPP
#define SHIMERA_CAMERA_HPP

#include "shimera_api.h"
#include "uniform/Mat4.hpp"
#include "uniform/Vec3.inl"

namespace shimera {

struct SHIMERA_API Camera {
    Mat4 view;
    Mat4 projection;
    Vec3<float> position {0.0f, 0.0f, 0.0f};
};

}

#endif //SHIMERA_CAMERA_HPP
