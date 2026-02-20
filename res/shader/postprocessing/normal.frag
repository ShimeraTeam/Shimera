#version 330 core

in vec2 texCoords;
out vec4 color;

uniform sampler2D u_screenTexture;

void main() {
    vec4 texColor = texture(u_screenTexture, texCoords);
    color = texColor;
}
