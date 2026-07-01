#ifndef SHIMERA_BRIGHTNESSEFFECT_HPP
#define SHIMERA_BRIGHTNESSEFFECT_HPP
#include <shimera_api.h>
#include "ShaderEffect.inl"
#include "backend/IBackend.hpp"

namespace shimera {

class SHIMERA_API BrightnessEffect final : public ShaderEffect<BrightnessEffect> {
    public:
        float m_uStrength = 0.0f;

        explicit BrightnessEffect(IBackend *backend);

        BrightnessEffect(IBackend *backend, float u_strength);

        void updateUniforms() override;

        BrightnessEffect &withStrength(float u_strength);

        std::string getName() const override;
};

}

#endif //SHIMERA_BRIGHTNESSEFFECT_HPP