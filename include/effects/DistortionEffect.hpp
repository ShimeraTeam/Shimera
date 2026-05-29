#ifndef SHIMERA_DISTORTIONEFFECT_HPP
#define SHIMERA_DISTORTIONEFFECT_HPP
#include <shimera_api.h>
#include "ShaderEffect.inl"
#include "backend/IBackend.hpp"

namespace shimera {

class SHIMERA_API DistortionEffect final : public ShaderEffect<DistortionEffect> {
    public:
        float m_uTime = 0.0f;
        float m_uNoiseScale = 3.0f;
        float m_uDistortionStrength = 0.13f;
        float m_uTimeScale = 0.1f;

        explicit DistortionEffect(IBackend *backend);

        DistortionEffect(IBackend *backend,
            float time,
            float noiseScale,
            float distortionStrength,
            float timeScale);

        void updateUniforms() override;

        DistortionEffect &withTime(float time);
        DistortionEffect &withNoiseScale(float noiseScale);
        DistortionEffect &withDistortionStrength(float distortionStrength);
        DistortionEffect &withTimeScale(float timeScale);
};

}

#endif //SHIMERA_DISTORTIONEFFECT_HPP