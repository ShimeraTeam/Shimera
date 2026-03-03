#ifndef SHIMERA_RAYLIBTEXTURE_HPP
#define SHIMERA_RAYLIBTEXTURE_HPP

#include "backend/ITexture.hpp"
#include "raylib.h"

/**
 * Raylib Texture wrapper implementing ITexture interface
 * Wraps an existing Texture2D to help with abstraction
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
    const Texture& texture;
};

#endif //SHIMERA_RAYLIBTEXTURE_HPP
