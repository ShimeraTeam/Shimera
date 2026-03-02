#ifndef SHIMERA_SATURATIONEFFECT_HPP
#define SHIMERA_SATURATIONEFFECT_HPP
#include <shimera_api.h>
#include "ShaderEffect.inl"
#include "backend/IBackend.hpp"

class SHIMERA_API SaturationEffect final : public ShaderEffect<SaturationEffect> {
    public:
        float u_strength = 1.0f;

        explicit SaturationEffect(IBackend *backend);

        SaturationEffect(IBackend *backend, float u_strength);

        void updateUniforms() override;

        SaturationEffect &withSaturation(float u_strength);
};

#endif //SHIMERA_SATURATIONEFFECT_HPP