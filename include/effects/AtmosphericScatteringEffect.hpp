#ifndef SHIMERA_ATMOSPHERICSCATTERINGEFFECT_HPP
#define SHIMERA_ATMOSPHERICSCATTERINGEFFECT_HPP

#include "ShaderEffect.inl"
#include "shimera_api.h"
#include "backend/IBackend.hpp"

namespace shimera {

class SHIMERA_API AtmosphericScatteringEffect final : public ShaderEffect<AtmosphericScatteringEffect> {
    public:
        Vec3<float> m_uCameraPos {0,0,0};
        Vec3<float> m_uCameraTarget{0,0,0};
        Vec3<float> m_uCameraUp {0,1,0};
        float m_fovYDegrees = 45.0f;
        float m_aspect = 16.0f/9.0f;
        float m_uNear = 0.1f, m_uFar = 1000.0f;

        Vec3<float> m_uLightDir {0,1,0};
        Vec3<float> m_uPlanetCenter {0,0,0};
        float m_uPlanetRadius = 100.0f;
        float m_uAtmosphereRadius = 120.0f;
        float m_uDensityFalloff = 4.0f;
        Vec3<float> m_uScatterCoefficients {5.5f, 13.0f, 22.4f};
        int m_uOpticalDepthSamples = 16;
        int m_uInScatteringPoints = 16;

        explicit AtmosphericScatteringEffect(IBackend* backend);

        void render(ITexture& scene) override;                       // overridden: also binds depth
        void render(ITexture& scene, IFrameBuffer& target) override;
        void updateUniforms() override;
        [[nodiscard]] bool isDepthNeeded() const override;
        void setDepthTexture(ITexture& depth) override;

        AtmosphericScatteringEffect& withCamera(Vec3<float> pos, Vec3<float> target, float fovY, float aspect, float near, float far);
        AtmosphericScatteringEffect& withSun(Vec3<float> dir);
        AtmosphericScatteringEffect& withPlanet(Vec3<float> center, float radius, float atmosphereRadius);
        AtmosphericScatteringEffect& withQuality(int opticalDepthSamples, int inScatteringPoints);

        std::string getName() const override;

    private:
        ITexture* m_depth = nullptr;
};

}

#endif //SHIMERA_ATMOSPHERICSCATTERINGEFFECT_HPP
