#ifndef TEST_MEMORY_SFML_MATERIALEFFECTBASE_HPP
#define TEST_MEMORY_SFML_MATERIALEFFECTBASE_HPP

#include <memory>

#include "shimera_api.h"
#include "backend/IMesh.hpp"
#include "backend/IShader.hpp"
#include "scene/Camera.hpp"

namespace shimera {

class SHIMERA_API MaterialEffectBase {
    public:
        virtual ~MaterialEffectBase() = default;

        virtual void render(IMesh& mesh, const Camera& camera) = 0;
        virtual void updateUniforms(const Camera& camera) = 0;

        MaterialEffectBase() = default;
        MaterialEffectBase(MaterialEffectBase&&) = default;
        MaterialEffectBase& operator=(MaterialEffectBase&&) = default;
        MaterialEffectBase(const MaterialEffectBase&) = delete;
        MaterialEffectBase& operator=(const MaterialEffectBase&) = delete;

        void setEnabled(bool enabled);
        [[nodiscard]] bool isEnabled() const;

        void setModel(const Mat4& model);
        [[nodiscard]] IShader* getShader() const;

        void setTransform(const Vec3<float>& position,
            const Vec3<float>& rotationEuler = Vec3(0.0f), const Vec3<float>& scale = Vec3(1.0f));

    protected:
        std::unique_ptr<IShader> m_shader = nullptr;
        Mat4 m_model = {{1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1}};
        bool m_enabled = true;
};

}

#endif //TEST_MEMORY_SFML_MATERIALEFFECTBASE_HPP
