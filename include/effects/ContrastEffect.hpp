#ifndef SHIMERA_CONTRASTEFFECT_HPP
#define SHIMERA_CONTRASTEFFECT_HPP
#include <shimera_api.h>
#include "ShaderEffect.inl"
#include "backend/IBackend.hpp"

class SHIMERA_API ContrastEffect final : public ShaderEffect<ContrastEffect> {
    public:
        float u_contrastFactor = 1.0f;

        explicit ContrastEffect(IBackend *backend);

        ContrastEffect(IBackend *backend, float u_contrastFactor);

        void updateUniforms() override;

        ContrastEffect &withContrastFactor(float u_contrastFactor);
};

#endif //SHIMERA_CONTRASTEFFECT_HPP