#include "backend/raylib/RaylibMesh.hpp"

#include <stdexcept>
#include <GL/glew.h>

#include "glUtils.h"


shimera::RaylibMesh::RaylibMesh(Mesh& mesh) {
    upload(mesh);
}

shimera::RaylibMesh::RaylibMesh(Model& model, int meshIndex) {
    if (meshIndex < 0 || meshIndex >= model.meshCount) {
        throw std::out_of_range("meshIndex is out of range");
    }
    upload(model.meshes[meshIndex]);
}

shimera::RaylibMesh::~RaylibMesh() {
    if (m_vao != 0)
        GLC(glDeleteVertexArrays(1, &m_vao));
    if (m_vboPos != 0)
        GLC(glDeleteBuffers(1, &m_vboPos));
    if (m_vboNormal != 0)
        GLC(glDeleteBuffers(1, &m_vboNormal));
    if (m_ebo != 0)
        GLC(glDeleteBuffers(1, &m_ebo));
}

void shimera::RaylibMesh::draw() const {
    GLC(glBindVertexArray(m_vao));
    if (m_indexed) {
        GLC(glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_indexCount),
            GL_UNSIGNED_SHORT, nullptr));
    } else {
        GLC(glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(m_indexCount)));
    }
    GLC(glBindVertexArray(0));
}

unsigned int shimera::RaylibMesh::getIndexCount() const {
    return m_indexCount;
}

void shimera::RaylibMesh::upload(const Mesh& mesh) {
    const int vertexCount = mesh.vertexCount;

    GLC(glGenVertexArrays(1, &m_vao));
    GLC(glBindVertexArray(m_vao));

    // location 0: position
    GLC(glGenBuffers(1, &m_vboPos));
    GLC(glBindBuffer(GL_ARRAY_BUFFER, m_vboPos));
    GLC(glBufferData(GL_ARRAY_BUFFER, vertexCount * 3 * sizeof(float),
        mesh.vertices, GL_STATIC_DRAW));
    GLC(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr));
    GLC(glEnableVertexAttribArray(0));

    // location 1: normal
    if (mesh.normals != nullptr) {
        GLC(glGenBuffers(1, &m_vboNormal));
        GLC(glBindBuffer(GL_ARRAY_BUFFER, m_vboNormal));
        GLC(glBufferData(GL_ARRAY_BUFFER, vertexCount * 3 * sizeof(float),
            mesh.normals, GL_STATIC_DRAW));
        GLC(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr));
        GLC(glEnableVertexAttribArray(1));
    }

    if (mesh.indices != nullptr) {
        m_indexed = true;
        m_indexCount = static_cast<unsigned int>(mesh.triangleCount * 3);
        GLC(glGenBuffers(1, &m_ebo));
        GLC(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo));
        // Raylib indices are unsigned short
        GLC(glBufferData(GL_ELEMENT_ARRAY_BUFFER,
            m_indexCount * sizeof(unsigned short), mesh.indices, GL_STATIC_DRAW));
    } else {
        m_indexed = false;
        m_indexCount = static_cast<unsigned int>(vertexCount);
    }

    GLC(glBindVertexArray(0));
}
