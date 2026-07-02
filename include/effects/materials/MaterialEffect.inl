#ifndef SHIMERA_MATERIALEFFECT_INL
#define SHIMERA_MATERIALEFFECT_INL

#include "MaterialEffectBase.hpp"

namespace shimera {

template <typename Derived>
class MaterialEffect : public MaterialEffectBase {
    public:
        void render(IMesh& mesh, const Camera& camera) {
            if (!m_enabled)
                return;
            static_cast<Derived*>(this)->uploadUniforms(*m_material);
            m_backend->renderMaterial(*m_material, mesh, camera, m_transform);
        }

        Derived& with() { return *static_cast<Derived*>(this); }

    protected:
        MaterialEffect(IBackend* backend, const std::string& vert, const std::string& frag) {
            m_backend = backend;
            m_material.reset(backend->createMaterial(vert, frag));
        }
};

}

#endif //SHIMERA_MATERIALEFFECT_INL
