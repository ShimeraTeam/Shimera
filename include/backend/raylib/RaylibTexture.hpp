#ifndef SHIMERA_RAYLIBTEXTURE_HPP
#define SHIMERA_RAYLIBTEXTURE_HPP

#include <raylib.h>

#include "backend/ITexture.hpp"

namespace shimera {

/**
 * Raylib Texture wrapper implementing ITexture interface
 * Wraps an existing Texture to help with abstraction
 */
class RaylibTexture final : public ITexture {
public:
    explicit RaylibTexture(const Texture& texture);
    ~RaylibTexture() override = default;

    void bind(int slot = 0) override;
    void unbind() override;
    [[nodiscard]] unsigned int getNativeHandle() const override;
    [[nodiscard]] int getWidth() const override;
    [[nodiscard]] int getHeight() const override;

private:
    const Texture& m_texture;
};

}

#endif //SHIMERA_RAYLIBTEXTURE_HPP
