#include "postprocess.slang"

uniform Sampler2D u_screenTexture;
uniform float2 u_resolution;
uniform float2 u_direction;
uniform float u_sigma;
uniform int u_samples;

float gaussian(float x, float sigma) {
    return exp(-(x * x) / (2.0 * sigma * sigma));
}

[shader("fragment")]
float4 fragMain(VertexOutput input) : SV_Target {
    float2 texelSize = 1.0 / u_resolution;

    float centerWeight = gaussian(0.0, u_sigma);
    float4 result = u_screenTexture.Sample(input.texCoords) * centerWeight;
    float totalWeight = centerWeight;

    for (int i = 1; i <= u_samples; i++) {
        float weight = gaussian(float(i), u_sigma);
        float2 off = u_direction * texelSize * float(i);
        result += u_screenTexture.Sample(input.texCoords + off) * weight;
        result += u_screenTexture.Sample(input.texCoords - off) * weight;
        totalWeight += 2.0 * weight;
    }

    return result / totalWeight;
}