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

#include "GlfwTarget.hpp"

#include <GL/glew.h>

#include "GL/GLError.hpp"

using shimera::GlfwTarget;

GlfwTarget::GlfwTarget(const int width, const int height, const bool samplableDepth)
    : m_framebuffer(width, height, samplableDepth) {}

void GlfwTarget::begin() {
    m_previousFramebuffer = currentDrawFramebuffer();
    GLC(glGetIntegerv(GL_VIEWPORT, m_previousViewport));

    m_framebuffer.bind();
    GLC(glViewport(0, 0, m_framebuffer.width(), m_framebuffer.height()));
}

void GlfwTarget::end() {
    bindFramebuffer(m_previousFramebuffer);
    GLC(glViewport(m_previousViewport[0], m_previousViewport[1],
                   m_previousViewport[2], m_previousViewport[3]));
}

void GlfwTarget::resize(const int width, const int height) {
    m_framebuffer.resize(width, height);
}
