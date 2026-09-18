#pragma once

#include <memory>
#include <string>

#include "GL/GLMesh.hpp"
#include "GL/GLShader.hpp"
#include "scene/Camera.hpp"
#include "scene/Transform.hpp"

namespace shimera {

/**
 * Base of every material, a shader that shades geometry, as opposed to an Effect, which
 * is a shader that reprocesses a finished image.
 *
 * The two share everything below them, so GLShader, GLStateGuard and the host targets, which
 * is the point.
 *
 * Unlike an Effect, a material draws inside the user's frame, between their own draw calls.
 * That makes it the most demanding user of GLStateGuard in the whole library.
 */
class Material {
    public:
        // How the shaded fragments combine with what is already there.
        enum class Blend { None, Alpha, Premultiplied, Additive };

        virtual ~Material() = default;

        Material() = default;
        Material(const Material&) = delete;
        Material& operator=(const Material&) = delete;
        Material(Material&&) = delete;
        Material& operator=(Material&&) = delete;

        [[nodiscard]] virtual std::string getName() const = 0;
        virtual void updateUniforms() = 0;

        void setTransform(const Transform& transform) { m_transform = transform; }
        [[nodiscard]] Transform& getTransform() { return m_transform; }

        void setBlend(const Blend blend) { m_blend = blend; }

        // Closed geometry should cull. Without it the far side of the mesh is shaded too.
        void setCullBackFaces(const bool enabled) { m_cullBackFaces = enabled; }
        void setDepthTest(const bool enabled) { m_depthTest = enabled; }
        void setEnabled(const bool enabled) { m_enabled = enabled; }
        [[nodiscard]] bool isEnabled() const { return m_enabled; }

    protected:
        // Uniforms every material shader declares, so no derived class has to remember them.
        void applySceneUniforms(const Camera& camera) const;
        void applyRenderState() const;

        std::unique_ptr<GLShader> m_shader;
        Transform m_transform;
        Blend m_blend = Blend::Premultiplied;
        bool m_cullBackFaces = true;
        bool m_depthTest = true;
        bool m_enabled = true;
};

}
