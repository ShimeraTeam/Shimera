#ifndef TEST_MEMORY_SFML_RAYLIBMESH_HPP
#define TEST_MEMORY_SFML_RAYLIBMESH_HPP

#include <raylib.h>

#include "backend/IMesh.hpp"

namespace shimera
{

class RaylibMesh : public IMesh {
    public:
        explicit RaylibMesh(Mesh& mesh);
        explicit RaylibMesh(Model& model, int meshIndex = 0);
        ~RaylibMesh() override;

        RaylibMesh(const RaylibMesh&) = delete;
        RaylibMesh& operator=(const RaylibMesh&) = delete;

        void draw() const override;
        [[nodiscard]] unsigned int getIndexCount() const override;

    private:
        void upload(const Mesh& mesh);

        unsigned int m_vao = 0;
        unsigned int m_vboPos = 0;
        unsigned int m_vboNormal = 0;
        unsigned int m_ebo = 0;
        unsigned int m_indexCount = 0;
        bool m_indexed = false;
};

}

#endif //TEST_MEMORY_SFML_RAYLIBMESH_HPP
