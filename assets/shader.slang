import glsl_operators_overload;
import srgb;

struct VSInput {
    float3 Pos;
    float3 Color;
    float2 UV;
    uint32_t textureIndex;
};

[[vk::binding(0, 0)]] Sampler2D textures[];

struct VSOutput {
    float4 Pos : SV_POSITION;
    float3 Color;
    float2 UV;
    uint32_t textureIndex;
};

struct ShaderData {
    float4x4 projection;
    float4x4 view;
    float4 viewPosition;
    float4 viewDirection;
    float time;
};

// struct PushConstants {
//     ShaderData *u;
// };
// [[vk::push_constant]] uniform PushConstants pushConstant;
// property u : ShaderData {
//     get { return *pushConstant.u; }
// }

struct PushConstants {
    ConstBufferPointer<ShaderData> uni;  // 8 bytes (buffer device address)
    float4x4                       model;   // 64 bytes — put first for alignment
};

[[vk::push_constant]] PushConstants pc;

[shader("vertex")]
// VSOutput main(VSInput input, uni ImmutablePtr<ShaderData> u, uint instanceIndex : SV_VulkanInstanceID) {
VSOutput main(VSInput input, uint instanceIndex: SV_VulkanInstanceID) {
    VSOutput output;

    ShaderData u = pc.uni.get();   // deref the BDA
    output.Pos = u.projection * u.view * pc.model * float4(input.Pos.xyz, 1.0);
    output.Color = input.Color;
    output.UV = input.UV;
    output.textureIndex = input.textureIndex;

    return output;
}

[shader("fragment")]
// float4 main(VSOutput input, uniform ImmutablePtr<ShaderData> u) {
float4 main(VSOutput input) {
    float3 color = textures[input.textureIndex].Sample(input.UV).rgb;

    color = fromLinearToSRGB(color);
    return float4(color, 1.0);
}
