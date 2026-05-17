import glsl_operators_overload;

struct VSInput {
    float3 Pos;
    float2 UV;
};

struct VSOutput {
    float4 Pos : SV_POSITION;
    float2 UV;
};

[shader("vertex")]
VSOutput main(VSInput input, uint instanceIndex : SV_VulkanInstanceID) {
    VSOutput output;

    output.Pos = float4(input.Pos.xyz, 1.0);
    output.UV = input.UV;

    return output;
}

[shader("fragment")]
float4 main(VSOutput input) {
    float3 color = float3(input.UV, 0.1);

    // float3 color = input.Color * (sin(u.time * 3.0) * 0.5 + 0.5);

    return float4(color, 1.0);
}
