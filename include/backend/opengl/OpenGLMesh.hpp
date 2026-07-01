#ifndef TEST_MEMORY_SFML_OPENGLMESH_HPP
#define TEST_MEMORY_SFML_OPENGLMESH_HPP

#include <vector>

#include "shimera_api.h"
#include "backend/IMesh.hpp"

namespace shimera {

class SHIMERA_API OpenGLMesh final : public IMesh {
    public:
        OpenGLMesh(const std::vector<float>& positions,
            const std::vector<float>& normals,
            const std::vector<unsigned int>& indices);
        ~OpenGLMesh() override;

        OpenGLMesh(const OpenGLMesh&) = delete;
        OpenGLMesh& operator=(const OpenGLMesh&) = delete;

        void draw() const;
        [[nodiscard]] unsigned int getIndexCount() const;

    private:
        unsigned int m_vao = 0;
        unsigned int m_vboPos = 0;
        unsigned int m_vboNormal = 0;
        unsigned int m_ebo = 0;
        unsigned int m_indexCount = 0;
};

}

#endif //TEST_MEMORY_SFML_OPENGLMESH_HPP
