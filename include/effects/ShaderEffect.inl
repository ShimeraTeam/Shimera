#ifndef SHIMERA_SFML_SHADEREFFECT_INL
#define SHIMERA_SFML_SHADEREFFECT_INL
#include "ShaderEffectBase.hpp"

/*
 * Templated shader effect class
 * Inherits from ShaderEffectBase and provides a convenient interface for creating specific shader effects
 * The Derived template parameter allows for type-safe access to the derived class's methods and members
 */
template<typename Derived>
class ShaderEffect : public ShaderEffectBase {
    public:
        void render(ITexture& texture) override {
            if (!m_enabled) return;
            static_cast<Derived *>(this)->updateUniforms();
            m_processor->render(texture);
        }

        void render(ITexture& texture, IFrameBuffer& target) override {
            if (!m_enabled) return;
            static_cast<Derived *>(this)->updateUniforms();
            target.bind();
            m_processor->render(texture);
            target.unbind();
        }

        Derived &with() {
            return *static_cast<Derived *>(this);
        }

    protected:
        ShaderEffect() = default;
};

#endif //SHIMERA_SFML_SHADEREFFECT_INL