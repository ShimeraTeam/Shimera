#ifndef SHIMERA_CONTRASTEFFECT_HPP
#define SHIMERA_CONTRASTEFFECT_HPP
#include <shimera_api.h>
#include "ShaderEffect.inl"
#include "backend/IBackend.hpp"

namespace shimera {

class SHIMERA_API ContrastEffect final : public ShaderEffect<ContrastEffect> {
    public:
        float m_uContrastFactor = 1.0f;

        explicit ContrastEffect(IBackend *backend);

        ContrastEffect(IBackend *backend, float u_contrastFactor);

        void updateUniforms() override;

        ContrastEffect &withContrastFactor(float u_contrastFactor);

        std::string getName() const override;
};

}

#endif //SHIMERA_CONTRASTEFFECT_HPP