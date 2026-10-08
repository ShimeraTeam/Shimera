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
