#ifndef SHIMERA_SFML_DISTORTIONEFFECT_HPP
#define SHIMERA_SFML_DISTORTIONEFFECT_HPP
#include <shimera_api.h>
#include "ShaderEffect.inl"
#include "backend/IBackend.hpp"

class SHIMERA_API DistortionEffect final : public ShaderEffect<DistortionEffect> {
    public:
        float time = 0.0f;
        float noiseScale = 3.0f;
        float distortionStrength = 0.13f;
        float timeScale = 0.1f;

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

#endif //SHIMERA_SFML_DISTORTIONEFFECT_HPP