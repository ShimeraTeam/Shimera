#pragma once

#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

#include "GL/GLFramebuffer.hpp"
#include "GL/GLTexture.hpp"
#include "effects/Effect.hpp"
#include "hosts/IHostTarget.hpp"

namespace shimera {

class EffectPipeline {
    public:
        EffectPipeline(int width, int height);

        EffectPipeline(const EffectPipeline&) = delete;
        EffectPipeline& operator=(const EffectPipeline&) = delete;
        EffectPipeline(EffectPipeline&&) = default;
        EffectPipeline& operator=(EffectPipeline&&) = default;

        template<typename TEffect, typename... Args>
        EffectPipeline& add(Args&&... args) {
            static_assert(std::is_base_of_v<Effect, TEffect>,
                "EffectPipeline::add<T> requires T to derive from shimera::Effect");
            auto effect = std::make_unique<TEffect>(std::forward<Args>(args)...);
            effect->resize(m_width, m_height);
            m_effects.emplace_back(std::move(effect));
            return *this;
        }

        // Effects are addressed by type, index disambiguate repeats of one type.
        template<typename TEffect>
        TEffect& get(const std::size_t index = 0) {
            std::size_t count = 0;
            for (auto& fx : m_effects) {
                if (auto* effect = dynamic_cast<TEffect*>(fx.get())) {
                    if (count == index)
                        return *effect;
                    ++count;
                }
            }
            throw std::out_of_range("shimera: no such effect in the pipeline at index "
                + std::to_string(index));
        }

        // Renders into whatever framebuffer is bound when called.
        void render(const GLTexture& input, const GLTexture* depth = nullptr);
        // Renders into `target`.
        void render(const GLTexture& input, GLFramebuffer& target, const GLTexture* depth = nullptr);

        /* Takes the scene straight from a host target, depth included when the host has it.
         * This is the call users actually write. IHostTarget names no host library, so
         * the core stays host-free. */
        void render(IHostTarget& scene);
        void render(IHostTarget& scene, GLFramebuffer& target);

        void resize(int width, int height);

        [[nodiscard]] std::size_t size() const { return m_effects.size(); }
        [[nodiscard]] std::string effectNames() const;
        [[nodiscard]] int getWidth() const { return m_width; }
        [[nodiscard]] int getHeight() const { return m_height; }

    private:
        void build();
        void renderImpl(const GLTexture& input, uint32_t target);

        std::vector<std::unique_ptr<Effect>> m_effects;
        std::unique_ptr<GLFramebuffer> m_fboA;
        std::unique_ptr<GLFramebuffer> m_fboB;
        const GLTexture* m_depth = nullptr;
        int m_width;
        int m_height;
        bool m_built = false;
};

}
