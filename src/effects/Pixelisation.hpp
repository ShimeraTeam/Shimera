#pragma once

#include "ShaderEffect.inl"
#include "uniform/Vec2.inl"

namespace shimera {

class Pixelisation final : public ShaderEffect<Pixelisation> {
    public:
        explicit Pixelisation(float m_pixelSizeX = 4.0f,
                          float m_pixelSizeY = 4.0f,
                          float gap = 0.3f,
                          Vec2<float> m_uResolution = Vec2(1920.0f, 1080.0f),
                          Vec2<float> m_uOffset = Vec2(0.0f, 0.0f));

        void updateUniforms() override;
        [[nodiscard]] std::string getName() const override { return "Pixelisation"; }

        Pixelisation &withPixelSize(float pixelSize);
        Pixelisation &withPixelSizeX(float pixelSizeX);
        Pixelisation &withPixelSizeY(float pixelSizeY);
        Pixelisation &withResolution(Vec2<float> resolution);
        Pixelisation &withOffset(Vec2<float> offset);
        
        [[nodiscard]] float getPixelSize() const { return m_pixelSizeX; }
        [[nodiscard]] float getPixelSizeX() const { return m_pixelSizeX; }
        [[nodiscard]] float getPixelSizeY() const { return m_pixelSizeY; }
        [[nodiscard]] Vec2<float> getResolution() const { return m_resolution; }
        [[nodiscard]] Vec2<float> getOffset() const { return m_offset; }

    private:
        float m_pixelSizeX;
        float m_pixelSizeY;
        Vec2<float> m_resolution;
        Vec2<float> m_offset;
};

}
