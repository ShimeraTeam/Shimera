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

#ifndef SHIMERA_IFRAMEBUFFER_HPP
#define SHIMERA_IFRAMEBUFFER_HPP

#include <shimera_api.h>
#include "ITexture.hpp"
#include "../Color.inl"

namespace shimera {

/**
 * Framebuffer interface
 * Defines methods for framebuffer operations across different backends
 */
class SHIMERA_API IFrameBuffer {
    public:
        virtual ~IFrameBuffer() = default;

        virtual void bind() = 0;
        virtual void unbind() = 0;
        virtual void clear(Color color) = 0;
        virtual ITexture& getTexture() = 0;
        virtual ITexture& getDepthTexture() = 0; // Only for 3D capable backends
        virtual void resize(int width, int height) = 0;
        [[nodiscard]] virtual int getWidth() const = 0;
        [[nodiscard]] virtual int getHeight() const = 0;
        
        /**
         * Get the native render target for backend-specific rendering
         * - SFML: sf::RenderTexture*
         * - OpenGL: nullptr (use bind/unbind instead)
         * - SDL: SDL_Texture*
         * The user has to cast the returned void* to the appropriate type (for now)
         */
        virtual void* getNativeRenderTarget() = 0;
};

}

#endif //SHIMERA_IFRAMEBUFFER_HPP