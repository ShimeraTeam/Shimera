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

#include "uniform/Mat4.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <algorithm>
#include <cmath>

using shimera::Mat4;

namespace {

glm::mat4 toGlm(const Mat4& source) {
    return glm::make_mat4(source.m);
}

Mat4 fromGlm(const glm::mat4& source) {
    Mat4 result{};
    std::copy_n(glm::value_ptr(source), 16, result.m);
    return result;
}

}

bool Mat4::operator==(const Mat4& o) const {
    for (int i = 0; i < 16; i++)
        if (m[i] != o.m[i])
            return false;
    return true;
}

Mat4 Mat4::identity() {
    return fromGlm(glm::mat4{1.0f});
}

Mat4 Mat4::transposed() const {
    Mat4 result{};
    for (int c = 0; c < 4; ++c)
        for (int r = 0; r < 4; ++r)
            result.m[r * 4 + c] = m[c * 4 + r];
    return result;
}

Mat4 Mat4::inverse() const {
    const glm::mat4 source = toGlm(*this);
    const float det = glm::determinant(source);
    if (det == 0.0f || !std::isfinite(det))
        return identity();
    return fromGlm(glm::inverse(source));
}

Mat4 Mat4::operator*(const Mat4& o) const {
    return fromGlm(toGlm(*this) * toGlm(o));
}
