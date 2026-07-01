#ifndef SHIMERA_VIGNETTEEFFECT_HPP
#define SHIMERA_VIGNETTEEFFECT_HPP
#include <shimera_api.h>
#include "ShaderEffect.inl"
#include "backend/IBackend.hpp"

namespace shimera {

class SHIMERA_API VignetteEffect final : public ShaderEffect<VignetteEffect> {
    public:
        float m_uStrength = 1.0f;
        float m_uRadius = 0.5f;
        float m_uGap = 0.3f;
        Vec4<float> m_uColor = Vec4(0.0f, 0.0f, 0.0f, 1.0f);
        int m_uIsRounded = 0;
        Vec2<float> m_uResolution = Vec2(1920.0f, 1080.0f);

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

        std::string getName() const override;
};

}

#endif //SHIMERA_VIGNETTEEFFECT_HPP