//
// Created by leoma on 24/11/2025.
//

#include "PostProcessingQuad.h"

#include <iostream>
#include <GL/glew.h>

#include "glUtils.h"


PostProcessingQuad::PostProcessingQuad(const std::string& vertPath,
                                       const std::string& fragPath) {
    float quadVert[] = {
        // positions   // texCoords (uv)
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };

    unsigned int quadIndices[] = {
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
    GLC(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices),
        quadIndices, GL_STATIC_DRAW));

    GLC(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
        4 * sizeof(float), (void*)0));
    GLC(glEnableVertexAttribArray(0));

    GLC(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
        4 * sizeof(float), (void*)(2 * sizeof(float))));
    GLC(glEnableVertexAttribArray(1));

    GLC(glBindVertexArray(0));

    ShaderProgramSource source = parseShader(vertPath, fragPath);
    shader = createShader(source.vertex, source.fragment);
    if (shader == 0) {
        std::cerr << "Failed to create post-processing shader!" << std::endl;
        return;
    }

    GLC(glUseProgram(shader));
    GLC(int location = glGetUniformLocation(shader, "u_screenTexture"));
    ASSERT(location != -1);
    GLC(glUniform1i(location, 0));
}

PostProcessingQuad::~PostProcessingQuad() {
    GLC(glDeleteVertexArrays(1, &vao));
    GLC(glDeleteBuffers(1, &vbo));
    GLC(glDeleteBuffers(1, &ebo));
    GLC(glDeleteProgram(shader));
}

void PostProcessingQuad::render(unsigned int texture) const {
    GLC(glUseProgram(shader));
    GLC(glBindVertexArray(vao));

    GLC(glActiveTexture(GL_TEXTURE0));
    GLC(glBindTexture(GL_TEXTURE_2D, texture));

    GLC(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
    GLC(glBindVertexArray(0));
}

unsigned int PostProcessingQuad::getShader() const {
    return shader;
}

void PostProcessingQuad::bindShader() const {
    GLC(glUseProgram(shader));
}
