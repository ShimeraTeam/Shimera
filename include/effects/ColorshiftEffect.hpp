#ifndef SHIMERA_SFML_COLORSHIFTEFFECT_HPP
#define SHIMERA_SFML_COLORSHIFTEFFECT_HPP
#include "ShaderEffect.inl"
#include "backend/IBackend.hpp"

class ColorshiftEffect final : public ShaderEffect<ColorshiftEffect> {
    public:
        explicit ColorshiftEffect(IBackend *backend);
        ColorshiftEffect(IBackend *backend, vec3 colorShift);

        void updateUniforms() override;
        ColorshiftEffect &withColorShift(const vec3 &colorShift);

        vec3 colorShift = vec3(0.0f);
};

#endif //SHIMERA_SFML_COLORSHIFTEFFECT_HPP