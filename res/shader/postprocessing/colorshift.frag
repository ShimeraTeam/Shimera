#version 330 core

precision highp float;

in vec2 texCoords;
out vec4 color;

uniform sampler2D u_screenTexture;
uniform vec3 u_tint;

void main(void) {
    vec4 texColor = texture(u_screenTexture, texCoords);
    color.a = texColor.a;
    color.rgb = (texColor.rgb + u_tint) / 2.0;
}
