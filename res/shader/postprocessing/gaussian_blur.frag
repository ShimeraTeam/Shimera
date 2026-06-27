#version 330 core

in vec2 texCoords;
out vec4 color;

uniform sampler2D u_screenTexture;
uniform vec2 u_resolution;
uniform vec2 u_direction;
uniform float u_sigma;
uniform int u_samples;

float gaussian(float x, float sigma) {
    return exp(-(x * x) / (2.0 * sigma * sigma));
}

void main() {
    vec2 texelSize = 1.0 / u_resolution;

    float centerWeight = gaussian(0.0, u_sigma);
    vec4 result = texture(u_screenTexture, texCoords) * centerWeight;
    float totalWeight = centerWeight;

    for (int i = 1; i <= u_samples; i++) {
        float weight = gaussian(float(i), u_sigma);
        vec2 off = u_direction * texelSize * float(i);
        result += texture(u_screenTexture, texCoords + off) * weight;
        result += texture(u_screenTexture, texCoords - off) * weight;
        totalWeight += 2.0 * weight;
    }

    color = result / totalWeight;
}
