#ifndef SHIMERA_HDRBLOOMEFFECT_HPP
#define SHIMERA_HDRBLOOMEFFECT_HPP
#include <shimera_api.h>
#include "ShaderEffect.inl"
#include "backend/IBackend.hpp"
#include "backend/IFrameBuffer.hpp"
#include "uniform/Vec2.inl"

namespace shimera {

class SHIMERA_API HDRBloomEffect final : public ShaderEffect<HDRBloomEffect> {
    public:
        float m_uThreshold = 0.7f;
        float m_uKnee = 0.2f;
        float m_uIntensity = 1.0f;
        float m_uBlurSigma = 5.0f;
        int m_uBlurSamples = 15;
        Vec2<float> m_uResolution = Vec2(1920.0f, 1080.0f);

        explicit HDRBloomEffect(IBackend *backend);

        HDRBloomEffect(IBackend *backend,
            float threshold,
            float knee,
            float intensity,
            float blurSigma,
            int blurSamples,
            Vec2<float> resolution = Vec2(1920.0f, 1080.0f));

        void render(ITexture &texture) override;
        void render(ITexture &texture, IFrameBuffer &target) override;
        void updateUniforms() override;

        HDRBloomEffect &withThreshold(float threshold);
        HDRBloomEffect &withKnee(float knee);
        HDRBloomEffect &withIntensity(float intensity);
        HDRBloomEffect &withBlurSigma(float sigma);
        HDRBloomEffect &withBlurSamples(int samples);
        HDRBloomEffect &withResolution(Vec2<float> resolution);

    private:
        std::unique_ptr<IFrameBuffer> m_brightBlurredHBuffer;
};

}

#endif //SHIMERA_HDRBLOOMEFFECT_HPP
