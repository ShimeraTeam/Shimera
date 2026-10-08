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

#include "GL/GLTexture.hpp"

namespace shimera {

/**
 * What Shimera needs from a host library, and nothing else.
 *
 * A target answers three questions:
 *  1. where does the user draw? -> native<T>()
 *  2. how do I start/stop that? -> begin() / end()
 *  3. what GL texture holds it? -> getColor() (+ optional getDepth())
 *
 * If a proposed method does not answer one of those three, it does not belong here.
 * Everything from the GLuint is identical for every host.
 */
class IHostTarget {
    public:
        virtual ~IHostTarget() = default;

        IHostTarget() = default;
        IHostTarget(const IHostTarget&) = delete;
        IHostTarget& operator=(const IHostTarget&) = delete;

        // make the offscreen target active for the host's own draw calls.
        virtual void begin() = 0;
        /** finish it, AND flush any host-side batching. After end(),
         * getColor() must be complete and safe to sample. */
        virtual void end() = 0;

        [[nodiscard]] virtual GLTexture& getColor() = 0;

        // nullptr when the host cannot, or was not asked to provide depth.
        [[nodiscard]] virtual GLTexture* getDepth() { return nullptr; }

        virtual void resize(int width, int height) = 0;
        [[nodiscard]] virtual int getWidth() const = 0;
        [[nodiscard]] virtual int getHeight() const = 0;

        /** Typed access to the native object the user draws on. Replaces the beta's raw
         * `void* getNativeRenderTarget()` plus a user-side static_cast. The cast still
         * exists because the type is host-specific, but it lives in here now and the user
         * writes scene.native<sf::RenderTexture>()->draw(sprite). */
        template <typename T>
        T* native() { return static_cast<T*>(nativeHandle()); }

    protected:
        // Templates cannot be virtual, hence the untyped hook behind native<T>().
        virtual void* nativeHandle() { return nullptr; }
};

}
