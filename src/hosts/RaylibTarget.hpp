#pragma once

#include <raylib.h>

#include <optional>

#include "hosts/IHostTarget.hpp"
#include "scene/Camera.hpp"

namespace shimera {

/**
 * Host target for raylib.
 *
 * Every piece of rlgl knowledge in the whole system now lives in this one file, most of
 * it in the private makeWithDepthTexture().
 */
class RaylibTarget final : public IHostTarget {
    public:
        RaylibTarget(int width, int height, bool samplableDepth = false);
        ~RaylibTarget() override;

        void begin() override;
        void end() override;

        // A Camera sharing raylib's EXACT view and projection. Call inside BeginMode3D.
        [[nodiscard]] static Camera cameraFromRaylib(const Vec3<float>& eyePosition);

        [[nodiscard]] GLTexture& getColor() override { return *m_color; }
        [[nodiscard]] GLTexture* getDepth() override { return m_depth ? &*m_depth : nullptr; }

        void resize(int width, int height) override;
        [[nodiscard]] int getWidth() const override { return m_width; }
        [[nodiscard]] int getHeight() const override { return m_height; }

    protected:
        void* nativeHandle() override { return &m_renderTexture; }

    private:
        // The beta's rlgl branch, moved here unchanged. ~20 lines, and the only rlgl left.
        static RenderTexture2D makeWithDepthTexture(int width, int height);

        RenderTexture2D m_renderTexture{};
        std::optional<GLTexture> m_color;
        std::optional<GLTexture> m_depth;
        int m_width = 0;
        int m_height = 0;
        bool m_samplableDepth = false;
};

}
