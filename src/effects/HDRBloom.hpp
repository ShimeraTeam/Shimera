#pragma once

#include "ShaderEffect.inl"

namespace shimera {

class HDRBloom final : public ShaderEffect<HDRBloom> {
    public:
        explicit HDRBloom(float threshold = 0.7f,
                          float knee = 0.2f,
                          float intensity = 1.0f,
                          float blurSigma = 5.0f,
                          int blurSamples = 15);

        void render(const GLTexture& input) override;
        void render(const GLTexture& input, GLFramebuffer& target) override;
        void updateUniforms() override;
        void resize(int width, int height) override;

        [[nodiscard]] std::string getName() const override { return "HDRBloom"; }

        HDRBloom& withThreshold(float threshold);
        HDRBloom& withKnee(float knee);
        HDRBloom& withIntensity(float intensity);
        HDRBloom& withBlurSigma(float sigma);
        HDRBloom& withBlurSamples(int samples);

    private:
        void renderImpl(const GLTexture& input, uint32_t target);

        std::unique_ptr<GLFramebuffer> m_brightBlurredH;
        float m_threshold;
        float m_knee;
        float m_intensity;
        float m_blurSigma;
        int m_blurSamples;
};

}
