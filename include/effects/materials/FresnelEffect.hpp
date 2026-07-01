#ifndef TEST_MEMORY_SFML_FRESNELEFFECT_HPP
#define TEST_MEMORY_SFML_FRESNELEFFECT_HPP
#include "MaterialEffect.inl"
#include "backend/IBackend.hpp"

namespace shimera {

class SHIMERA_API FresnelEffect : public MaterialEffect<FresnelEffect> {
    public:
        explicit FresnelEffect(IBackend* backend);

        void uploadUniforms(IMaterial& material);

        FresnelEffect& withColor(Vec3<float> color);
        FresnelEffect& withPower(float power);
        FresnelEffect& withReflectance(float reflectance);
        FresnelEffect& withIntensity(float intensity);

    private:
        Vec3<float> m_color{0.3f, 0.7f, 1.0f};
        float m_power = 3.0f, m_reflectance = 0.04f, m_intensity = 1.5f;
};

}

#endif //TEST_MEMORY_SFML_FRESNELEFFECT_HPP
