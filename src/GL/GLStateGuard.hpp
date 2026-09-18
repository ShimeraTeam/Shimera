#pragma once

#include <array>

namespace shimera {

/**
 * Captures the host's GL state on construction and puts it back on destruction.
 *
 * There is one OpenGL context per window, and it is a global state machine, one current
 * program, one bound VAO, one blend mode, for everyone in the process. Sharing it is
 * automatic. Making it SAFE is not, because host libraries *cache* what they believe is
 * bound and skip redundant binds. So anything Shimera changes and does not put back
 * silently poisons the host's next draw.
 *
 * The field list was made following Effekseer's RenderStateSet. Saving "everything" via
 * glPushAttrib is deprecated and slow, and SFML's own docs call their save-all "quite expensive".
 */
class GLStateGuard {
    public:
        GLStateGuard();
        ~GLStateGuard();

        GLStateGuard(const GLStateGuard&) = delete;
        GLStateGuard& operator=(const GLStateGuard&) = delete;
        GLStateGuard(GLStateGuard&&) = delete;
        GLStateGuard& operator=(GLStateGuard&&) = delete;

        /* Can be disabled for users who render at a known safe point and want to
         * skip ~25 glGet. Enabled by default. */
        static void setEnabled(bool enabled);
        [[nodiscard]] static bool isEnabled();

        // Texture units Shimera is allowed to touch, and therefore has to restore.
        static constexpr int kTextureUnits = 8;

    private:
        void capture();
        void restore() const;

        static bool s_enabled;

        bool m_captured = false;

        // enables
        unsigned char m_blend = 0;
        unsigned char m_cullFace = 0;
        unsigned char m_depthTest = 0;
        unsigned char m_scissorTest = 0;
        // depth
        unsigned char m_depthMask = 0;
        int m_depthFunc = 0;
        // blend, separate rgb/alpha because hosts do use separate blending
        int m_blendSrcRGB = 0;
        int m_blendDstRGB = 0;
        int m_blendSrcAlpha = 0;
        int m_blendDstAlpha = 0;
        int m_blendEquationRGB = 0;
        int m_blendEquationAlpha = 0;
        // bindings
        int m_program = 0;
        int m_vertexArray = 0;
        int m_arrayBuffer = 0;
        int m_elementBuffer = 0;
        int m_drawFramebuffer = 0;
        int m_activeTexture = 0;
        std::array<int, kTextureUnits> m_texture2D{};
        // rasterizer
        int m_viewport[4] = {0, 0, 0, 0};
        /* a good exemple of why adding target can break:
         * SDL's renderer caches the clear color and SKIPS glClearColor when it thinks
         * the value is already set, so ours leaking out turns the host's clears black. */
        float m_clearColor[4] = {0.0f, 0.0f, 0.0f, 0.0f};
};

}
