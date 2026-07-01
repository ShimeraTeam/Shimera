#include "backend/opengl/OpenGLMesh.hpp"

#include <GL/glew.h>

#include "glUtils.h"

using shimera::OpenGLMesh;


OpenGLMesh::OpenGLMesh(const std::vector<float>& positions, const std::vector<float>& normals,
    const std::vector<unsigned int>& indices)
    : m_indexCount(static_cast<unsigned int>(indices.size())) {

    GLC(glGenVertexArrays(1, &m_vao));
    GLC(glBindVertexArray(m_vao));

    // location 0: position (vec3)
    GLC(glGenBuffers(1, &m_vboPos));
    GLC(glBindBuffer(GL_ARRAY_BUFFER, m_vboPos));
    GLC(glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float),
        positions.data(), GL_STATIC_DRAW));
    GLC(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr));
    GLC(glEnableVertexAttribArray(0));

    // location 2: normal (vec3) (to matches Raylib's baked mesh layout, to be changed in the future)
    GLC(glGenBuffers(1, &m_vboNormal));
    GLC(glBindBuffer(GL_ARRAY_BUFFER, m_vboNormal));
    GLC(glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float),
        normals.data(), GL_STATIC_DRAW));
    GLC(glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr));
    GLC(glEnableVertexAttribArray(2));

    GLC(glGenBuffers(1, &m_ebo));
    GLC(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo));
    GLC(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
        indices.data(), GL_STATIC_DRAW));

    GLC(glBindVertexArray(0));
}

OpenGLMesh::~OpenGLMesh() {
    if (m_vao != 0)
        GLC(glDeleteVertexArrays(1, &m_vao));
    if (m_vboPos != 0)
        GLC(glDeleteBuffers(1, &m_vboPos));
    if (m_vboNormal != 0)
        GLC(glDeleteBuffers(1, &m_vboNormal));
    if (m_ebo != 0)
        GLC(glDeleteBuffers(1, &m_ebo));
}

void OpenGLMesh::draw() const {
    GLC(glBindVertexArray(m_vao));
    GLC(glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_indexCount),
        GL_UNSIGNED_INT, nullptr));
    GLC(glBindVertexArray(0));
}

unsigned int OpenGLMesh::getIndexCount() const {
    return m_indexCount;
}
