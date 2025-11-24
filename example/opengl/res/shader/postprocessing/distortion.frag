#version 330 core

in vec2 texCoords;
out vec4 color;

uniform sampler2D u_screenTexture;
//uniform float time; // Hardcoded for now
const float time = 0.0;
//uniform float noiseScale; // Hardcoded for now
const float noiseScale = 3.0;
//uniform float distortionStrength; // Hardcoded for now
const float distortionStrength = 0.13;
//uniform float timeScale; // Hardcoded for now
const float timeScale = 0.1;

// https://www.shadertoy.com/view/XdXBRH from https://iquilezles.org/articles/gradientnoise/
vec2 hash(in ivec2 p) {
    // 2D -> 1D
    ivec2 n = p.x * ivec2(3, 37) + p.y * ivec2(311, 113);

    // 1D hash by Hugo Elias
    n = (n << 13) ^ n;
    n = n * (n * n * 15731 + 789221) + 1376312589;
    return -1.0 + 2.0 * vec2(n & ivec2(0x0fffffff)) / float(0x0fffffff);
}

vec3 noised(in vec2 p) {
    ivec2 i = ivec2(floor(p));
    vec2 f = fract(p);

    // quintic interpolation
    vec2 u = f * f * f * (f * (f * 6.0 - 15.0) + 10.0);
    vec2 du = 30.0 * f * f * (f * (f - 2.0) + 1.0);

    vec2 ga = hash(i + ivec2(0, 0));
    vec2 gb = hash(i + ivec2(1, 0));
    vec2 gc = hash(i + ivec2(0, 1));
    vec2 gd = hash(i + ivec2(1, 1));

    float va = dot(ga, f - vec2(0.0, 0.0));
    float vb = dot(gb, f - vec2(1.0, 0.0));
    float vc = dot(gc, f - vec2(0.0, 1.0));
    float vd = dot(gd, f - vec2(1.0, 1.0));

    return vec3(va + u.x * (vb - va) + u.y * (vc - va) + u.x * u.y * (va - vb - vc + vd),
    ga + u.x * (gb - ga) + u.y * (gc - ga) + u.x * u.y * (ga - gb - gc + gd) +
    du * (u.yx * (va - vb - vc + vd) + vec2(vb, vc) - va));
}

void main() {
    vec2 distortion = noised((texCoords + time * timeScale) * noiseScale).xy * distortionStrength;
    vec4 texel = texture(u_screenTexture, texCoords + distortion);
    color = texel;
}
