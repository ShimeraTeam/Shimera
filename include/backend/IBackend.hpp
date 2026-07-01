#ifndef SHIMERA_IBACKEND_HPP
#define SHIMERA_IBACKEND_HPP

#include <shimera_api.h>
#include <vector>

#include "IFrameBuffer.hpp"
#include "IMaterial.hpp"
#include "IPostProcessor.hpp"
#include "IMesh.hpp"
#include "scene/Camera.hpp"

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
        virtual IMesh* createMesh(const std::vector<float>& positions,
                          const std::vector<float>& normals,
                          const std::vector<unsigned int>& indices) = 0;
        virtual IMaterial* createMaterial(const std::string& vert,
                                          const std::string& frag) = 0;
        virtual void renderMaterial(IMaterial& material, IMesh& mesh,
                                    const Camera& camera, const Mat4& transform) = 0;
        virtual ITexture* createTexture(int width, int height) = 0;
        virtual IShader* createShader(const std::string& vert,
                                        const std::string& frag) = 0;
};

}

#endif //SHIMERA_IBACKEND_HPP