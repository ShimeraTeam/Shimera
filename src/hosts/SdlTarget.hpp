#pragma once

#include <SDL3/SDL.h>

#include <memory>
#include <optional>

#include "hosts/IHostTarget.hpp"

namespace shimera {

class GLFramebuffer;

/**
 * Host target for SDL3's SDL_Renderer.
 *
 * This only works when the renderer was created with
 * the "opengl" driver (`SDL_CreateRenderer(window, "opengl")`)
 *
 * The scene is an SDL render-target texture, so the host keeps drawing with plain
 * SDL_Render* calls.
 */
class SdlTarget final : public IHostTarget {
    public:
        SdlTarget(SDL_Renderer& renderer, int width, int height);
        ~SdlTarget() override;

        void begin() override;
        void end() override;

        [[nodiscard]] GLTexture& getColor() override;

        void resize(int width, int height) override;
        [[nodiscard]] int getWidth() const override { return m_width; }
        [[nodiscard]] int getHeight() const override { return m_height; }

    protected:
        void* nativeHandle() override { return m_renderer; }

    private:
        SDL_Renderer* m_renderer;
        SDL_Texture* m_target = nullptr;

        // Read-side wrapper around SDL's texture, only ever used as the blit source.
        uint32_t m_sdlFbo = 0;
        std::optional<GLTexture> m_sdlColor;

        // The y-corrected copy that the pipeline actually samples.
        std::unique_ptr<GLFramebuffer> m_scene;

        int m_width = 0;
        int m_height = 0;
};

} // namespace shimera
