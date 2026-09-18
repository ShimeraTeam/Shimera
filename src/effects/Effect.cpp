#include "Effect.hpp"

using shimera::Effect;

void Effect::resize(const int width, const int height) {
    m_resolution = Vec2(static_cast<float>(width), static_cast<float>(height));
}
