#ifndef SHIMERA_COLOR_HPP
#define SHIMERA_COLOR_HPP

#include <cstdint>

/**
 * Backend "abstraction-friendly" color representation
 * Normalized RGBA values (0.0 to 1.0)
 */
struct Color {
    float r;
    float g;
    float b;
    float a;

    constexpr explicit Color(const float r = 0.0f, const float g = 0.0f, const float b = 0.0f, const float a = 1.0f)
        : r(r), g(g), b(b), a(a) {}

    static constexpr Color fromRGB(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a = 255) {
        return Color(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
    }

    static constexpr Color Black()       { return Color(0.0f, 0.0f, 0.0f, 1.0f); }
    static constexpr Color White()       { return Color(1.0f, 1.0f, 1.0f, 1.0f); }
    static constexpr Color Red()         { return Color(1.0f, 0.0f, 0.0f, 1.0f); }
    static constexpr Color Green()       { return Color(0.0f, 1.0f, 0.0f, 1.0f); }
    static constexpr Color Blue()        { return Color(0.0f, 0.0f, 1.0f, 1.0f); }
    static constexpr Color Yellow()      { return Color(1.0f, 1.0f, 0.0f, 1.0f); }
    static constexpr Color Cyan()        { return Color(0.0f, 1.0f, 1.0f, 1.0f); }
    static constexpr Color Magenta()     { return Color(1.0f, 0.0f, 1.0f, 1.0f); }
    static constexpr Color Transparent() { return Color(0.0f, 0.0f, 0.0f, 0.0f); }
};

#endif //SHIMERA_COLOR_HPP
