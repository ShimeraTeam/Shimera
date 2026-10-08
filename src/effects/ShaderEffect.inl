#pragma once

#include "Effect.hpp"

namespace shimera {

/**
 * render(input, target) does not unbind the target afterwards (unlike the beta). The
 * pipeline decides what is bound next, so unbinding here would just be a guess that
 * the default framebuffer is 0.
 */
template<typename Derived>
class ShaderEffect : public Effect {
    public:
        void render(const GLTexture& input) override {
            if (!m_enabled)
                return;
            static_cast<Derived*>(this)->updateUniforms();
            m_processor->render(input);
        }

        void render(const GLTexture& input, GLFramebuffer& target) override {
            if (!m_enabled)
                return;
            static_cast<Derived*>(this)->updateUniforms();
            target.bind();
            m_processor->render(input);
        }

        Derived& with() { return *static_cast<Derived*>(this); }

    private:
        ShaderEffect() = default;
        friend Derived;
};

}
