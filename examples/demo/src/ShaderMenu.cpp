#include "ShaderMenu.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdlib>
#include <utility>

#include "raylib.h"

#include "DemoConfig.hpp"

namespace demo {

// --- Parameter helpers -------------------------------------------------------

ShaderParam ShaderParam::number(std::string name, float value, float min, float max,
                                std::function<void(float)> set)
{
    ShaderParam p;
    p.name = std::move(name);
    p.value = value;
    p.min = min;
    p.max = max;
    p.apply = [set = std::move(set)](const ShaderParam &self) { set(std::get<float>(self.value)); };
    return p;
}

ShaderParam ShaderParam::integer(std::string name, int value, int min, int max, std::function<void(int)> set)
{
    ShaderParam p;
    p.name = std::move(name);
    p.value = value;
    p.min = static_cast<float>(min);
    p.max = static_cast<float>(max);
    p.apply = [set = std::move(set)](const ShaderParam &self) { set(std::get<int>(self.value)); };
    return p;
}

ShaderParam ShaderParam::boolean(std::string name, bool value, std::function<void(bool)> set)
{
    ShaderParam p;
    p.name = std::move(name);
    p.value = value;
    p.apply = [set = std::move(set)](const ShaderParam &self) { set(std::get<bool>(self.value)); };
    return p;
}

ShaderParam ShaderParam::vec3(std::string name, shimera::Vec3<float> value,
                              std::function<void(shimera::Vec3<float>)> set)
{
    ShaderParam p;
    p.name = std::move(name);
    p.value = value;
    p.apply = [set = std::move(set)](const ShaderParam &self) {
        set(std::get<shimera::Vec3<float>>(self.value));
    };
    return p;
}

namespace {

// --- Layout ------------------------------------------------------------------

constexpr float kPanelWidth = 260.0f;
constexpr float kMargin = 12.0f;
constexpr float kPanelGap = 10.0f;
constexpr float kHeaderHeight = 36.0f;
constexpr float kPadding = 8.0f;
constexpr float kListRowHeight = 28.0f;
constexpr float kParamRowHeight = 42.0f;
constexpr float kScrollStep = 28.0f;
constexpr float kScrollbarRoom = 8.0f;
// Share of the window height the shader list may take; the rest goes to the
// parameters panel below it.
constexpr float kListShare = 0.55f;

constexpr Color kAccent{120, 230, 255, 255};
constexpr Color kSelection{150, 90, 230, 255};
constexpr Color kDimText{165, 165, 172, 255};

struct Panel {
    Rectangle box;   // whole panel, header included
    Rectangle view;  // scrollable rows area
    float rowsHeight = 0.0f;
    float maxScroll = 0.0f;
};

struct MenuLayout {
    Panel list;
    Panel params;
};

Panel panelLayout(float y, std::size_t rowCount, float rowHeight, float maxHeight)
{
    const float rowsHeight = static_cast<float>(rowCount) * rowHeight;
    const float viewHeight = std::max(0.0f, std::min(rowsHeight, maxHeight - kHeaderHeight - kPadding));
    const float x = kWidth - kPanelWidth - kMargin;

    Panel panel;
    panel.box = Rectangle{x, y, kPanelWidth, kHeaderHeight + viewHeight + kPadding};
    panel.view = Rectangle{x, y + kHeaderHeight, kPanelWidth, viewHeight};
    panel.rowsHeight = rowsHeight;
    panel.maxScroll = std::max(0.0f, rowsHeight - viewHeight);
    return panel;
}

const std::vector<ShaderParam> *selectedParams(const ShaderMenu &menu, const std::vector<ShaderToggle> &toggles)
{
    if (menu.selected >= toggles.size())
        return nullptr;
    return &toggles[menu.selected].params;
}

MenuLayout menuLayout(const ShaderMenu &menu, const std::vector<ShaderToggle> &toggles)
{
    const float available = kHeight - 2.0f * kMargin;
    const auto *params = selectedParams(menu, toggles);
    // One row is kept for the "right click a shader" hint when nothing is selected.
    const std::size_t paramRows = params ? std::max<std::size_t>(params->size(), 1) : 1;

    MenuLayout layout;
    layout.list = panelLayout(kMargin, toggles.size(), kListRowHeight, available * kListShare);
    layout.params = panelLayout(layout.list.box.y + layout.list.box.height + kPanelGap, paramRows,
                                kParamRowHeight, available - layout.list.box.height - kPanelGap);
    return layout;
}

Rectangle rowRect(const Panel &panel, float scroll, std::size_t index, float rowHeight)
{
    return Rectangle{panel.view.x, panel.view.y - scroll + static_cast<float>(index) * rowHeight,
                     panel.view.width, rowHeight};
}

bool rowVisible(const Panel &panel, const Rectangle &row)
{
    return row.y + row.height >= panel.view.y && row.y <= panel.view.y + panel.view.height;
}

// Widgets inside a parameter row. All of them leave room for the scrollbar.
float rowInnerWidth(const Rectangle &row) { return row.width - 24.0f - kScrollbarRoom; }

Rectangle sliderTrack(const Rectangle &row)
{
    return Rectangle{row.x + 12.0f, row.y + 29.0f, rowInnerWidth(row), 5.0f};
}

Rectangle sliderHitBox(const Rectangle &row)
{
    return Rectangle{row.x + 6.0f, row.y + 20.0f, rowInnerWidth(row) + 12.0f, 22.0f};
}

Rectangle checkBox(const Rectangle &row) { return Rectangle{row.x + 12.0f, row.y + 13.0f, 16.0f, 16.0f}; }

std::array<Rectangle, 3> vec3Fields(const Rectangle &row)
{
    constexpr float gap = 4.0f;
    const float w = (rowInnerWidth(row) - 2.0f * gap) / 3.0f;
    std::array<Rectangle, 3> fields{};
    for (std::size_t k = 0; k < 3; ++k)
        fields[k] = Rectangle{row.x + 12.0f + static_cast<float>(k) * (w + gap), row.y + 21.0f, w, 18.0f};
    return fields;
}

// --- Value helpers -----------------------------------------------------------

bool isNumber(const ShaderParam &p)
{
    return std::holds_alternative<float>(p.value) || std::holds_alternative<int>(p.value);
}

float numberAsFloat(const ShaderParam &p)
{
    return std::holds_alternative<int>(p.value) ? static_cast<float>(std::get<int>(p.value))
                                                : std::get<float>(p.value);
}

// Slider position in [0, 1] -> parameter value, rounded for integers.
void setNumberFromRatio(ShaderParam &p, float ratio)
{
    const float v = p.min + std::clamp(ratio, 0.0f, 1.0f) * (p.max - p.min);
    if (std::holds_alternative<int>(p.value))
        p.value = static_cast<int>(std::lround(v));
    else
        p.value = v;
    p.apply(p);
}

std::string formatNumber(const ShaderParam &p)
{
    if (std::holds_alternative<int>(p.value))
        return TextFormat("%d", std::get<int>(p.value));
    // Small ranges deserve one more decimal to read slider moves.
    return TextFormat((p.max - p.min) <= 1.0f ? "%.3f" : "%.2f", std::get<float>(p.value));
}

float &vec3Component(shimera::Vec3<float> &v, std::size_t k)
{
    return k == 0 ? v.x : (k == 1 ? v.y : v.z);
}

// --- Text field editing (Vec3 components) ------------------------------------

void focusField(ShaderMenu &menu, const ShaderParam &p, std::size_t paramIndex, std::size_t component)
{
    menu.focusedField = paramIndex * 3 + component;
    auto v = std::get<shimera::Vec3<float>>(p.value);
    menu.editBuffer = TextFormat("%g", vec3Component(v, component));
}

// Parses the buffer into the focused component. An unreadable buffer keeps the
// previous value.
void commitField(ShaderMenu &menu, std::vector<ShaderParam> *params)
{
    if (menu.focusedField == ShaderMenu::kNone)
        return;
    const std::size_t paramIndex = menu.focusedField / 3;
    const std::size_t component = menu.focusedField % 3;
    menu.focusedField = ShaderMenu::kNone;

    if (!params || paramIndex >= params->size())
        return;
    ShaderParam &p = (*params)[paramIndex];
    if (!std::holds_alternative<shimera::Vec3<float>>(p.value))
        return;

    char *end = nullptr;
    const float parsed = std::strtof(menu.editBuffer.c_str(), &end);
    if (end == menu.editBuffer.c_str() || *end != 0)
        return;

    auto v = std::get<shimera::Vec3<float>>(p.value);
    vec3Component(v, component) = parsed;
    p.value = v;
    p.apply(p);
}

void typeIntoField(ShaderMenu &menu, std::vector<ShaderParam> *params)
{
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER)) {
        commitField(menu, params);
        return;
    }
    if (IsKeyPressed(KEY_ESCAPE)) {
        menu.focusedField = ShaderMenu::kNone;
        return;
    }
    if (IsKeyPressed(KEY_BACKSPACE) && !menu.editBuffer.empty())
        menu.editBuffer.pop_back();

    for (int c = GetCharPressed(); c != 0; c = GetCharPressed()) {
        if (c == ',')
            c = '.';
        const bool digit = c >= '0' && c <= '9';
        if ((digit || c == '.' || c == '-' || c == 'e') && menu.editBuffer.size() < 12)
            menu.editBuffer.push_back(static_cast<char>(c));
    }
}

// --- Input -------------------------------------------------------------------

void updateList(ShaderMenu &menu, std::vector<ShaderToggle> &toggles, const Panel &panel, Vector2 mouse)
{
    const bool left = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    const bool right = IsMouseButtonPressed(MOUSE_BUTTON_RIGHT);
    if ((!left && !right) || !CheckCollisionPointRec(mouse, panel.view))
        return;

    for (std::size_t i = 0; i < toggles.size(); ++i) {
        if (!CheckCollisionPointRec(mouse, rowRect(panel, menu.scroll, i, kListRowHeight)))
            continue;
        if (left) {
            toggles[i].enabled = !toggles[i].enabled;
        } else {
            // Right click selects the shader to edit; again on it deselects.
            commitField(menu, selectedParams(menu, toggles) ? &toggles[menu.selected].params : nullptr);
            menu.selected = (menu.selected == i) ? ShaderMenu::kNone : i;
            menu.paramScroll = 0.0f;
            menu.draggedSlider = ShaderMenu::kNone;
        }
        return;
    }
}

void updateParams(ShaderMenu &menu, std::vector<ShaderParam> &params, const Panel &panel, Vector2 mouse)
{
    // A slider grabbed earlier follows the mouse until the button is released,
    // even outside the panel.
    if (menu.draggedSlider != ShaderMenu::kNone) {
        if (!IsMouseButtonDown(MOUSE_BUTTON_LEFT) || menu.draggedSlider >= params.size()) {
            menu.draggedSlider = ShaderMenu::kNone;
        } else {
            const Rectangle track = sliderTrack(rowRect(panel, menu.paramScroll, menu.draggedSlider, kParamRowHeight));
            setNumberFromRatio(params[menu.draggedSlider], (mouse.x - track.x) / track.width);
        }
    }

    if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        return;

    // Any click ends the current text edit; the click is then processed normally.
    commitField(menu, &params);

    if (!CheckCollisionPointRec(mouse, panel.view))
        return;

    for (std::size_t i = 0; i < params.size(); ++i) {
        const Rectangle row = rowRect(panel, menu.paramScroll, i, kParamRowHeight);
        if (!CheckCollisionPointRec(mouse, row))
            continue;
        ShaderParam &p = params[i];

        if (isNumber(p)) {
            if (CheckCollisionPointRec(mouse, sliderHitBox(row))) {
                menu.draggedSlider = i;
                const Rectangle track = sliderTrack(row);
                setNumberFromRatio(p, (mouse.x - track.x) / track.width);
            }
        } else if (std::holds_alternative<bool>(p.value)) {
            p.value = !std::get<bool>(p.value);
            p.apply(p);
        } else {
            const auto fields = vec3Fields(row);
            for (std::size_t k = 0; k < 3; ++k) {
                if (CheckCollisionPointRec(mouse, fields[k]))
                    focusField(menu, p, i, k);
            }
        }
        return;
    }
}

// --- Drawing -----------------------------------------------------------------

void drawPanelFrame(const Panel &panel, const char *title, const char *hint)
{
    DrawRectangleRec(panel.box, Fade(BLACK, 0.65f));
    DrawRectangleLinesEx(panel.box, 1.0f, Fade(RAYWHITE, 0.15f));
    DrawText(title, static_cast<int>(panel.box.x) + 12, static_cast<int>(panel.box.y) + 11, 16, RAYWHITE);
    if (hint) {
        DrawText(hint, static_cast<int>(panel.box.x + panel.box.width) - 12 - MeasureText(hint, 13),
                 static_cast<int>(panel.box.y) + 13, 13, GRAY);
    }
    DrawRectangle(static_cast<int>(panel.view.x) + 1, static_cast<int>(panel.view.y),
                  static_cast<int>(panel.view.width) - 2, 1, Fade(RAYWHITE, 0.15f));
}

void drawScrollbar(const Panel &panel, float scroll)
{
    if (panel.maxScroll <= 0.0f)
        return;
    const float trackX = panel.view.x + panel.view.width - 6.0f;
    const float thumbHeight = std::max(24.0f, panel.view.height * panel.view.height / panel.rowsHeight);
    const float thumbY = panel.view.y + (panel.view.height - thumbHeight) * (scroll / panel.maxScroll);
    DrawRectangleRec(Rectangle{trackX, panel.view.y, 3.0f, panel.view.height}, Fade(RAYWHITE, 0.10f));
    DrawRectangleRec(Rectangle{trackX, thumbY, 3.0f, thumbHeight}, Fade(RAYWHITE, 0.45f));
}

void drawCheck(const Rectangle &box, bool on)
{
    DrawRectangleRec(box, on ? kAccent : Fade(RAYWHITE, 0.08f));
    DrawRectangleLinesEx(box, 1.0f, Fade(RAYWHITE, 0.35f));
    if (on) {
        DrawLineEx(Vector2{box.x + 3.5f, box.y + 8.5f}, Vector2{box.x + 6.5f, box.y + 12.0f}, 2.0f, BLACK);
        DrawLineEx(Vector2{box.x + 6.5f, box.y + 12.0f}, Vector2{box.x + 12.5f, box.y + 4.5f}, 2.0f, BLACK);
    }
}

void drawList(const ShaderMenu &menu, const std::vector<ShaderToggle> &toggles, const Panel &panel,
              Vector2 mouse, bool allowInput)
{
    drawPanelFrame(panel, "Shaders", allowInput ? "[M] masquer" : "[R] souris");

    BeginScissorMode(static_cast<int>(panel.view.x), static_cast<int>(panel.view.y),
                     static_cast<int>(panel.view.width), static_cast<int>(panel.view.height));
    for (std::size_t i = 0; i < toggles.size(); ++i) {
        const Rectangle row = rowRect(panel, menu.scroll, i, kListRowHeight);
        if (!rowVisible(panel, row))
            continue;

        const bool on = toggles[i].enabled;
        if (i == menu.selected)
            DrawRectangleRec(row, Fade(kSelection, 0.45f));
        else if (allowInput && CheckCollisionPointRec(mouse, row) && CheckCollisionPointRec(mouse, panel.view))
            DrawRectangleRec(row, Fade(RAYWHITE, 0.07f));

        const Rectangle check{row.x + 12.0f, row.y + 6.0f, 16.0f, 16.0f};
        drawCheck(check, on);
        DrawText(toggles[i].name.c_str(), static_cast<int>(check.x) + 26, static_cast<int>(row.y) + 7, 14,
                 on ? RAYWHITE : kDimText);
    }
    EndScissorMode();
    drawScrollbar(panel, menu.scroll);
}

void drawSliderRow(const ShaderParam &p, const Rectangle &row, bool dragging)
{
    DrawText(p.name.c_str(), static_cast<int>(row.x) + 12, static_cast<int>(row.y) + 6, 13, RAYWHITE);
    const std::string value = formatNumber(p);
    DrawText(value.c_str(),
             static_cast<int>(row.x + row.width - kScrollbarRoom) - 12 - MeasureText(value.c_str(), 13),
             static_cast<int>(row.y) + 6, 13, kAccent);

    const Rectangle track = sliderTrack(row);
    const float ratio = (p.max > p.min) ? std::clamp((numberAsFloat(p) - p.min) / (p.max - p.min), 0.0f, 1.0f) : 0.0f;
    DrawRectangleRec(track, Fade(RAYWHITE, 0.15f));
    DrawRectangleRec(Rectangle{track.x, track.y, track.width * ratio, track.height}, Fade(kAccent, 0.8f));
    DrawCircle(static_cast<int>(track.x + track.width * ratio), static_cast<int>(track.y + track.height * 0.5f),
               dragging ? 7.0f : 6.0f, dragging ? RAYWHITE : kAccent);
}

void drawBoolRow(const ShaderParam &p, const Rectangle &row)
{
    const Rectangle box = checkBox(row);
    drawCheck(box, std::get<bool>(p.value));
    DrawText(p.name.c_str(), static_cast<int>(box.x) + 26, static_cast<int>(row.y) + 14, 13, RAYWHITE);
}

void drawVec3Row(const ShaderMenu &menu, const ShaderParam &p, const Rectangle &row, std::size_t paramIndex)
{
    DrawText(p.name.c_str(), static_cast<int>(row.x) + 12, static_cast<int>(row.y) + 5, 13, RAYWHITE);

    auto v = std::get<shimera::Vec3<float>>(p.value);
    const auto fields = vec3Fields(row);
    for (std::size_t k = 0; k < 3; ++k) {
        const bool focused = menu.focusedField == paramIndex * 3 + k;
        DrawRectangleRec(fields[k], Fade(RAYWHITE, focused ? 0.14f : 0.08f));
        DrawRectangleLinesEx(fields[k], 1.0f, focused ? kAccent : Fade(RAYWHITE, 0.3f));

        std::string text = focused ? menu.editBuffer : std::string(TextFormat("%g", vec3Component(v, k)));
        if (focused && static_cast<int>(GetTime() * 2.0) % 2 == 0)
            text += '_';
        DrawText(text.c_str(), static_cast<int>(fields[k].x) + 5, static_cast<int>(fields[k].y) + 3, 12,
                 focused ? RAYWHITE : kAccent);
    }
}

void drawParams(const ShaderMenu &menu, const std::vector<ShaderToggle> &toggles, const Panel &panel)
{
    const auto *params = selectedParams(menu, toggles);
    const std::string title = params ? "Parametres: " + toggles[menu.selected].name : "Parametres";
    drawPanelFrame(panel, title.c_str(), nullptr);

    BeginScissorMode(static_cast<int>(panel.view.x), static_cast<int>(panel.view.y),
                     static_cast<int>(panel.view.width), static_cast<int>(panel.view.height));
    if (!params || params->empty()) {
        DrawText(params ? "Aucun parametre" : "Clic droit sur un shader",
                 static_cast<int>(panel.view.x) + 12, static_cast<int>(panel.view.y) + 14, 13, kDimText);
    } else {
        for (std::size_t i = 0; i < params->size(); ++i) {
            const Rectangle row = rowRect(panel, menu.paramScroll, i, kParamRowHeight);
            if (!rowVisible(panel, row))
                continue;
            const ShaderParam &p = (*params)[i];
            if (isNumber(p))
                drawSliderRow(p, row, menu.draggedSlider == i);
            else if (std::holds_alternative<bool>(p.value))
                drawBoolRow(p, row);
            else
                drawVec3Row(menu, p, row, i);
        }
    }
    EndScissorMode();
    drawScrollbar(panel, menu.paramScroll);
}

}

// --- Public API ----------------------------------------------------------------

bool isMouseOverMenu(const ShaderMenu &menu, const std::vector<ShaderToggle> &toggles)
{
    if (!menu.visible || toggles.empty())
        return false;
    const MenuLayout layout = menuLayout(menu, toggles);
    const Vector2 mouse = GetMousePosition();
    return CheckCollisionPointRec(mouse, layout.list.box) || CheckCollisionPointRec(mouse, layout.params.box);
}

void updateShaderMenu(ShaderMenu &menu, std::vector<ShaderToggle> &toggles, bool allowInput)
{
    std::vector<ShaderParam> *params =
        selectedParams(menu, toggles) ? &toggles[menu.selected].params : nullptr;

    if (IsKeyPressed(KEY_SEMICOLON)) {
        menu.visible = !menu.visible;
        commitField(menu, params);
        menu.draggedSlider = ShaderMenu::kNone;
    }
    if (!menu.visible || toggles.empty())
        return;
    if (!allowInput) {
        commitField(menu, params);
        menu.draggedSlider = ShaderMenu::kNone;
        return;
    }

    const MenuLayout layout = menuLayout(menu, toggles);
    const Vector2 mouse = GetMousePosition();

    if (CheckCollisionPointRec(mouse, layout.list.box))
        menu.scroll -= GetMouseWheelMove() * kScrollStep;
    else if (CheckCollisionPointRec(mouse, layout.params.box))
        menu.paramScroll -= GetMouseWheelMove() * kScrollStep;
    menu.scroll = std::clamp(menu.scroll, 0.0f, layout.list.maxScroll);
    menu.paramScroll = std::clamp(menu.paramScroll, 0.0f, layout.params.maxScroll);

    if (menu.focusedField != ShaderMenu::kNone)
        typeIntoField(menu, params);

    updateList(menu, toggles, layout.list, mouse);

    // The selection may have just changed: re-resolve before editing.
    if (selectedParams(menu, toggles))
        updateParams(menu, toggles[menu.selected].params, layout.params, mouse);
    else
        menu.draggedSlider = ShaderMenu::kNone;
}

void drawShaderMenu(const ShaderMenu &menu, const std::vector<ShaderToggle> &toggles, bool allowInput)
{
    if (!menu.visible || toggles.empty())
        return;

    const MenuLayout layout = menuLayout(menu, toggles);
    const Vector2 mouse = GetMousePosition();

    drawList(menu, toggles, layout.list, mouse, allowInput);
    drawParams(menu, toggles, layout.params);
}

}
