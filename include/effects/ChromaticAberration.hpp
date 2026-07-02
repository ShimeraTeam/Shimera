#ifndef SHIMERA_CHROMATICABERRATION_HPP
#define SHIMERA_CHROMATICABERRATION_HPP

#include "shimera_api.h"
#include "ShaderEffect.inl"
#include "backend/IBackend.hpp"

namespace shimera {

class SHIMERA_API ChromaticAberrationEffect final : public ShaderEffect<ChromaticAberrationEffect> {
    public:
        float m_uStrength = 1.0f;
        bool m_uRadius = false;
        float m_uContrast = 2.0f;
        int m_uSamples = 20;

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

        std::string getName() const override;
};

}

#endif //SHIMERA_CHROMATICABERRATION_HPP