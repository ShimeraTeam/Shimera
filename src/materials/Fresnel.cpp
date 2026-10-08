#include "Fresnel.hpp"

#include "Resources.inl"

using shimera::Fresnel;
using shimera::Vec3;

Fresnel::Fresnel(const Vec3<float> color, const float power,
                 const float reflectance, const float intensity)
    : m_color(color), m_power(power), m_reflectance(reflectance), m_intensity(intensity) {
    m_shader = GLShader::fromFiles(materialShader("fresnel.vert"), materialShader("fresnel.frag"));
}

void Fresnel::updateUniforms() {
    m_shader->setUniform("u_color", m_color);
    m_shader->setUniform("u_power", m_power);
    m_shader->setUniform("u_reflectance", m_reflectance);
    m_shader->setUniform("u_intensity", m_intensity);
}

Fresnel& Fresnel::withColor(const Vec3<float> color) {
    m_color = color;
    return *this;
}

Fresnel& Fresnel::withPower(const float power) {
    m_power = power;
    return *this;
}

Fresnel& Fresnel::withReflectance(const float reflectance) {
    m_reflectance = reflectance;
    return *this;
}

Fresnel& Fresnel::withIntensity(const float intensity) {
    m_intensity = intensity;
    return *this;
}
