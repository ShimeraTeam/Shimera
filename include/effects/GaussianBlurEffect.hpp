#ifndef SHIMERA_GAUSSIANBLUREFFECT_HPP
#define SHIMERA_GAUSSIANBLUREFFECT_HPP
#include <shimera_api.h>
#include "ShaderEffect.inl"
#include "backend/IBackend.hpp"
#include "backend/IFrameBuffer.hpp"
#include "uniform/Vec2.inl"

namespace shimera {

class SHIMERA_API GaussianBlurEffect final : public ShaderEffect<GaussianBlurEffect> {
    public:
        float m_uSigma = 3.0f;
        int m_uSamples = 10;
        Vec2<float> m_uResolution = Vec2(1920.0f, 1080.0f);

        explicit GaussianBlurEffect(IBackend *backend);

        GaussianBlurEffect(IBackend *backend,
            float sigma,
            int samples,
            Vec2<float> resolution = Vec2(1920.0f, 1080.0f));

        void render(ITexture &texture) override;
        void render(ITexture &texture, IFrameBuffer &target) override;
        void updateUniforms() override;

        GaussianBlurEffect &withSigma(float sigma);
        GaussianBlurEffect &withSamples(int samples);
        GaussianBlurEffect &withResolution(Vec2<float> resolution);

        std::string getName() const override;

    private:
        std::unique_ptr<IFrameBuffer> m_intermediateBuffer;
};

}

#endif //SHIMERA_GAUSSIANBLUREFFECT_HPP
