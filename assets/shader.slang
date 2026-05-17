import glsl_operators_overload;

struct VSInput {
    float3 Pos;
    float3 Color;
    float2 UV;
    int index;
};

[[vk::binding(0, 0)]] Sampler2D textures[];

struct VSOutput {
    float4 Pos : SV_POSITION;
    float3 Color;
    float2 UV;
    // uint32_t InstanceIndex;
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

float3 toLinearSRGB(float3 sRGB)
{
    // bool3 cutoff = lessThan(sRGB, float3(0.04045));
    float3 cutoff = float3(sRGB.lessThan(float3(0.04045)));
    float3 higher = pow((sRGB + float3(0.055))/float3(1.055), float3(2.4));
    float3 lower = sRGB/float3(12.92);
    return lerp(higher, lower, cutoff);
}

float3 fromLinearToSRGB(float3 linearRGB)
{
    float3 cutoff = float3(linearRGB.lessThan(float3(0.0031308)));
    float3 higher = float3(1.055)*pow(linearRGB, float3(1.0/2.4)) - float3(0.055);
    float3 lower = linearRGB * float3(12.92);

    return lerp(higher, lower, cutoff);
}

[shader("vertex")]
VSOutput main(VSInput input, uniform ImmutablePtr<ShaderData> u, uint instanceIndex : SV_VulkanInstanceID) {
// VSOutput main(VSInput input, uint instanceIndex: SV_VulkanInstanceID) {
    VSOutput output;

    output.Pos = u.projection * u.view * float4(input.Pos.xyz, 1.0);
    // output.Pos = mul(u.projection, mul(u.view, float4(input.Pos.xyz, 1.0)));
    output.Color = input.Color;
    output.UV = input.UV;

    return output;
}

[shader("fragment")]
float4 main(VSOutput input, uniform ImmutablePtr<ShaderData> u) {
    float3 color = textures[0].Sample(input.UV).rgb;

    color = fromLinearToSRGB(color);
    return float4(color, 1.0);
}
