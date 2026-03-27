/**
* DEPRECATED: This class is deprecated. It was the first implementation of post-processing SFML abstraction.
* Being only Single-Pass, it is now replaced by `BackendFactory` that is Multi-Pass capable.
* It is kept for proof of work purposes only.
*/

#ifndef SHIMERA_SFML_POSTPROCESSINGPIPELINE_INL
#define SHIMERA_SFML_POSTPROCESSINGPIPELINE_INL

#include "PostProcessingPipelineBase.inl"

#ifdef SHIMERA_BACKEND_SFML
#include <SFML/Graphics.hpp>
#include "sfml/SFMLPostProcessor.hpp"
#include "sfml/SFMLTexture.hpp"

namespace shimera {

class PostProcessingPipeline : public PostProcessingPipelineBase<PostProcessingPipeline>{
    sf::RenderTexture renderTexture;
    std::unique_ptr<SFMLPostProcessor> postProcessor;

    public:
        PostProcessingPipeline(const int w, const int h, const std::string& vertPath, const std::string& fragPath) {
            width = w;
            height = h;

            if (!renderTexture.resize({static_cast<unsigned>(w), static_cast<unsigned>(h)})) {
                throw std::runtime_error("Failed to create render texture");
            }

            postProcessor = std::make_unique<SFMLPostProcessor>();
            postProcessor->setShader(vertPath, fragPath);
        }

        sf::RenderTexture& getRenderTexture() {
            return renderTexture;
        }

        void render() const {
            SFMLTexture texture(renderTexture.getTexture());
            postProcessor->render(texture);
        }

        void applyUniform(const std::string& name, const UniformValue& value) const{
            postProcessor->setUniform(name, value);
        }

        void onResize(const int w, const int h) {
            if (!renderTexture.resize({static_cast<unsigned>(w), static_cast<unsigned>(h)})) {
                throw std::runtime_error("Failed to resize render texture");
            }
        }
};

}
#endif

#endif //SHIMERA_SFML_POSTPROCESSINGPIPELINE_INL