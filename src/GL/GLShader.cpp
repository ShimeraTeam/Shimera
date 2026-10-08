#include "GLShader.hpp"

#include <GL/glew.h>
#include "GL/GLError.hpp"
#include "GL/GLStateGuard.hpp"

#include <iostream>
#include <optional>
#include <stdexcept>
#include <variant>
#include <vector>

#include "common/File.hpp"
#include "uniform/Vec2.inl"
#include "uniform/Vec3.inl"
#include "uniform/Vec4.inl"
#include "uniform/Mat4.hpp"

using shimera::GLShader;

namespace {

std::string stageName(const uint32_t stage) {
    return stage == GL_VERTEX_SHADER ? "vertex" : "fragment";
}

std::string infoLog(const uint32_t object, const bool isProgram) {
    int length = 0;
    if (isProgram) {
        GLC(glGetProgramiv(object, GL_INFO_LOG_LENGTH, &length));
    } else {
        GLC(glGetShaderiv(object, GL_INFO_LOG_LENGTH, &length));
    }
    if (length <= 0)
        return "<no info log>";

    std::vector<char> buffer(static_cast<size_t>(length));
    if (isProgram) {
        GLC(glGetProgramInfoLog(object, length, &length, buffer.data()));
    } else {
        GLC(glGetShaderInfoLog(object, length, &length, buffer.data()));
    }
    return std::string(buffer.data(), static_cast<size_t>(length));
}

} // namespace

std::unique_ptr<GLShader> GLShader::fromFiles(const std::string& vertPath, const std::string& fragPath) {
    // Name the file that is actually missing, rather than letting an empty source turn
    // into a baffling compile error further down.
    const std::optional<std::string> vert = readTextFile(vertPath);
    if (!vert)
        throw std::runtime_error("shimera: cannot read vertex shader '" + vertPath + "'");

    const std::optional<std::string> frag = readTextFile(fragPath);
    if (!frag)
        throw std::runtime_error("shimera: cannot read fragment shader '" + fragPath + "'");

    return std::unique_ptr<GLShader>(new GLShader(*vert, *frag, fragPath));
}

std::unique_ptr<GLShader> GLShader::fromMemory(const std::string_view vertSource,
                                               const std::string_view fragSource,
                                               std::string name) {
    return std::unique_ptr<GLShader>(new GLShader(vertSource, fragSource, std::move(name)));
}

GLShader::GLShader(const std::string_view vertSource, const std::string_view fragSource, std::string name)
    : m_name(std::move(name)) {
    /* Linking ends with this program bound (the sampler unit is set through it). The host
     * caches which program it believes is current, so creation restores theirs on exit. */
    const GLStateGuard guard;

    const uint32_t vert = compile(GL_VERTEX_SHADER, vertSource, m_name);
    uint32_t frag = 0;
    try {
        frag = compile(GL_FRAGMENT_SHADER, fragSource, m_name);
    } catch (...) {
        GLC(glDeleteShader(vert));
        throw;
    }

    GLC(m_program = glCreateProgram());
    GLC(glAttachShader(m_program, vert));
    GLC(glAttachShader(m_program, frag));
    GLC(glLinkProgram(m_program));

    // Detach and delete regardless of outcome, the program keeps what it needs.
    GLC(glDetachShader(m_program, vert));
    GLC(glDetachShader(m_program, frag));
    GLC(glDeleteShader(vert));
    GLC(glDeleteShader(frag));

    int linked = GL_FALSE;
    GLC(glGetProgramiv(m_program, GL_LINK_STATUS, &linked));
    if (linked == GL_FALSE) {
        const std::string log = infoLog(m_program, true);
        GLC(glDeleteProgram(m_program));
        m_program = 0;
        throw std::runtime_error("shimera: failed to link shader '" + m_name + "':\n" + log);
    }

    m_blocks = UniformBlock::discoverAll(m_program);

    /* Every post-processing shader samples the scene from unit 0. Setting it once here
    * means effects never have to remember to. */
    bind();
    if (const int location = uniformLocation("u_screenTexture"); location != -1) {
        GLC(glUniform1i(location, 0));
    }
}

GLShader::~GLShader() {
    if (m_program != 0) {
        glDeleteProgram(m_program);
    }
}

uint32_t GLShader::compile(const uint32_t stage, const std::string_view source, const std::string& name) {
    GLC(const uint32_t id = glCreateShader(stage));

    const char* data = source.data();
    const auto length = static_cast<int>(source.size());
    GLC(glShaderSource(id, 1, &data, &length));
    GLC(glCompileShader(id));

    int compiled = GL_FALSE;
    GLC(glGetShaderiv(id, GL_COMPILE_STATUS, &compiled));
    if (compiled == GL_FALSE) {
        const std::string log = infoLog(id, false);
        GLC(glDeleteShader(id));
        throw std::runtime_error(
            "shimera: failed to compile " + stageName(stage) + " shader '" + name + "':\n" + log);
    }
    return id;
}

void GLShader::bind() {
    GLC(glUseProgram(m_program));
    // Block only allow reupload if dirty (changed), so it's fine to bind every blocks each frames
    for (UniformBlock& block : m_blocks) {
        block.bind();
    }
}

int GLShader::uniformLocation(const std::string& uniform) {
    if (const auto it = m_uniformCache.find(uniform); it != m_uniformCache.end()) {
        return it->second;
    }
    GLC(const int location = glGetUniformLocation(m_program, uniform.c_str()));
    m_uniformCache[uniform] = location;
    return location;
}

void GLShader::setUniform(const std::string& name, const UniformValue& value) {
    // if uniform found in a block
    for (UniformBlock& block : m_blocks) {
        if (block.write(name, value))
            return;
    }
    // else its a normal uniform (probably from a handwritten shader)
    const int location = uniformLocation(name);
    if (location == -1) {
        std::cerr << "shimera: uniform '" << name << "' not found in shader '" << m_name << "'\n";
        return;
    }

    bind();

    // Lambda replaced by explicit template parameter (`auto&& val` was replaced by `<typename T0>(T0&& val)`)
    std::visit([location]<typename T0>(T0&& val) {
        using T = std::decay_t<T0>;
        if constexpr (std::is_same_v<T, float>) {
            GLC(glUniform1f(location, val));
        } else if constexpr (std::is_same_v<T, int>) {
            GLC(glUniform1i(location, val));
        } else if constexpr (std::is_same_v<T, Vec2<float>>) {
            GLC(glUniform2f(location, val.x, val.y));
        } else if constexpr (std::is_same_v<T, Vec3<float>>) {
            GLC(glUniform3f(location, val.x, val.y, val.z));
        } else if constexpr (std::is_same_v<T, Vec4<float>>) {
            GLC(glUniform4f(location, val.x, val.y, val.z, val.w));
        } else if constexpr (std::is_same_v<T, Mat4>) {
            GLC(glUniformMatrix4fv(location, 1, GL_FALSE, val.m));
        }
    }, value);
}
