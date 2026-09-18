#pragma once

#include "GL/GLStateGuard.hpp"
#include "Material.hpp"

namespace shimera {

/**
 * CRTP helper, a material only has to write updateUniforms() and its parameters, exactly
 * like ShaderEffect does for the post-processing side.
 */
template<typename Derived>
class ShaderMaterial : public Material {
    public:
        // Draws into whatever framebuffer is bound, typically the user's scene target.
        void render(const GLMesh& mesh, const Camera& camera) {
            if (!m_enabled)
                return;

            const GLStateGuard guard;

            m_shader->bind();
            applySceneUniforms(camera);
            static_cast<Derived*>(this)->updateUniforms();
            applyRenderState();
            mesh.draw();
        }

        Derived& with() { return *static_cast<Derived*>(this); }

    private:
        ShaderMaterial() = default;
        friend Derived;
};

}
