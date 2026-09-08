#ifndef SHIMERA_DEMO_SHADERSHOWCASE_HPP
#define SHIMERA_DEMO_SHADERSHOWCASE_HPP

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

#include "raylib.h"

#include "shimera.h"
#include "EffectPipeline.inl"
#include "backend/IBackend.hpp"
#include "backend/raylib/RaylibMesh.hpp"
#include "effects/materials/FresnelEffect.hpp"

#include "ShaderMenu.hpp"

namespace demo {

// Owns everything Shimera-related in the demo: the effects, the pipeline they
// are chained in, the Fresnel material and the passthrough shader used when no
// effect is active. See the constructor in ShaderShowcase.cpp to pick which
// effects the demo runs.
class ShaderShowcase {
    public:
        explicit ShaderShowcase(shimera::IBackend *backend);
        ~ShaderShowcase();

        ShaderShowcase(const ShaderShowcase &) = delete;
        ShaderShowcase &operator=(const ShaderShowcase &) = delete;

        // Checkbox list, empty when the menu is off (kUseShaderMenu == false).
        std::vector<ShaderToggle> &toggles() { return m_toggles; }

        // Pushes the checkbox states onto the effects. Call once per frame.
        void applyToggles();

        // Feeds the per-frame uniforms (time, camera) to the effects that need them.
        void updateUniforms(float time, const Camera &camera);

        // 3D material pass, between BeginMode3D() and EndMode3D().
        void renderMaterials(const shimera::Camera &camera);

        // Post-processing pass over the rendered scene.
        void render(shimera::IFrameBuffer &sceneFb);

        [[nodiscard]] std::size_t activeEffectCount() const;
        [[nodiscard]] std::string activeEffectNames() const;

    private:
        shimera::IBackend *m_backend;
        shimera::EffectPipeline m_pipeline;
        std::vector<ShaderToggle> m_toggles;

        // Declaration order matters: RaylibMesh keeps a reference to the model.
        Model m_fresnelModel;
        shimera::RaylibMesh m_fresnelMesh;
        shimera::FresnelEffect m_fresnel;
        std::unique_ptr<shimera::IPostProcessor> m_passthrough;

        bool m_fresnelEnabled = true;
        std::size_t m_fresnelToggleIndex = 0;

        // Puts `effect` in the pipeline (disabled) and adds its checkbox and
        // editable parameters. Defined in ShaderShowcase.cpp next to the
        // per-effect parameter lists.
        template<typename TEffect>
        void addToggleableEffect(TEffect &&effect, std::string label);
};

}

#endif //SHIMERA_DEMO_SHADERSHOWCASE_HPP
