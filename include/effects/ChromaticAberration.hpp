#ifndef SHIMERA_SFML_CHROMATICABERRATION_HPP
#define SHIMERA_SFML_CHROMATICABERRATION_HPP

#include "ShaderEffect.inl"
#include "backend/IBackend.hpp"

class ChromaticAberrationEffect final : public ShaderEffect<ChromaticAberrationEffect> {
    public:
        Vec2<float> offset = Vec2(0.005f, 0.005f);
        float radius = 0.0f;

        explicit ChromaticAberrationEffect(IBackend *backend);

        ChromaticAberrationEffect(IBackend *backend,
            Vec2<float> offset,
            float radius
        );

        void updateUniforms() override;

        ChromaticAberrationEffect &withOffset(Vec2<float> o);
        ChromaticAberrationEffect &withRadius(float r);
};

#endif //SHIMERA_SFML_CHROMATICABERRATION_HPP