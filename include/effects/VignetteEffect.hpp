#ifndef SHIMERA_VIGNETTEEFFECT_HPP
#define SHIMERA_VIGNETTEEFFECT_HPP
#include <shimera_api.h>
#include "ShaderEffect.inl"
#include "backend/IBackend.hpp"

class SHIMERA_API VignetteEffect final : public ShaderEffect<VignetteEffect> {
    public:
        float u_strength = 1.0f;
        float u_radius = 0.5f;
        float u_gap = 0.3f;
        Vec4<float> u_color = Vec4<float>(0.0f, 0.0f, 0.0f, 1.0f);
        int u_isRounded = 0;
        Vec2<float> u_resolution = Vec2<float>(1920.0f, 1080.0f);

        explicit VignetteEffect(IBackend *backend);

        VignetteEffect(IBackend *backend,
            float strength,
            float radius,
            float gap,
            Vec4<float> color = Vec4<float>(0.0f, 0.0f, 0.0f, 1.0f),
            bool isRounded = false,
            Vec2<float> resolution = Vec2<float>(1920.0f, 1080.0f));

        void updateUniforms() override;

        VignetteEffect &withStrength(float strength);
        VignetteEffect &withRadius(float radius);
        VignetteEffect &withGap(float gap);
        VignetteEffect &withColor(Vec4<float> color);
        VignetteEffect &withIsRounded(bool isRounded);
        VignetteEffect &withResolution(Vec2<float> resolution);
};

#endif //SHIMERA_VIGNETTEEFFECT_HPP