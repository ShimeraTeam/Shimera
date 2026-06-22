#ifndef SHIMERA_COLORTINTEFFECT_HPP
#define SHIMERA_COLORTINTEFFECT_HPP
#include <shimera_api.h>
#include "ShaderEffect.inl"
#include "backend/IBackend.hpp"
#include "uniform/Vec3.inl"

namespace shimera {

class SHIMERA_API ColortintEffect final : public ShaderEffect<ColortintEffect> {
    public:
        Vec3<float> m_uTint = Vec3(0.0f);

        explicit ColortintEffect(IBackend *backend);

        ColortintEffect(IBackend *backend, Vec3<float> tint);

        void updateUniforms() override;

        ColortintEffect &withTint(const Vec3<float> &tint);
};

}

#endif //SHIMERA_COLORTINTEFFECT_HPP