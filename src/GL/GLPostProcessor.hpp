#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "GLShader.hpp"
#include "GLTexture.hpp"

namespace shimera {

/**
 * One fullscreen-quad draw through one shader.
 *
 * render() binds shader, VAO and textures, draws, and stops. It deliberately does not
 * unbind anything afterwards. See GLTexture for why undoing state by zeroing it is
 * worse than leaving it to GLStateGuard.
 */
class GLPostProcessor {
    public:
        GLPostProcessor(const std::string& vertPath, const std::string& fragPath);
        ~GLPostProcessor();

        GLPostProcessor(const GLPostProcessor&) = delete;
        GLPostProcessor& operator=(const GLPostProcessor&) = delete;
        GLPostProcessor(GLPostProcessor&&) = delete;
        GLPostProcessor& operator=(GLPostProcessor&&) = delete;

        // Draws the quad, sampling `input` from texture unit 0.
        void render(const GLTexture& input);

        /**
         * Binds an extra texture for the NEXT render() only, and points `uniformName`
         * at its unit. Cleared after each draw, so effects re-declare their inputs every
         * frame, which is what makes a resized intermediate buffer safe.
         */
        void addInputTexture(const std::string& uniformName, const GLTexture& texture, int unit);

        void setUniform(const std::string& name, const UniformValue& value);

        [[nodiscard]] GLShader& shader() const { return *m_shader; }

    private:
        void initializeQuad();

        struct ExtraTexture {
            int unit;
            uint32_t handle;
        };

        uint32_t m_vao = 0;
        uint32_t m_vbo = 0;
        uint32_t m_ebo = 0;
        std::unique_ptr<GLShader> m_shader;
        std::vector<ExtraTexture> m_extraTextures;
};

}
