#ifndef SHIMERA_SFML_CHROMATICABERRATION_HPP
#define SHIMERA_SFML_CHROMATICABERRATION_HPP

#include "ShaderEffect.inl"
#include "backend/IBackend.hpp"

class ChromaticAberrationEffect final : public ShaderEffect<ChromaticAberrationEffect> {
    public:
        float strength = 1.0f;
        bool radius = false;
        float contrast = 2.0f;
        int samples = 20;

        explicit ChromaticAberrationEffect(IBackend *backend);

        ChromaticAberrationEffect(IBackend *backend,
            float strength,
            bool radius,
            float contrast = 2.0f,
            int samples = 20
        );

        void updateUniforms() override;

        ChromaticAberrationEffect &withStrength(float s);
        ChromaticAberrationEffect &withRadius(bool r);
        ChromaticAberrationEffect &withContrast(float c);
        ChromaticAberrationEffect &withSamples(int s);
};

#endif //SHIMERA_SFML_CHROMATICABERRATION_HPP