struct VertexInput {
    float2 position : POSITION;
    float2 texCoords : TEXCOORD0;
};

struct VertexOutput {
    float4 position : SV_Position;
    float2 texCoords : TEXCOORD0;
};

[shader("vertex")]
VertexOutput vertMain(VertexInput input) {
    VertexOutput output;
    output.position = float4(input.position, 0.0, 1.0);
    output.texCoords = input.texCoords;
    return output;
}
