#include "backend/raylib/RaylibMaterial.hpp"

#include "backend/raylib/converts/RaylibConvert.hpp"

using shimera::RaylibMaterial;
using shimera::UniformValue;

RaylibMaterial::RaylibMaterial(const std::string& v, const std::string& f) {
    m_shader = LoadShader(v.c_str(), f.c_str());
    bindShimeraConventions();
}
RaylibMaterial::~RaylibMaterial() {
    UnloadShader(m_shader);
}

void RaylibMaterial::bindShimeraConventions() {
    m_shader.locs[SHADER_LOC_VERTEX_POSITION] = GetShaderLocationAttrib(m_shader, "aPos");
    m_shader.locs[SHADER_LOC_VERTEX_NORMAL] = GetShaderLocationAttrib(m_shader, "aNormal");
    m_shader.locs[SHADER_LOC_MATRIX_MODEL] = GetShaderLocation(m_shader, "u_model");
    m_shader.locs[SHADER_LOC_MATRIX_VIEW] = GetShaderLocation(m_shader, "u_view");
    m_shader.locs[SHADER_LOC_MATRIX_PROJECTION] = GetShaderLocation(m_shader, "u_projection");
}

int RaylibMaterial::locOf(const std::string& name) {
    auto it = m_locs.find(name);
    if (it != m_locs.end()) return it->second;
    int loc = GetShaderLocation(m_shader, name.c_str());
    m_locs[name] = loc;
    return loc;
}

void RaylibMaterial::setUniform(const std::string& name, const UniformValue& value) {
    const int loc = locOf(name);
    if (loc < 0) return;
    std::visit([&](auto&& v) {
        using T = std::decay_t<decltype(v)>;
        if constexpr (std::is_same_v<T, float>)
            SetShaderValue(m_shader, loc, &v, SHADER_UNIFORM_FLOAT);
        else if constexpr (std::is_same_v<T, int>)
            SetShaderValue(m_shader, loc, &v, SHADER_UNIFORM_INT);
        else if constexpr (std::is_same_v<T, Vec2<float>>)
            SetShaderValue(m_shader, loc, &v, SHADER_UNIFORM_VEC2);
        else if constexpr (std::is_same_v<T, Vec3<float>>)
            SetShaderValue(m_shader, loc, &v, SHADER_UNIFORM_VEC3);
        else if constexpr (std::is_same_v<T, Vec4<float>>)
            SetShaderValue(m_shader, loc, &v, SHADER_UNIFORM_VEC4);
        else if constexpr (std::is_same_v<T, Mat4>)
            SetShaderValueMatrix(m_shader, loc, toRaylibMatrix(v));
    }, value);
}

Shader RaylibMaterial::shader() const {
    return m_shader;
}
