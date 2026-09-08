#ifndef SHIMERA_DEMO_DEMOCONFIG_HPP
#define SHIMERA_DEMO_DEMOCONFIG_HPP

#include "raylib.h"

namespace demo {

constexpr int kWidth = 1280;
constexpr int kHeight = 720;

// Two ways to pick the effects of the demo:
//   true  -> in-window menu with a checkbox per shader (non technical public)
//   false -> historical way, uncomment the addEffect() lines in
//            ShaderShowcase.cpp
constexpr bool kUseShaderMenu = true;

// Cornell box
constexpr float kHalf = 5.0f;
constexpr float kBoxHeight = 10.0f;
constexpr Vector3 kBoxCenter{0.0f, kBoxHeight * 0.45f, 0.0f};

// The sphere the atmospheric scattering effect uses as a planet
constexpr Vector3 kSphereCenter{1.8f, 3.9f, 1.2f};
constexpr float kSphereRadius = 0.9f;
constexpr float kAtmosphereScale = 1.7f;

constexpr float kNearPlane = 0.1f;
constexpr float kFarPlane = 300.0f;

}

#endif //SHIMERA_DEMO_DEMOCONFIG_HPP
