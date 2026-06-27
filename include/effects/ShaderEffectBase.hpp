#ifndef SHIMERA_SHADEREFFECTBASE_HPP
#define SHIMERA_SHADEREFFECTBASE_HPP

#include <shimera_api.h>
#include <memory>

#include "backend/IFrameBuffer.hpp"
#include "backend/IPostProcessor.hpp"
#include "backend/ITexture.hpp"

namespace shimera {

/**
 * Abstract class for shader effects
 * Provides common functionality for all shader effects
 */
class SHIMERA_API ShaderEffectBase {
    public:
        virtual ~ShaderEffectBase() = default;

        // shader name, used for debugging and identifying the effect in the pipeline
        [[nodiscard]] virtual std::string getName() const = 0;

        // screen render
        virtual void render(ITexture &texture) = 0;
        // offscreen (framebuffer) render
        virtual void render(ITexture &texture, IFrameBuffer &target) = 0;

        virtual void updateUniforms() = 0;

        [[nodiscard]] virtual bool isDepthNeeded() const;
        virtual void setDepthTexture(ITexture& depth);

        // restored, suppressed by the move declarations below
        ShaderEffectBase() = default;
        // only allow moving ownership
        ShaderEffectBase(ShaderEffectBase&&) = default;
        ShaderEffectBase& operator=(ShaderEffectBase&&) = default;
        // no copying (unique_ptr member), made explicit
        ShaderEffectBase(const ShaderEffectBase&) = delete;
        ShaderEffectBase& operator=(const ShaderEffectBase&) = delete;

        void setEnabled(bool enabled);
        [[nodiscard]] bool isEnabled() const;

        [[nodiscard]] IPostProcessor *getPostProcessor() const;

    protected:
        std::unique_ptr<IPostProcessor> m_processor;
        bool m_enabled = true;
};

}

#endif //SHIMERA_SHADEREFFECTBASE_HPP