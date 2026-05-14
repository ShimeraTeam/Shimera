#ifndef SHIMERA_PIXELISATIONEFFECT_HPP
#define SHIMERA_PIXELISATIONEFFECT_HPP
#include <shimera_api.h>
#include "ShaderEffect.inl"
#include "backend/IBackend.hpp"
#include "uniform/Vec2.inl"

namespace shimera {

class SHIMERA_API PixelisationEffect final : public ShaderEffect<PixelisationEffect> {
    public:
        float m_uPixelSizeX = 4.0f;
        float m_uPixelSizeY = 4.0f;
        Vec2<float> m_uResolution = Vec2(1920.0f, 1080.0f);
        Vec2<float> m_uOffset = Vec2(0.0f, 0.0f);

        explicit PixelisationEffect(IBackend *backend);

        PixelisationEffect(IBackend *backend,
            float pixelSize,
            Vec2<float> resolution = Vec2(1920.0f, 1080.0f));

        void updateUniforms() override;

        PixelisationEffect &withPixelSize(float pixelSize);
        PixelisationEffect &withPixelSizeX(float pixelSizeX);
        PixelisationEffect &withPixelSizeY(float pixelSizeY);
        PixelisationEffect &withResolution(Vec2<float> resolution);
        PixelisationEffect &withOffset(Vec2<float> offset);
};

}

#endif //SHIMERA_PIXELISATIONEFFECT_HPP
