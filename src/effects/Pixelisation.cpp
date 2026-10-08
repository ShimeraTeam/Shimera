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

#include "Pixelisation.hpp"

#include "Resources.inl"

using shimera::Pixelisation;

Pixelisation::Pixelisation(const float pixelSizeX, const float pixelSizeY, const float gap,
                   const Vec2<float> resolution, const Vec2<float> offset)
    : m_pixelSizeX(pixelSizeX), m_pixelSizeY(pixelSizeY), m_resolution(resolution), m_offset(offset) {
    // The only diff between Slang shader and hand written shaders.
    m_processor = std::make_unique<GLPostProcessor>(generatedShader("postprocess.vert"),
                                                    generatedShader("pixelisation.frag"));
}

void Pixelisation::updateUniforms() {
    m_processor->setUniform("u_pixelSizeX", m_pixelSizeX);
    m_processor->setUniform("u_pixelSizeY", m_pixelSizeY);
    m_processor->setUniform("u_resolution", m_resolution);
    m_processor->setUniform("u_offset", m_offset);
}

Pixelisation& Pixelisation::withPixelSize(const float pixelSize) {
    m_pixelSizeX = pixelSize;
    m_pixelSizeY = pixelSize;
    return *this;
}

Pixelisation& Pixelisation::withPixelSizeX(const float pixelSizeX) {
    m_pixelSizeX = pixelSizeX;
    return *this;
}

Pixelisation& Pixelisation::withPixelSizeY(const float pixelSizeY) {
    m_pixelSizeY = pixelSizeY;
    return *this;
}

Pixelisation& Pixelisation::withResolution(const Vec2<float> resolution) {
    m_resolution = resolution;
    return *this;
}

Pixelisation& Pixelisation::withOffset(const Vec2<float> offset) {
    m_offset = offset;
    return *this;
}
