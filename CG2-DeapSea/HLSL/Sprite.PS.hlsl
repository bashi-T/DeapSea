#include "Sprite.hlsli"

struct Material
{
    float32_t4 color;
    //int32_t enableLighting;
    float32_t4x4 uvTransform;
};
ConstantBuffer<Material> gMaterial : register(b0);

//struct DirectionalLight
//{
//    float32_t4 color;
//    float32_t3 direction;
//    float intensity;
//};
//ConstantBuffer<DirectionalLight> gDirectionalLight : register(b1);
//
//struct Camera
//{
//    float32_t3 worldPosition;
//};
//ConstantBuffer<Camera> gCamera : register(b2);

struct PixelShaderOutput
{
    float32_t4 color : SV_TARGET0;
};
Texture2D<float32_t4> gTexture : register(t0);
SamplerState gSampler : register(s0);

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    float4 transformedUV = mul(float32_t4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
    float32_t4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);
    output.color = gMaterial.color * textureColor;
    if (output.color.a <= 0.0)
    {
        discard;
    }

    return output;
}