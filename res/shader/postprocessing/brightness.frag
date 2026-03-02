#version 330 core

precision highp float;

in vec2 texCoords;
out vec4 color;

uniform sampler2D u_screenTexture;
uniform float u_strength;

void main(void) {
    vec4 texColor = texture(u_screenTexture, texCoords);

    vec3 brightened = texColor.rgb + u_strength;
    brightened = clamp(brightened, 0.0, 1.0);

    color = vec4(brightened, texColor.a);
}
