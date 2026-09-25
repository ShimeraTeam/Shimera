#pragma once

#include "ShaderEffect.inl"

namespace shimera {

class Distortion final : public ShaderEffect<Distortion> {
    public:
        explicit Distortion(float noiseScale = 3.0f,
                            float distortionStrength = 0.13f,
                            float timeScale = 0.1f);

        void updateUniforms() override;
        [[nodiscard]] std::string getName() const override { return "Distortion"; }

        Distortion& withTime(float time);
        Distortion& withNoiseScale(float noiseScale);
        Distortion& withDistortionStrength(float distortionStrength);
        Distortion& withTimeScale(float timeScale);
        float getTime() const;
        float getNoiseScale() const;
        float getDistortionStrength() const;
        float getTimeScale() const;

    private:
        float m_time = 0.0f;
        float m_noiseScale;
        float m_distortionStrength;
        float m_timeScale;
};

}
