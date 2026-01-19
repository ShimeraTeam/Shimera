#version 330 core

precision highp float;

in vec2 texCoords;
out vec4 color;

uniform sampler2D u_screenTexture;

void main(void) {
    vec4 texColor = texture(u_screenTexture, texCoords);

    float contrast = 1.5;

    vec3 contrasted = (texColor.rgb - 0.5) * contrast + 0.5;
    contrasted = clamp(contrasted, 0.0, 1.0);

    color = vec4(contrasted, texColor.a);
}
