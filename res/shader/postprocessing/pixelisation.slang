#include "postprocess.slang"

uniform Sampler2D u_screenTexture;
uniform float u_pixelSizeX;
uniform float u_pixelSizeY;
uniform float2 u_resolution;
uniform float2 u_offset;

[shader("fragment")]
void fragMain(VertexOutput input, out float4 color) : SV_Target{
    float2 pixelUV = float2(u_pixelSizeX, u_pixelSizeY) / u_resolution;
    float2 coord = floor((input.texCoords - u_offset) / pixelUV) * pixelUV + u_offset + pixelUV * 0.5;
    color = u_screenTexture.Sample(coord);
}
