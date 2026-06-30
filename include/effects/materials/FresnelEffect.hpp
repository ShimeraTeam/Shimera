#ifndef TEST_MEMORY_SFML_FRESNELEFFECT_HPP
#define TEST_MEMORY_SFML_FRESNELEFFECT_HPP
#include "MaterialEffect.inl"
#include "backend/IBackend.hpp"

namespace shimera {

class FresnelEffect : public MaterIalEffect<FresnelEffect> {
    public:
        Vec3<float> m_uColor = Vec3(0.4f, 0.7f, 1.0f);
        float m_uPower = 3.0f;
        float m_uReflectance  = 0.04f;
        float m_uIntensity = 1.0f;

        explicit FresnelEffect(IBackend* backend);

        FresnelEffect(IBackend* backend, Vec3<float> color, float power, float reflectance, float intensity);

        void updateUniforms(const Camera& camera) override;

        FresnelEffect& withColor(Vec3<float> color);
        FresnelEffect& withPower(float power);
        FresnelEffect& withReflectance(float reflectance);
        FresnelEffect& withIntensity(float intensity);
};

}

#endif //TEST_MEMORY_SFML_FRESNELEFFECT_HPP
