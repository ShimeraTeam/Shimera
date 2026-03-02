#version 330 core

in vec2 texCoords;
out vec4 color;

uniform sampler2D u_screenTexture;
uniform float u_strength;
uniform int u_radius;
uniform float u_contrast;
uniform int u_samples;


void main()
{
    vec4 color_sum  = vec4(0.0);
    vec4 weight_sum = vec4(0.0);

    for (float i = 0.0; i <= 1.0; i += 1.0 / u_samples) {
        /* samples weight : red go from 1 to 0, green peak at middle, blue go from 0 to 1.
        we won't see green as a color often, because it will be mixed with red and blue, which will give the expacted color,
        but leave red and blue at the edges. */
        vec4 weight = vec4(i, 1.0 - abs(i * 2 - 1.0), 1.0 - i, 0.5);

        vec2 coord = texCoords;
        if (u_radius >= 1) {
            // For radius, the farther from the center, the more pixels will be displaced.
            coord = mix(texCoords, vec2(0.5), (i - 0.5) * 0.1 * u_strength);
        } else {
            coord = texCoords + 0.04 * (i - 0.5) * u_strength;
        }

        weight = 0.5 + (weight - 0.5) * u_contrast;

        vec4 col = texture(u_screenTexture, coord);
        color_sum += col * col * weight; // blend in linear space
        weight_sum += weight;
    }

    color = sqrt(clamp(color_sum / weight_sum, 0.0, 1.0)); // convert back to gamma space
};
