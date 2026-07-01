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

#ifndef SHIMERA_SFML_EFFECTPIPELINE_HPP
#define SHIMERA_SFML_EFFECTPIPELINE_HPP
#include <vector>

#include "backend/IBackend.hpp"
#include "effects/ShaderEffectBase.hpp"

namespace shimera
{

class SHIMERA_API EffectPipeline
{
    public:
        EffectPipeline(IBackend* backend, const unsigned int width, const unsigned int height)
            : m_backend(backend), m_width(width), m_height(height) {}
        ~EffectPipeline() = default;

        template<typename TEffect>
        EffectPipeline &addEffect(TEffect &&effect) {
            static_assert(std::is_base_of_v<ShaderEffectBase, TEffect>);
            m_effects.emplace_back(std::make_unique<TEffect>(std::forward<TEffect>(effect)));
            return *this;
        }

        template<typename TEffect, typename ...Args>
        EffectPipeline &addEffect(Args &&...args) {
            static_assert(std::is_base_of_v<ShaderEffectBase, TEffect>);
            m_effects.emplace_back(
                std::make_unique<TEffect>(m_backend, std::forward<Args>(args)...));
            return *this;
        }

        // Fow now, can only get by type, making stacking same type of shaders difficult.
        template<typename TEffect>
        TEffect &get(const std::size_t index = 0) {
            std::size_t count = 0;
            for (auto &fx : m_effects) {
                if (auto *effect = dynamic_cast<TEffect *>(fx.get())) {
                    if (count == index)
                        return *effect;
                    ++count;
                }
            }
            throw std::out_of_range("Effect requested was not found at index " + std::to_string(index));
        }

        void build() {
            if (m_built)
                return;

            // TODO: Remove the `static_cast` when `IBackend`'s parameters will be changed to unsigned int (issue #184)
            m_fboA.reset(m_backend->createFrameBuffer(static_cast<int>(m_width), static_cast<int>(m_height)));
            m_fboB.reset(m_backend->createFrameBuffer(static_cast<int>(m_width), static_cast<int>(m_height)));
            m_built = true;
        }

        void render(ITexture &input, ITexture *depth = nullptr) {
            if (depth != nullptr)
                m_depthTexture = depth;
            renderImpl(input, nullptr);
        }
        void render(ITexture &input, IFrameBuffer &tgt, ITexture *depth = nullptr) {
            if (depth != nullptr)
                m_depthTexture = depth;
            renderImpl(input, &tgt);
        }

        void resize(const int width, const int height) {
            m_width = width;
            m_height = height;
            if (m_fboA)
                m_fboA->resize(width, height);
            if (m_fboB)
                m_fboB->resize(width, height);
        }

        [[nodiscard]] std::size_t size() const {
            return m_effects.size();
        }

    private:
        IBackend *m_backend;
        std::vector<std::unique_ptr<ShaderEffectBase>> m_effects;
        std::unique_ptr<IFrameBuffer> m_fboA;
        std::unique_ptr<IFrameBuffer> m_fboB;
        unsigned int m_width, m_height = 0;
        bool m_built = false;
        ITexture *m_depthTexture = nullptr;

        void renderImpl(ITexture &input, IFrameBuffer *target = nullptr) {
            if (!m_built)
                build();

            std::vector<ShaderEffectBase*> active;
            active.reserve(m_effects.size());
            for (auto &fx : m_effects) {
                if (fx->isEnabled())
                    active.push_back(fx.get());
            }

            if (active.empty())
                return;

            for (auto* fx : active) {
                if (fx->isDepthNeeded()) {
                    if (m_depthTexture == nullptr)
                        throw std::runtime_error("An effect requires scene depth. Call render(input, ..., sceneDepth)");
                    fx->setDepthTexture(*m_depthTexture);
                }
            }

            if (active.size() == 1) {
                if (target != nullptr)
                    active[0]->render(input, *target);
                else
                    active[0]->render(input);
                return;
            }

            ITexture *currentTexture = &input;
            IFrameBuffer *writeFb = m_fboA.get();
            IFrameBuffer *readFb = m_fboB.get();

            for (size_t i = 0; i + 1 < active.size(); ++i) {
                active[i]->render(*currentTexture, *writeFb);
                currentTexture = &writeFb->getTexture();
                std::swap(writeFb, readFb);
            }

            if (target != nullptr)
                active.back()->render(*currentTexture, *target);
            else
                active.back()->render(*currentTexture);
        }


};

}

#endif //SHIMERA_SFML_EFFECTPIPELINE_HPP
