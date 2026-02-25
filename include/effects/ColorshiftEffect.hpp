#ifndef SHIMERA_SFML_COLORSHIFTEFFECT_HPP
#define SHIMERA_SFML_COLORSHIFTEFFECT_HPP
#include "ShaderEffect.inl"
#include "backend/IBackend.hpp"
#include "uniform/Vec3.hpp"

class ColorshiftEffect final : public ShaderEffect<ColorshiftEffect> {
    public:
        explicit ColorshiftEffect(IBackend *backend);
        ColorshiftEffect(IBackend *backend, Vec3<float> colorShift);

        void updateUniforms() override;
        ColorshiftEffect &withColorShift(const Vec3<float> &colorShift);

        Vec3<float> colorShift = Vec3<float>(0.0f);
};

#endif //SHIMERA_SFML_COLORSHIFTEFFECT_HPP