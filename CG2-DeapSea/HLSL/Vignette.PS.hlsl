#include"FullScreen.hlsli"

Texture2D<float32_t4> gTexture : register(t0);
SamplerState gSampler : register(s0);

//struct Vignetting
//{
//    float centerLighting;
//};
//ConstantBuffer<Vignetting> gVignetting : register(b0);

struct PixelShaderOutput
{
    float32_t4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    output.color = gTexture.Sample(gSampler, input.texcoord);
    
    //中心にいくほど明るくなるよう調整
    float32_t2 correct = input.texcoord * (1.0f - input.texcoord.yx);
    //中心の明るさの最大値を決める。1.0fで0.0625
    float vignette = correct.x * correct.y * 16.0f;
    //
    vignette = saturate(pow(vignette, 1.0f));
    
    output.color.rgb *= vignette;
    return output;
}