#ifndef _RENDER_FX_
#define _RENDER_FX_
#include "Global.fx"
#include "EffectBase.fx"


#define MAX_MODEL_TRANSFORMS 250
#define MAX_MODEL_KEYFRAMES 250
#define MAX_MODEL_INSTANCE 500

Texture2D NoiseMap;
Texture2D MaskMap;
BlendState AlphaBlendState
{
    AlphaToCoverageEnable = true;
    BlendEnable[0] = true;
    SrcBlend = SRC_ALPHA;
    DestBlend = INV_SRC_ALPHA;
    BlendOp = ADD;
    BlendOpAlpha = ADD;
    SrcBlendAlpha = ONE;
    DestBlendAlpha = ZERO;
    RenderTargetWriteMask[0] = 0x0F;
};
//Mesh Render//
//struct EffectMesh
//{
//    float4 position : POSITION;
//    float2 uv : TEXCOORD;
//    float3 normal : NORMAL;
//    float3 tangent : TANGENT;
//};
struct EffectMesh
{
    float4 position : POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    uint instanceID : SV_INSTANCEID;
    matrix world : INST;
    float time : INSTTIME;
};

struct EffectOutput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float3 worldPosition : POSITION1;
    float time : TIME;
};

cbuffer ColorBuffer
{
    float4 particleColor;
    float4 LowParticleColor;
};
EffectOutput StormVS(EffectMesh input)
{
    EffectOutput output;
    
   // output.position = mul(input.position, W);
    output.position = mul(input.position, input.world);
    output.worldPosition = output.position;
    output.position = mul(output.position, VP);
    output.normal =input.normal;
    
    output.uv = input.uv;
    output.time = input.time;
    return output;
}
float4 PS(EffectOutput input) : SV_TARGET
{
   
    float2 doubleUV = mul(input.uv, 2.0f);
    float2 pannedUV1 = Panner(doubleUV, float2(0.2f, 1.0f), input.time);
    float4 noisecolor = NoiseMap.Sample(LinearSampler, pannedUV1);
    float noiseScalar = noisecolor.r * 0.15f;
    float2 uvOffset = float2(-0.08866f, 0.8f);
    uvOffset = input.uv + uvOffset;
    
    uvOffset = uvOffset + float2(noiseScalar, noiseScalar);
    float4 color = MaskMap.Sample(LinearSampler, uvOffset);
    float rgbmixer = pow(color.b, 2.0f);
    float3 rgbcolor = mul(particleColor.rgb, rgbmixer);
    
    RadialGradientExponetialDesc desc;
    desc = Init_RadialGradientExponetialDesc();
    desc.radius = 0.4f;
    desc.center = float2(0.25f, 0.25f);
//    desc.doInvert = true;
    noiseScalar = RadialGradientExponetial(input.uv / 2, desc);
    noiseScalar = color.r * noiseScalar;
    noiseScalar = particleColor.a * noiseScalar;
    color.rgb = 1 - rgbcolor;
    color.a = noiseScalar;
    //if (noiseScalar < (color.r + color.g + color.b)/3)
    //{
    //    return float4(0, 0, 0, 0);
    //}
    
    return color;
}

technique11 T0
{
    PASS_RS_BS_VP(P0, CullNone, AlphaBlendState, StormVS, PS)
//    PASS_RS_SP(P0, CullNone, MeshVS, PS)
//	PASS_RS_SP(P0, ShadowRaster, MeshVS, PS)
};

#endif
