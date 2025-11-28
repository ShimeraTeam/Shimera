#version 330 core

in vec2 texCoords;
out vec4 color;

uniform sampler2D u_screenTexture;
//uniform vec3 colorShift; // Hardcoded for now
const vec3 colorShift = vec3(0.8, 0.5, 0.2);

void main() {
    vec4 texColor = texture(u_screenTexture, texCoords);
    color.a = texColor.a;
    color.rgb = (texColor.rgb + colorShift) / 2.0;
}
