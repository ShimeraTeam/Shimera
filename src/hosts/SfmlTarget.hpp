#pragma once

#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <optional>

#include "hosts/IHostTarget.hpp"

namespace shimera {

/**
 * Host target for SFML 3.
 *
 * This is the entire SFML integration. Compare the beta, 5 files, ~470 lines, of which
 * ~400 were OpenGL code pasted from the other two "backends".
 *
 * sf::RenderTexture carries its own GL context, so after drawing the scene SOMETHING
 * has to make the window current again or every later GL call lands in the wrong context.
 * Only the user knows which window, so the target is told once at construction and end() handles it.
 * That keeps the per-frame API identical to the other hosts.
 *
 * No getDepth() override, sf::RenderTexture can carry a depth buffer via ContextSettings
 * but will not hand it over as a samplable texture. To see if that causes problems in the future.
 */
class SfmlTarget final : public IHostTarget {
    public:
        SfmlTarget(sf::RenderWindow& window, int width, int height);

        void begin() override;
        void end() override;

        [[nodiscard]] GLTexture& getColor() override { return *m_color; }

        void resize(int width, int height) override;
        [[nodiscard]] int getWidth() const override { return m_width; }
        [[nodiscard]] int getHeight() const override { return m_height; }

    protected:
        void* nativeHandle() override { return &m_renderTexture; }

    private:
        sf::RenderWindow* m_window;
        sf::RenderTexture m_renderTexture;
        std::optional<GLTexture> m_color;
        int m_width = 0;
        int m_height = 0;
};

}
