#version 330 core

in vec2 texCoords;
out vec4 color;

uniform sampler2D u_screenTexture;
uniform float u_strength;
uniform float u_radius;
uniform float u_gap;
uniform vec4 u_color;
uniform int u_isRounded;
uniform vec2 u_resolution;

void main() {
    vec4 texColor = texture(u_screenTexture, texCoords);
    float dist = 0.0;
    if (u_isRounded == 1) {
        vec2 aspect = vec2(u_resolution.x / u_resolution.y, 1.0);
        dist = distance(texCoords * aspect, vec2(0.5) * aspect);
    } else {
        dist = distance(texCoords, vec2(0.5));
    }
    float vignette = smoothstep(u_radius, u_radius + u_gap, dist);
    vignette = vignette * u_strength;
    color = vec4(mix(texColor.rgb, u_color.rgb, vignette), texColor.a);
}
