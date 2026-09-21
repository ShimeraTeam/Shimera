#include "Material.hpp"

#include <GL/glew.h>

#include "GL/GLError.hpp"

using shimera::Material;

void Material::applySceneUniforms(const Camera& camera) const {
    m_shader->setUniform("u_model", m_transform.toMatrix());
    m_shader->setUniform("u_view", camera.view);
    m_shader->setUniform("u_projection", camera.projection);
    m_shader->setUniform("u_cameraPos", camera.position);
}

void Material::applyRenderState() const {
    if (m_depthTest) {
        GLC(glEnable(GL_DEPTH_TEST));
    } else {
        GLC(glDisable(GL_DEPTH_TEST));
    }

    if (m_cullBackFaces) {
        GLC(glEnable(GL_CULL_FACE));
        GLC(glCullFace(GL_BACK));
        GLC(glFrontFace(GL_CCW));
    } else {
        GLC(glDisable(GL_CULL_FACE));
    }

    if (m_blend == Blend::None) {
        GLC(glDisable(GL_BLEND));
        return;
    }

    GLC(glEnable(GL_BLEND));
    switch (m_blend) {
        case Blend::Alpha:
            GLC(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
            break;
        case Blend::Additive:
            GLC(glBlendFunc(GL_ONE, GL_ONE));
            break;
        case Blend::Premultiplied:
        default:
            GLC(glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA));
            break;
    }
}
