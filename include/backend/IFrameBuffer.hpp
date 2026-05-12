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
        virtual void clear(shimera::Color color) = 0;
        virtual ITexture& getTexture() = 0;
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