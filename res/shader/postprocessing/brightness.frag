#version 330 core

precision highp float;

in vec2 texCoords;
out vec4 color;

uniform sampler2D u_screenTexture;
uniform float brightness;

void main(void) {
    vec4 texColor = texture(u_screenTexture, texCoords);

    vec3 brightened = texColor.rgb + brightness;
    brightened = clamp(brightened, 0.0, 1.0);

    color = vec4(brightened, texColor.a);
}
