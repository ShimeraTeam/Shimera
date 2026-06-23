#ifndef SHIMERA_SATURATIONEFFECT_HPP
#define SHIMERA_SATURATIONEFFECT_HPP
#include <shimera_api.h>
#include "ShaderEffect.inl"
#include "backend/IBackend.hpp"

namespace shimera {

class SHIMERA_API SaturationEffect final : public ShaderEffect<SaturationEffect> {
    public:
        float m_uStrength = 1.0f;

        explicit SaturationEffect(IBackend *backend);

        SaturationEffect(IBackend *backend, float u_strength);

        void updateUniforms() override;

        SaturationEffect &withStrength(float u_strength);

        std::string getName() const override;
};

}

#endif //SHIMERA_SATURATIONEFFECT_HPP