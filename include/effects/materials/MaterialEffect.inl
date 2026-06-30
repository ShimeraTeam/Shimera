#ifndef TEST_MEMORY_SFML_MATERIALEFFECT_INL
#define TEST_MEMORY_SFML_MATERIALEFFECT_INL
#include "MaterialEffectBase.hpp"

namespace shimera {

template <typename Derived>
class MaterIalEffect : public MaterialEffectBase {
    public:
        void render(IMesh& mesh, const Camera& camera) override {
            if (!m_enabled)
                return;
            m_shader->bind();
            static_cast<Derived*>(this)->updateUniforms(camera);
            mesh.draw();
            m_shader->unbind();
        }

        Derived& with() {
            return *static_cast<Derived*>(this);
        }

    private:
        MaterIalEffect() = default;
        friend Derived;
};

}

#endif //TEST_MEMORY_SFML_MATERIALEFFECT_INL
