#include "effects/materials/FresnelEffect.hpp"

shimera::FresnelEffect::FresnelEffect(IBackend* backend)
    : FresnelEffect(backend, Vec3(0.4f, 0.7f, 1.0f), 3.0f, 0.04f, 1.0f) {}

shimera::FresnelEffect::FresnelEffect(IBackend* backend, const Vec3<float> color, const float power,
    const float reflectance, const float intensity)
    : m_uColor(color), m_uPower(power), m_uReflectance(reflectance), m_uIntensity(intensity) {
    m_shader = std::unique_ptr<IShader>(
        backend->createShader(
            "../../../../res/shader/material/fresnel.vert",
            "../../../../res/shader/material/fresnel.frag"
        )
    );
}

void shimera::FresnelEffect::updateUniforms(const Camera& camera) {
    m_shader->setUniform("u_model", m_model);
    m_shader->setUniform("u_view", camera.view);
    m_shader->setUniform("u_projection", camera.projection);
    m_shader->setUniform("u_cameraPos", camera.position);
    m_shader->setUniform("u_color", m_uColor);
    m_shader->setUniform("u_power", m_uPower);
    m_shader->setUniform("u_reflectance", m_uReflectance);
    m_shader->setUniform("u_intensity", m_uIntensity);
}

shimera::FresnelEffect& shimera::FresnelEffect::withColor(const Vec3<float> color) {
    m_uColor = color;
    return *this;
}

shimera::FresnelEffect& shimera::FresnelEffect::withPower(const float power) {
    m_uPower = power;
    return *this;
}

shimera::FresnelEffect& shimera::FresnelEffect::withReflectance(const float reflectance) {
    m_uReflectance = reflectance;
    return *this;
}

shimera::FresnelEffect& shimera::FresnelEffect::withIntensity(const float intensity) {
    m_uIntensity = intensity;
    return *this;
}


