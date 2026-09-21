#include "GLMesh.hpp"

#include <GL/glew.h>

#include <stdexcept>
#include <string>

#include "GL/GLError.hpp"
#include "GL/GLStateGuard.hpp"

using shimera::GLMesh;

GLMesh::GLMesh(const std::vector<float>& positions,
               const std::vector<float>& normals,
               const std::vector<unsigned int>& indices)
    : m_indexCount(static_cast<unsigned int>(indices.size())) {
    if (positions.empty() || indices.empty())
        throw std::runtime_error("shimera: GLMesh needs at least one triangle");
    if (positions.size() != normals.size())
        throw std::runtime_error("shimera: GLMesh got " + std::to_string(positions.size() / 3)
            + " positions but " + std::to_string(normals.size() / 3) + " normals");

    // Creation must leave the host's bindings as it found them, same as the render path.
    const GLStateGuard guard;

    GLC(glGenVertexArrays(1, &m_vao));
    GLC(glBindVertexArray(m_vao));

    GLC(glGenBuffers(1, &m_positionBuffer));
    GLC(glBindBuffer(GL_ARRAY_BUFFER, m_positionBuffer));
    GLC(glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(positions.size() * sizeof(float)),
        positions.data(), GL_STATIC_DRAW));
    GLC(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr));
    GLC(glEnableVertexAttribArray(0));

    GLC(glGenBuffers(1, &m_normalBuffer));
    GLC(glBindBuffer(GL_ARRAY_BUFFER, m_normalBuffer));
    GLC(glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(normals.size() * sizeof(float)),
        normals.data(), GL_STATIC_DRAW));
    GLC(glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr));
    GLC(glEnableVertexAttribArray(2));

    GLC(glGenBuffers(1, &m_indexBuffer));
    GLC(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer));
    GLC(glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(indices.size() * sizeof(unsigned int)),
        indices.data(), GL_STATIC_DRAW));

    // Unbinding the VAO here records its contents rather than clobbering host state.
    GLC(glBindVertexArray(0));
}

GLMesh::~GLMesh() {
    if (m_vao != 0) glDeleteVertexArrays(1, &m_vao);
    if (m_positionBuffer != 0) glDeleteBuffers(1, &m_positionBuffer);
    if (m_normalBuffer != 0) glDeleteBuffers(1, &m_normalBuffer);
    if (m_indexBuffer != 0) glDeleteBuffers(1, &m_indexBuffer);
}

void GLMesh::draw() const {
    GLC(glBindVertexArray(m_vao));
    GLC(glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_indexCount), GL_UNSIGNED_INT, nullptr));
}
