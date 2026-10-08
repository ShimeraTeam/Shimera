#include "postprocess.slang"

uniform Sampler2D u_screenTexture;

uniform float u_strength;
uniform float u_radius;
uniform float u_gap;
uniform float4 u_color;
uniform int u_isRounded;
uniform float2 u_resolution;

[shader("fragment")]
float4 fragMain(VertexOutput input) : SV_Target {
    const float4 texColor = u_screenTexture.Sample(input.texCoords);

    float dist;
    if (u_isRounded == 1) {
        const float2 aspect = float2(u_resolution.x / u_resolution.y, 1.0);
        dist = distance(input.texCoords * aspect, float2(0.5, 0.5) * aspect);
    } else {
        dist = distance(input.texCoords, float2(0.5, 0.5));
    }

    const float vignette = smoothstep(u_radius, u_radius + u_gap, dist) * u_strength;
    return float4(lerp(texColor.rgb, u_color.rgb, vignette), texColor.a);
}
