#pragma once

#include "ShaderEffect.inl"

namespace shimera {

class GaussianBlur final : public ShaderEffect<GaussianBlur> {
    public:
        explicit GaussianBlur(float sigma = 3.0f, int samples = 10, Vec2<float> resolution = Vec2(1920.0f, 1080.0f));

        void render(const GLTexture& input) override;
        void render(const GLTexture& input, GLFramebuffer& target) override;
        void resize(int width, int height) override;
        void updateUniforms() override;

        [[nodiscard]] std::string getName() const override { return "GaussianBlur"; }

        GaussianBlur& withSigma(float sigma);
        GaussianBlur& withSamples(int samples);
        GaussianBlur& withResolution(Vec2<float> resolution);

        [[nodiscard]]float getSigma() const { return m_sigma; }
        [[nodiscard]]int getSamples() const { return m_samples; }
        [[nodiscard]]Vec2<float> getResolution() const { return m_uResolution; }

    private:
        void renderImpl(const GLTexture& input, uint32_t target);
    
        float m_sigma;
        int m_samples;
        Vec2<float> m_uResolution = Vec2(1920.0f, 1080.0f);
        std::unique_ptr<GLFramebuffer> m_intermediateBuffer;
};

}