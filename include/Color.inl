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

#ifndef SHIMERA_COLOR_HPP
#define SHIMERA_COLOR_HPP

#include <shimera_api.h>
#include <cstdint>

#include "uniform/Vec3.inl"
#include "uniform/Vec4.inl"

/**
 * Backend "abstraction-friendly" color representation
 * Normalized RGBA values (0.0 to 1.0)
 */
namespace shimera {

struct SHIMERA_API Color {
    float m_r;
    float m_g;
    float m_b;
    float m_a;

    constexpr explicit Color(const float r = 0.0f, const float g = 0.0f, const float b = 0.0f, const float a = 1.0f)
        : m_r(r), m_g(g), m_b(b), m_a(a) {}

    [[nodiscard]] constexpr Vec4<unsigned char> toRGB() const {
        return {static_cast<unsigned char>(m_r * 255.0f), static_cast<unsigned char>(m_g * 255.0f),
            static_cast<unsigned char>(m_b * 255.0f), static_cast<unsigned char>(m_a * 255.0f)};
    }

    static constexpr Color fromRGB(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a = 255) {
        return Color(static_cast<float>(r) / 255.0f, static_cast<float>(g) / 255.0f, static_cast<float>(b) / 255.0f, static_cast<float>(a) / 255.0f);
    }

    static constexpr Color black()       { return Color(0.0f, 0.0f, 0.0f, 1.0f); }
    static constexpr Color white()       { return Color(1.0f, 1.0f, 1.0f, 1.0f); }
    static constexpr Color red()         { return Color(1.0f, 0.0f, 0.0f, 1.0f); }
    static constexpr Color green()       { return Color(0.0f, 1.0f, 0.0f, 1.0f); }
    static constexpr Color blue()        { return Color(0.0f, 0.0f, 1.0f, 1.0f); }
    static constexpr Color yellow()      { return Color(1.0f, 1.0f, 0.0f, 1.0f); }
    static constexpr Color cyan()        { return Color(0.0f, 1.0f, 1.0f, 1.0f); }
    static constexpr Color magenta()     { return Color(1.0f, 0.0f, 1.0f, 1.0f); }
    static constexpr Color transparent() { return Color(0.0f, 0.0f, 0.0f, 0.0f); }
};

}

#endif //SHIMERA_COLOR_HPP
