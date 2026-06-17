#ifndef SHIMERA_IBACKEND_HPP
#define SHIMERA_IBACKEND_HPP

#include <shimera_api.h>
#include "IFrameBuffer.hpp"
#include "IPostProcessor.hpp"

namespace shimera {

/**
 * Backend interface
 * Defines methods that should be implemented by any rendering backend and serves as a bridge
 */
class SHIMERA_API IBackend {
    public:
        virtual ~IBackend() = default;

        virtual IFrameBuffer* createFrameBuffer(int width, int height,
                                                    bool samplableDepth = false) = 0;
        virtual IPostProcessor* createPostProcessor(const std::string& vert,
                                                        const std::string& frag) = 0;
        virtual ITexture* createTexture(int width, int height) = 0;
        virtual IShader* createShader(const std::string& vert,
                                        const std::string& frag) = 0;
};

}

#endif //SHIMERA_IBACKEND_HPP