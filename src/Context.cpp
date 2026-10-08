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

#include "Context.hpp"

#include <GL/glew.h>

#include "Error.hpp"
#include "GL/GLError.hpp"

using shimera::Context;

namespace {

// loading GLEW's pointers twice is wasted work, so checking if they are already loaded.
bool g_functionsLoaded = false;

const char* glString(const GLenum name) {
    const auto* value = reinterpret_cast<const char*>(glGetString(name));
    return value != nullptr ? value : "unknown";
}

}

Context Context::create() {
    return {}; // braced initializer list
}

Context::Context() {
    /* glGetString is core GL 1.1 and resolves through the system library, so it answers
     * even before GLEW loads anything, which makes it the one reliable way to tell
     * "no current context" apart from "GL failed to load", and those need different advice. */
    if (glGetString(GL_VERSION) == nullptr) {
        throw InitError(
            "shimera::Context::create() must be called after your window exists and its "
            "OpenGL context is current.\n"
            "   SFML : after the sf::RenderWindow constructor\n"
            "   raylib : after InitWindow()\n"
            "   GLFW : after glfwMakeContextCurrent()");
    }

    if (!g_functionsLoaded) {
        glewExperimental = GL_TRUE;
        if (const GLenum error = glewInit(); error != GLEW_OK) {
            throw InitError(std::string("shimera: could not load OpenGL functions: ")
                + reinterpret_cast<const char*>(glewGetErrorString(error)));
        }
        g_functionsLoaded = true;
    }

    // glewExperimental provokes a GL_INVALID_ENUM on some drivers. Skip it so
    // the user's first GLC() reports their frame, not GLEW's startup.
    clearGLErrors();
}

Context::~Context() = default;
Context::Context(Context&&) noexcept = default;
Context& Context::operator=(Context&&) noexcept = default;

std::string Context::getRendererInfo() {
    return std::string("OpenGL ") + glString(GL_VERSION) + " on " + glString(GL_RENDERER);
}
