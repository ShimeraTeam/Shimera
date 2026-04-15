#ifndef SHIMERA_RAYLIBBACKEND_HPP
#define SHIMERA_RAYLIBBACKEND_HPP

#include "backend/IBackend.hpp"

/**
 * Raylib backend implementation
 * Creates Raylib-specific implementations of framework interfaces
 */
class RaylibBackend final : public IBackend {
public:
    RaylibBackend() = default;
    ~RaylibBackend() override = default;

    IFrameBuffer* createFrameBuffer(int width, int height) override;
    IPostProccessor* createPostProcessor(const std::string& vert, const std::string& frag) override;
    ITexture* createTexture(int width, int height) override;
    IShader* createShader(const std::string& vert, const std::string& frag) override;
};

#endif //SHIMERA_RAYLIBBACKEND_HPP