#ifndef SHIMERA_BRIGHTNESSEFFECT_HPP
#define SHIMERA_BRIGHTNESSEFFECT_HPP
#include <shimera_api.h>
#include "ShaderEffect.inl"
#include "backend/IBackend.hpp"

class SHIMERA_API BrightnessEffect final : public ShaderEffect<BrightnessEffect> {
    public:
        float u_strength = 0.0f;

        explicit BrightnessEffect(IBackend *backend);

        BrightnessEffect(IBackend *backend, float u_strength);

        void updateUniforms() override;

        BrightnessEffect &withBrightness(float u_strength);
};

#endif //SHIMERA_BRIGHTNESSEFFECT_HPP