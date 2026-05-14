#version 330 core

in vec2 texCoords;
out vec4 color;

uniform sampler2D u_screenTexture;
uniform float u_pixelSizeX;
uniform float u_pixelSizeY;
uniform vec2 u_resolution;
uniform vec2 u_offset;

void main() {
    vec2 pixelUV = vec2(u_pixelSizeX, u_pixelSizeY) / u_resolution;
    vec2 coord = floor((texCoords - u_offset) / pixelUV) * pixelUV + u_offset + pixelUV * 0.5;
    color = texture(u_screenTexture, coord);
}
