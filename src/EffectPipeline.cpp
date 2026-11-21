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

#include "EffectPipeline.hpp"

#include <GL/glew.h>
#include "GL/GLError.hpp"
#include "GL/GLStateGuard.hpp"

using shimera::EffectPipeline;
using shimera::Effect;
using shimera::GLFramebuffer;
using shimera::GLTexture;

EffectPipeline::EffectPipeline(const int width, const int height, const bool hdr)
    : m_width(width), m_height(height), m_hdr(hdr) {}

void EffectPipeline::build() {
    if (m_built)
        return;
    const auto format = m_hdr ? GLTexture::Format::ColorHDR : GLTexture::Format::Color;
    // Intermediates only ever receive fullscreen passes, so they carry no depth.
    m_fboA = std::make_unique<GLFramebuffer>(m_width, m_height, GLFramebuffer::Depth::None, format);
    m_fboB = std::make_unique<GLFramebuffer>(m_width, m_height, GLFramebuffer::Depth::None, format);
    m_built = true;
}

void EffectPipeline::setHdr(const bool hdr) {
    if (hdr == m_hdr)
        return;
    m_hdr = hdr;
    // The intermediates change format, so let the next render rebuild them.
    m_fboA.reset();
    m_fboB.reset();
    m_built = false;
    for (const auto& fx : m_effects)
        fx->setPipelineHdr(hdr);
}

shimera::Tonemap& EffectPipeline::tonemap() {
    if (!m_tonemap) {
        m_tonemap = std::make_unique<Tonemap>();
        m_tonemap->resize(m_width, m_height);
    }
    return *m_tonemap;
}

void EffectPipeline::resize(const int width, const int height) {
    m_width = width;
    m_height = height;
    if (m_fboA) m_fboA->resize(width, height);
    if (m_fboB) m_fboB->resize(width, height);
    if (m_tonemap) m_tonemap->resize(width, height);
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

    // An HDR chain ends in 8 bits whatever happens, so the display mapping is ours to do.
    if (m_hdr && m_autoTonemap)
        active.push_back(&tonemap());

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
