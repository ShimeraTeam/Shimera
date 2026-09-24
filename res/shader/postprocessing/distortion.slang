#include "postprocess.slang"

uniform Sampler2D u_screenTexture;
uniform float time;
uniform float noiseScale;
uniform float distortionStrength;
uniform float timeScale;

float2 hash(int2 p) {
    // 2D -> 1D
    int2 n = p.x * int2(3, 37) + p.y * int2(311, 113);

    // 1D hash by Hugo Elias
    n = (n << 13) ^ n;
    n = n * (n * n * 15731 + 789221) + 1376312589;
    return -1.0 + 2.0 * float2(n & int2(0x0fffffff)) / float(0x0fffffff);
}

float3 noised(float2 p) {
    int2 i = int2(floor(p));
    float2 f = fract(p);

    // quintic interpolation
    float2 u = f * f * f * (f * (f * 6.0 - 15.0) + 10.0);
    float2 du = 30.0 * f * f * (f * (f - 2.0) + 1.0);

    float2 ga = hash(i + int2(0, 0));
    float2 gb = hash(i + int2(1, 0));
    float2 gc = hash(i + int2(0, 1));
    float2 gd = hash(i + int2(1, 1));

    float va = dot(ga, f - float2(0.0, 0.0));
    float vb = dot(gb, f - float2(1.0, 0.0));
    float vc = dot(gc, f - float2(0.0, 1.0));
    float vd = dot(gd, f - float2(1.0, 1.0));

    return float3(va + u.x * (vb - va) + u.y * (vc - va) + u.x * u.y * (va - vb - vc + vd),
                ga + u.x * (gb - ga) + u.y * (gc - ga) + u.x * u.y * (ga - gb - gc + gd) +
                    du * (u.yx * (va - vb - vc + vd) + float2(vb, vc) - va));
}

[shader("fragment")]
float4 fragMain(VertexOutput input) : SV_Target {
    float2 distortion = noised((input.texCoords + time * timeScale) * noiseScale).xy * distortionStrength;
    float4 texel = u_screenTexture.Sample(input.texCoords + distortion);
    return texel;
}