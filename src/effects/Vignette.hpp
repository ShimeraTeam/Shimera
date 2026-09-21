#pragma once

#include "ShaderEffect.inl"
#include "uniform/Vec4.inl"

namespace shimera {

class Vignette final : public ShaderEffect<Vignette> {
    public:
        explicit Vignette(float strength = 1.0f,
                          float radius = 0.5f,
                          float gap = 0.3f,
                          Vec4<float> color = Vec4(0.0f, 0.0f, 0.0f, 1.0f),
                          bool rounded = false);

        void updateUniforms() override;
        [[nodiscard]] std::string getName() const override { return "Vignette"; }

        Vignette& withStrength(float strength);
        Vignette& withRadius(float radius);
        Vignette& withGap(float gap);
        Vignette& withColor(Vec4<float> color);
        Vignette& withRounded(bool rounded);

        [[nodiscard]] float getStrength() const { return m_strength; }

    private:
        float m_strength;
        float m_radius;
        float m_gap;
        Vec4<float> m_color;
        int m_rounded;
};

}
