#ifndef SHIMERA_SATURATIONEFFECT_HPP
#define SHIMERA_SATURATIONEFFECT_HPP
#include <shimera_api.h>
#include "ShaderEffect.inl"
#include "backend/IBackend.hpp"

class SHIMERA_API SaturationEffect final : public ShaderEffect<SaturationEffect> {
    public:
        float saturation = 1.0f;

        explicit SaturationEffect(IBackend *backend);

        SaturationEffect(IBackend *backend, float saturation);

        void updateUniforms() override;

        SaturationEffect &withSaturation(float saturation);
};

#endif //SHIMERA_SATURATIONEFFECT_HPP