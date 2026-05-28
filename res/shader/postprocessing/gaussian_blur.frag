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

    vec4 result = vec4(0.0);
    float totalWeight = 0.0;

    for (int i = -u_samples; i <= u_samples; i++) {
        float weight = gaussian(float(i), u_sigma);
        result += texture(u_screenTexture, texCoords + u_direction * texelSize * float(i)) * weight;
        totalWeight += weight;
    }

    color = result / totalWeight;
}
