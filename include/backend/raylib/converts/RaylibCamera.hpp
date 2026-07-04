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

#ifndef SHIMERA_RAYLIBCAMERA_HPP
#define SHIMERA_RAYLIBCAMERA_HPP

#include <raylib.h>

#include "shimera_api.h"
#include "scene/Camera.hpp"

namespace shimera {

SHIMERA_API class RaylibCamera {
    public:
        static Camera toShimera(const Camera3D& cam, float nearPlane = 0.01f,
                                float farPlane = 1000.0f, float aspect = -1.0f);
};

}
#endif //SHIMERA_RAYLIBCAMERA_HPP
