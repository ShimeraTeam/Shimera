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

#pragma once

#include <memory>
#include <string>

#include "GL/GLFramebuffer.hpp"
#include "GL/GLPostProcessor.hpp"
#include "GL/GLTexture.hpp"
#include "uniform/Vec2.inl"

namespace shimera {

/**
 * Base of every post-processing effect. Based on the beta's ShaderEffectBase.
 *
 * The resolution is pushed down by the pipeline
 * through resize() instead of being a uniform the user is expected to set. In the beta
 * every effect defaulted to 1920x1080 and silently rendered a vignette sized for the
 * wrong screen until someone called withResolution().
 */
class Effect {
    public:
        virtual ~Effect() = default;

        Effect() = default;
        Effect(Effect&&) = default;
        Effect& operator=(Effect&&) = default;
        Effect(const Effect&) = delete;
        Effect& operator=(const Effect&) = delete;

        [[nodiscard]] virtual std::string getName() const = 0;

        // Draws into whichever framebuffer is currently bound.
        virtual void render(const GLTexture& input) = 0;
        // Draws into `target`. Leaves it bound, the pipeline owns what comes next. (if we ever need that)
        virtual void render(const GLTexture& input, GLFramebuffer& target) = 0;

        virtual void updateUniforms() = 0;

        // Called by the pipeline on build and on every resize (obviously).
        virtual void resize(int width, int height);

        [[nodiscard]] virtual bool needsDepth() const { return false; }
        virtual void setDepthTexture(const GLTexture& depth) { m_depth = &depth; }

        void setEnabled(const bool enabled) { m_enabled = enabled; }
        [[nodiscard]] bool isEnabled() const { return m_enabled; }

        [[nodiscard]] GLPostProcessor& processor() const { return *m_processor; }

    protected:
        std::unique_ptr<GLPostProcessor> m_processor;
        const GLTexture* m_depth = nullptr;
        Vec2<float> m_resolution{1.0f, 1.0f};
        bool m_enabled = true;
};

}
