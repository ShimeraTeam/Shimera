#include "effects/materials/FresnelEffect.hpp"

using shimera::FresnelEffect;
using shimera::IBackend;
using shimera::IMaterial;
using shimera::Vec3;

FresnelEffect::FresnelEffect(IBackend* backend)
    : MaterialEffect(backend,
        "../../../../res/shader/material/fresnel.vert",
        "../../../../res/shader/material/fresnel.frag") {}

void FresnelEffect::uploadUniforms(IMaterial& material) {
    material.setUniform("u_color", m_color);
    material.setUniform("u_power", m_power);
    material.setUniform("u_reflectance", m_reflectance);
    material.setUniform("u_intensity", m_intensity);
}

FresnelEffect& FresnelEffect::withColor(Vec3<float> c) {
    m_color = c;
    return *this;
}
FresnelEffect& FresnelEffect::withPower(float p) {
    m_power = p;
    return *this;
}
FresnelEffect& FresnelEffect::withReflectance(float r) {
    m_reflectance = r;
    return *this;
}
FresnelEffect& FresnelEffect::withIntensity(float i) {
    m_intensity = i;
    return *this;
}


