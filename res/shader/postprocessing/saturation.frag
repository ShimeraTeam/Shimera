#version 330 core

precision highp float;

in vec2 texCoords;
out vec4 color;

uniform sampler2D u_screenTexture;
uniform float saturation;

void main(void) {
    vec4 texColor = texture(u_screenTexture, texCoords);

    // ITU-R BT.709 luma coefficients (HD standard)
    float luminance = dot(texColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    vec3 saturated = mix(vec3(luminance), texColor.rgb, saturation);

    color = vec4(saturated, texColor.a);
}
