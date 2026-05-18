import glsl_operators_overload;

struct VSInput {
    float3 Pos;
    float2 UV;
};

struct VSOutput {
    float4 Pos : SV_POSITION;
    float2 UV;
};

struct ShaderData {
    float4x4 projection;
    float4x4 view;
    float4 viewPosition;
    float4 viewDirection;
    float time;
};

struct PushConstants {
    ConstBufferPointer<ShaderData> uni;  // 8 bytes (buffer device address)
    float4x4                       model;   // 64 bytes — put first for alignment
};

[[vk::push_constant]] PushConstants pc;


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

    float time = pc.uni.get().time;

    color *= (sin(time) * 0.5 + 0.5) * 0.5+0.5; // 0.5-1.0

    return float4(color, 1.0);
}
