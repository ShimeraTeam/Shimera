#ifndef SHIMERA_COLORSHIFTEFFECT_HPP
#define SHIMERA_COLORSHIFTEFFECT_HPP
#include <shimera_api.h>
#include "ShaderEffect.inl"
#include "backend/IBackend.hpp"
#include "uniform/Vec3.hpp"

class SHIMERA_API ColorshiftEffect final : public ShaderEffect<ColorshiftEffect> {
    public:
        Vec3<float> colorShift = Vec3<float>(0.0f);

        explicit ColorshiftEffect(IBackend *backend);

        ColorshiftEffect(IBackend *backend, Vec3<float> colorShift);

        void updateUniforms() override;

        ColorshiftEffect &withColorShift(const Vec3<float> &colorShift);
};

#endif //SHIMERA_COLORSHIFTEFFECT_HPP