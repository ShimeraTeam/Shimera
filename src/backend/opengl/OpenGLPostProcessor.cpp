#include "backend/opengl/OpenGLPostProcessor.hpp"

#include <GL/glew.h>
#include <glUtils.h>

#include <iostream>
#include <stdexcept>

#include "backend/opengl/OpenGLShader.hpp"

OpenGLPostProcessor::OpenGLPostProcessor()
    : vao(0), vbo(0), ebo(0), shader(nullptr) {
    initializeQuad();
}

OpenGLPostProcessor::~OpenGLPostProcessor() {
    cleanupQuad();
    cleanupShader();
}

void OpenGLPostProcessor::initializeQuad() {
    constexpr float quadVert[] = {
        // positions   // texCoords (uv)
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };

    const unsigned int quadIndices[] = {
        0, 1, 2,
        0, 2, 3
    };

    GLC(glGenVertexArrays(1, &vao));
    GLC(glGenBuffers(1, &vbo));
    GLC(glGenBuffers(1, &ebo));

    GLC(glBindVertexArray(vao));

    GLC(glBindBuffer(GL_ARRAY_BUFFER, vbo));
    GLC(glBufferData(GL_ARRAY_BUFFER, sizeof(quadVert), quadVert, GL_STATIC_DRAW));

    GLC(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo));
    GLC(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), quadIndices, GL_STATIC_DRAW));

    GLC(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0));
    GLC(glEnableVertexAttribArray(0));

    GLC(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float))));
    GLC(glEnableVertexAttribArray(1));

    GLC(glBindVertexArray(0));

    if (vao == 0) {
        throw std::runtime_error("Failed to create VAO for post-processing quad");
    }
}

void OpenGLPostProcessor::cleanupQuad() {
    if (vao != 0) {
        GLC(glDeleteVertexArrays(1, &vao));
        vao = 0;
    }
    if (vbo != 0) {
        GLC(glDeleteBuffers(1, &vbo));
        vbo = 0;
    }
    if (ebo != 0) {
        GLC(glDeleteBuffers(1, &ebo));
        ebo = 0;
    }
}

void OpenGLPostProcessor::cleanupShader() {
    shader.reset();
}

void OpenGLPostProcessor::setShader(const std::string& vert, const std::string& frag) {
    auto newShader = std::make_unique<OpenGLShader>();

    try {
        newShader->loadFromFiles(vert, frag);
        shader = std::move(newShader);
    } catch (const std::exception& e) {
        std::cerr << "Failed to load shader: " << e.what() << std::endl;
        throw;
    }
}

void OpenGLPostProcessor::render(ITexture& texture) {
    if (!shader) {
        std::cerr << "Error: No shader loaded for post-processing!" << std::endl;
        return;
    }

    GLC(glBindVertexArray(0));

    shader->bind();
    GLC(glBindVertexArray(vao));

    GLC(glActiveTexture(GL_TEXTURE0));
    GLC(glBindTexture(GL_TEXTURE_2D, texture.getNativeHandle()));

    GLC(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

    GLC(glBindVertexArray(0));
    shader->unbind();
}

void OpenGLPostProcessor::setUniform(const std::string& name, const UniformValue& value) {
    if (!shader) {
        std::cerr << "Error: No shader loaded, cannot set uniform!" << std::endl;
        return;
    }

    shader->setUniform(name, value);
}

IShader& OpenGLPostProcessor::getShader() {
    if (!shader) {
        throw std::runtime_error("No shader loaded!");
    }
    return *shader;
}

void OpenGLPostProcessor::bindShader() {
    if (!shader) {
        std::cerr << "Error: No shader loaded, cannot bind!" << std::endl;
        return;
    }
    shader->bind();
}
