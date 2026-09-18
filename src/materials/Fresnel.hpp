#pragma once

#include "ShaderMaterial.inl"
#include "uniform/Vec3.inl"

namespace shimera {

/**
 * Fresnel, bright where the surface turns away from the camera.
 */
class Fresnel final : public ShaderMaterial<Fresnel> {
    public:
        explicit Fresnel(Vec3<float> color = Vec3(0.3f, 0.6f, 1.0f),
                         float power = 3.0f,
                         float reflectance = 0.04f,
                         float intensity = 1.5f);

        void updateUniforms() override;
        [[nodiscard]] std::string getName() const override { return "Fresnel"; }

        Fresnel& withColor(Vec3<float> color);
        Fresnel& withPower(float power);
        Fresnel& withReflectance(float reflectance);
        Fresnel& withIntensity(float intensity);

    private:
        Vec3<float> m_color;
        float m_power;
        float m_reflectance;
        float m_intensity;
};

}
