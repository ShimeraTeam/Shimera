#ifndef SHIMERA_COLOR_HPP
#define SHIMERA_COLOR_HPP

#include <shimera_api.h>
#include <cstdint>

/**
 * Backend "abstraction-friendly" color representation
 * Normalized RGBA values (0.0 to 1.0)
 */
namespace shimera {

struct SHIMERA_API Color {
    float m_r;
    float m_g;
    float m_b;
    float m_a;

    constexpr explicit Color(const float r = 0.0f, const float g = 0.0f, const float b = 0.0f, const float a = 1.0f)
        : m_r(r), m_g(g), m_b(b), m_a(a) {}

    static constexpr Color fromRGB(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a = 255) {
        return Color(static_cast<float>(r) / 255.0f, static_cast<float>(g) / 255.0f, static_cast<float>(b) / 255.0f, static_cast<float>(a) / 255.0f);
    }

    static constexpr Color black()       { return Color(0.0f, 0.0f, 0.0f, 1.0f); }
    static constexpr Color white()       { return Color(1.0f, 1.0f, 1.0f, 1.0f); }
    static constexpr Color red()         { return Color(1.0f, 0.0f, 0.0f, 1.0f); }
    static constexpr Color green()       { return Color(0.0f, 1.0f, 0.0f, 1.0f); }
    static constexpr Color blue()        { return Color(0.0f, 0.0f, 1.0f, 1.0f); }
    static constexpr Color yellow()      { return Color(1.0f, 1.0f, 0.0f, 1.0f); }
    static constexpr Color cyan()        { return Color(0.0f, 1.0f, 1.0f, 1.0f); }
    static constexpr Color magenta()     { return Color(1.0f, 0.0f, 1.0f, 1.0f); }
    static constexpr Color transparent() { return Color(0.0f, 0.0f, 0.0f, 0.0f); }
};

}

#endif //SHIMERA_COLOR_HPP
