#ifndef SHIMERA_SHADEREFFECTBASE_HPP
#define SHIMERA_SHADEREFFECTBASE_HPP

#include <shimera_api.h>
#include <memory>

#include "backend/IFrameBuffer.hpp"
#include "backend/IPostProccessor.hpp"
#include "backend/ITexture.hpp"

/**
 * Abstract class for shader effects
 * Provides common functionality for all shader effects
 */
class SHIMERA_API ShaderEffectBase {
    public:
        virtual ~ShaderEffectBase() = default;

        // screen render
        virtual void render(ITexture &texture) = 0;
        // offscreen (framebuffer) render
        virtual void render(ITexture &texture, IFrameBuffer &target) = 0;

        virtual void updateUniforms() = 0;

        void setEnabled(bool enabled);
        bool isEnabled() const;

        IPostProccessor *getPostProcessor() const;

    protected:
        std::unique_ptr<IPostProccessor> m_processor;
        bool m_enabled = true;
};

#endif //SHIMERA_SHADEREFFECTBASE_HPP