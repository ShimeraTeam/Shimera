#ifndef TEST_MEMORY_SFML_MATERIALEFFECTBASE_HPP
#define TEST_MEMORY_SFML_MATERIALEFFECTBASE_HPP

#include <memory>

#include "shimera_api.h"
#include "backend/IBackend.hpp"
#include "scene/Camera.hpp"

namespace shimera {

class SHIMERA_API MaterialEffectBase {
    public:
        virtual ~MaterialEffectBase() = default;

        void setEnabled(bool enabled);
        [[nodiscard]] bool isEnabled() const;

        void setTransform(const Vec3<float>& position,
                          const Vec3<float>& rotationEuler = Vec3(0.0f),
                          const Vec3<float>& scale = Vec3(1.0f));

    protected:
        IBackend* m_backend = nullptr;
        std::unique_ptr<IMaterial> m_material;
        Mat4 m_transform{{1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1}};
        bool m_enabled = true;
};

}

#endif //TEST_MEMORY_SFML_MATERIALEFFECTBASE_HPP
