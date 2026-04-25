#ifndef SHIMERA_SFMLTEXTURE_HPP
#define SHIMERA_SFMLTEXTURE_HPP

#include <SFML/Graphics/Texture.hpp>
#include "backend/ITexture.hpp"

namespace shimera {

/**
 * SFML Texture wrapper implementing ITexture interface
 * Wraps an existing sf::Texture to help with abstraction
 */
class SFMLTexture final : public ITexture {
public:
    explicit SFMLTexture(const sf::Texture& texture);
    ~SFMLTexture() override = default;

    void bind(int slot = 0) override;
    void unbind() override;
    [[nodiscard]] unsigned int getNativeHandle() const override;
    [[nodiscard]] int getWidth() const override;
    [[nodiscard]] int getHeight() const override;

private:
    const sf::Texture& m_texture;
};

}

#endif //SHIMERA_SFMLTEXTURE_HPP
