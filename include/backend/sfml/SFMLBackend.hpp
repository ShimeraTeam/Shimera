#ifndef SHIMERA_SFMLBACKEND_HPP
#define SHIMERA_SFMLBACKEND_HPP

#include "backend/IBackend.hpp"

namespace shimera {

/**
 * SFML backend implementation
 * Creates SFML-specific implementations of framework interfaces
 */
class SFMLBackend final : public IBackend {
public:
    SFMLBackend() = default;
    ~SFMLBackend() override = default;

    IFrameBuffer* createFrameBuffer(int width, int height, bool samplableDepth = false) override;
    IPostProcessor* createPostProcessor(const std::string& vert, const std::string& frag) override;
    ITexture* createTexture(int width, int height) override;
    IShader* createShader(const std::string& vert, const std::string& frag) override;
};

}

#endif //SHIMERA_SFMLBACKEND_HPP
