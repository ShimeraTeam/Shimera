#version 330 core

layout (location = 0) in vec2 a_position;
layout (location = 1) in vec2 a_texCoords;

out vec2 texCoords;

void main() {
    texCoords = a_texCoords;
    gl_Position = vec4(a_position, 0.0, 1.0);
}
