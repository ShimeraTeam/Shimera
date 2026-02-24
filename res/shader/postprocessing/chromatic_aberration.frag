#version 330 core
in vec2 texCoords;
out vec4 color;

uniform sampler2D u_screenTexture;
uniform vec2 u_offset;
uniform float u_radius;

void flat_chromatic_aberration()
{
    color.r = texture(u_screenTexture, texCoords - u_offset).r;
    color.g = texture(u_screenTexture, texCoords).g;
    color.b = texture(u_screenTexture, texCoords + u_offset).b;
    color.a = 1.0;
}

void radial_chromatic_aberration() {
    vec2 center = vec2(0.5, 0.5);
    vec2 dir = texCoords - center;
    float dist = length(dir);
    vec2 offset = normalize(dir) * u_radius * dist;

    color.r = texture(u_screenTexture, texCoords - offset).r;
    color.g = texture(u_screenTexture, texCoords).g;
    color.b = texture(u_screenTexture, texCoords + offset).b;
    color.a = 1.0;
}

void main()
{
    if (u_radius == 0.0) {
        flat_chromatic_aberration();
        return;
    }
    radial_chromatic_aberration();
};