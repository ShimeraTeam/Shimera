#ifndef SHIMERA_RAYLIBMESH_HPP
#define SHIMERA_RAYLIBMESH_HPP

#include <raylib.h>

#include "backend/IMesh.hpp"

namespace shimera
{

class SHIMERA_API RaylibMesh : public IMesh {
    public:
        explicit RaylibMesh(Model& model);

        [[nodiscard]] Model& model();

    private:
        Model& m_model;
};

}

#endif //SHIMERA_RAYLIBMESH_HPP
