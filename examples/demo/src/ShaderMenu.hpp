#ifndef SHIMERA_DEMO_SHADERMENU_HPP
#define SHIMERA_DEMO_SHADERMENU_HPP

#include <cstddef>
#include <functional>
#include <string>
#include <variant>
#include <vector>

#include "effects/ShaderEffectBase.hpp"
#include "uniform/Vec3.inl"

namespace demo {

// Two immediate-mode panels on the right of the window, hidden with [M]:
//  - the shader list: one checkbox per shader; a right click selects the
//    shader whose parameters are shown in the second panel;
//  - the parameters panel: one row per parameter of the selected shader,
//    sliders for numbers, a checkbox for booleans, three inputs for Vec3.
// Both scroll with the wheel when their content is taller than the window.
// Nothing here touches the library beyond ShaderEffectBase::setEnabled() and
// the effects' own withX() setters wrapped in ShaderParam::apply.

struct ShaderParam {
    using Value = std::variant<float, int, bool, shimera::Vec3<float>>;

    std::string name;
    Value value;
    float min = 0.0f; // slider range, numbers only
    float max = 1.0f;
    std::function<void(const ShaderParam &)> apply; // pushes `value` to the effect

    static ShaderParam number(std::string name, float value, float min, float max, std::function<void(float)> set);
    static ShaderParam integer(std::string name, int value, int min, int max, std::function<void(int)> set);
    static ShaderParam boolean(std::string name, bool value, std::function<void(bool)> set);
    static ShaderParam vec3(std::string name, shimera::Vec3<float> value,
                            std::function<void(shimera::Vec3<float>)> set);
};

struct ShaderToggle {
    std::string name;
    bool enabled = false;
    shimera::ShaderEffectBase *effect = nullptr; // nullptr: not a pipeline effect
    std::vector<ShaderParam> params;
};

struct ShaderMenu {
    static constexpr std::size_t kNone = static_cast<std::size_t>(-1);

    bool visible = true;
    float scroll = 0.0f;        // shader list
    float paramScroll = 0.0f;   // parameters panel
    std::size_t selected = kNone;

    // Widget being interacted with in the parameters panel.
    std::size_t draggedSlider = kNone;
    std::size_t focusedField = kNone; // param index * 3 + component (x, y, z)
    std::string editBuffer;
};

// True while the cursor is above one of the panels: the caller uses it to
// keep the wheel from also zooming the camera.
[[nodiscard]] bool isMouseOverMenu(const ShaderMenu &menu, const std::vector<ShaderToggle> &toggles);

// Handles [M], scrolling, checkboxes, selection and parameter edits.
// `allowInput` is false while the free camera owns the (hidden) cursor.
void updateShaderMenu(ShaderMenu &menu, std::vector<ShaderToggle> &toggles, bool allowInput);

void drawShaderMenu(const ShaderMenu &menu, const std::vector<ShaderToggle> &toggles, bool allowInput);

}

#endif //SHIMERA_DEMO_SHADERMENU_HPP
