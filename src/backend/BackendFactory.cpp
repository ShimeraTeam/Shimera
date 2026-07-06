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

#include "backend/BackendFactory.hpp"

#ifdef SHIMERA_BACKEND_OPENGL
    #include "backend/opengl/OpenGLBackend.hpp"
#endif

#ifdef SHIMERA_BACKEND_SFML
    #include "backend/sfml/SFMLBackend.hpp"
#endif

#ifdef SHIMERA_BACKEND_RAYLIB
    #include "backend/raylib/RaylibBackend.hpp"
#endif

using shimera::BackendFactory;
using shimera::IBackend;

#ifdef SHIMERA_BACKEND_OPENGL
using shimera::OpenGLBackend;
#endif

#ifdef SHIMERA_BACKEND_SFML
using shimera::SFMLBackend;
#endif

#ifdef SHIMERA_BACKEND_RAYLIB
using shimera::RaylibBackend;
#endif

IBackend* BackendFactory::create() {
#ifdef SHIMERA_BACKEND_OPENGL
    return new OpenGLBackend();
#elifdef SHIMERA_BACKEND_SFML
    return new SFMLBackend();
#elifdef SHIMERA_BACKEND_SDL
    // return new SDLBackend();
    return nullptr;
#elifdef SHIMERA_BACKEND_RAYLIB
    return new RaylibBackend();
#else
    #error "No backend defined. Define SHIMERA_BACKEND_OPENGL, SHIMERA_BACKEND_SFML, or SHIMERA_BACKEND_SDL"
#endif
}
