#include "EffectPipeline.hpp"

#include <GL/glew.h>
#include "GL/GLError.hpp"
#include "GL/GLStateGuard.hpp"

using shimera::EffectPipeline;
using shimera::Effect;
using shimera::GLFramebuffer;
using shimera::GLTexture;

EffectPipeline::EffectPipeline(const int width, const int height)
    : m_width(width), m_height(height) {}

void EffectPipeline::build() {
    if (m_built)
        return;
    m_fboA = std::make_unique<GLFramebuffer>(m_width, m_height);
    m_fboB = std::make_unique<GLFramebuffer>(m_width, m_height);
    m_built = true;
}

void EffectPipeline::resize(const int width, const int height) {
    m_width = width;
    m_height = height;
    if (m_fboA) m_fboA->resize(width, height);
    if (m_fboB) m_fboB->resize(width, height);
    for (const auto& fx : m_effects)
        fx->resize(width, height);
}

void EffectPipeline::render(const GLTexture& input, const GLTexture* depth) {
    if (depth != nullptr)
        m_depth = depth;
    /* Whatever the caller had bound is where the finished frame belongs.
     * Under a host, 0 is usually the wrong answer. */
    renderImpl(input, currentDrawFramebuffer());
}

void EffectPipeline::render(const GLTexture& input, GLFramebuffer& target, const GLTexture* depth) {
    if (depth != nullptr)
        m_depth = depth;
    renderImpl(input, target.handle());
}

void EffectPipeline::render(IHostTarget& scene) {
    render(scene.getColor(), scene.getDepth());
}

void EffectPipeline::render(IHostTarget& scene, GLFramebuffer& target) {
    render(scene.getColor(), target, scene.getDepth());
}

void EffectPipeline::renderImpl(const GLTexture& input, const uint32_t target) {
    if (!m_built)
        build();

    std::vector<Effect*> active;
    active.reserve(m_effects.size());
    for (const auto& fx : m_effects) {
        if (fx->isEnabled())
            active.push_back(fx.get());
    }
    if (active.empty())
        return;

    for (auto* fx : active) {
        if (fx->needsDepth()) {
            if (m_depth == nullptr)
                throw std::runtime_error("shimera: effect '" + fx->getName()
                    + "' needs the scene depth. Pass it: pipeline.render(colour, &depth)");
            fx->setDepthTexture(*m_depth);
        }
    }

    /* One capture/restore for the whole chain, not one per effect. RAII means it also runs
     * if an effect throws. Everything Shimera touches below is put back by this. */
    const GLStateGuard guard;

    GLC(glViewport(0, 0, m_width, m_height));

    if (active.size() == 1) {
        bindFramebuffer(target);
        active[0]->render(input);
    } else {
        const GLTexture* current = &input;
        GLFramebuffer* write = m_fboA.get();
        GLFramebuffer* read = m_fboB.get();

        for (std::size_t i = 0; i + 1 < active.size(); ++i) {
            active[i]->render(*current, *write);
            current = &write->color();
            std::swap(write, read);
        }

        bindFramebuffer(target);
        active.back()->render(*current);
    }
}

std::string EffectPipeline::effectNames() const {
    if (m_effects.empty())
        return "no_effects";
    std::string names;
    for (const auto& fx : m_effects) {
        if (!names.empty())
            names += ", ";
        names += fx->getName();
    }
    return names;
}
