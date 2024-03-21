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
    //AlphaToCoverageEnable = true;
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
    float2 uBaseUV = float2(input.uv.x, 0.0f);
    float2 vBaseUV = float2(0.0f, input.uv.y);

    uBaseUV = mul(uBaseUV, LowParticleColor.b);
    vBaseUV = mul(vBaseUV, LowParticleColor.a);
    float ubasetime = input.time * LowParticleColor.r;
    float vBasetime = input.time * LowParticleColor.g;
    uBaseUV = Panner(uBaseUV, float2(0, 1.0f), ubasetime);
    vBaseUV = Panner(vBaseUV, float2(1.0f, 0), vBasetime);
    float2 mergeUV = uBaseUV + vBaseUV;
    float4 noiseColor = NoiseMap.Sample(LinearSampler, mergeUV);
    
    mergeUV = float2(noiseColor.g, noiseColor.g) + input.uv;
    float4 color = MaskMap.Sample(LinearSampler, mergeUV);
    
    RadialGradientExponetialDesc desc;
    desc = Init_RadialGradientExponetialDesc();
    desc.radius = 0.5f;
    desc.density = 9;
    float3 rgbcolor = mul(particleColor.rgb, color.rgb);
    float alpha = pow(color.a, 2.0f);
    alpha = particleColor.a * alpha;
    float radialMask = RadialGradientExponetial(input.uv, desc);
    alpha = alpha * radialMask;
    color.rgb = rgbcolor;
    color.a = alpha;
    return color;
    
}

technique11 T0
{
    PASS_RS_BS_VP(P0, CullBack, AlphaBlendState, StormVS, PS)
//    PASS_RS_SP(P0, CullNone, MeshVS, PS)
//	PASS_RS_SP(P0, ShadowRaster, MeshVS, PS)
};

#endif
