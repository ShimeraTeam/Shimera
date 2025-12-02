#version 330 core
in vec2 texCoords;
out vec4 color;

uniform sampler2D u_screenTexture;

void main() {
    vec4 texColor = texture(u_screenTexture, texCoords);
    float average = (texColor.r + texColor.g + texColor.b) / 3.0;
    color = vec4(average, average, average, 1.0);
}
