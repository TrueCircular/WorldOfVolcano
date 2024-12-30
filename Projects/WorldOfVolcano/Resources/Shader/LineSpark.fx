#ifndef _RENDER_FX_
#define _RENDER_FX_
#include "Global.fx"
#include "EffectBase.fx"


Texture2D EffectMap;
float duration;

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
    float4 dynamicColor;
};
EffectOutput StormVS(EffectMesh input)
{
    EffectOutput output;
    
   // output.position = mul(input.position, W);
    output.position = input.position;
    input.position.y = input.position.y * input.time * 5;
    output.position = mul(input.position, input.world);
    output.worldPosition = output.position;
    output.position = mul(output.position, VP);
    output.normal = input.normal;
    
    output.uv = input.uv;
    output.time = input.time;
    return output;
}

float4 PS(EffectOutput input) : SV_TARGET
{
    float alpha = 1.0;
    if (input.time > duration / 2)
    {
        alpha = input.time - (duration / 2);
        alpha = alpha / (duration / 2);
        alpha = 1 - alpha;
    }
    else
    {
        alpha = input.time / (duration / 2);
    }
    float4 color = EffectMap.Sample(LinearSampler, input.uv);
    if (color.r + color.g + color.b < alpha * 1.3)
    {
        return float4(0, 0, 0, 0);
    }
    color.r = pow(color.r, dynamicColor.r);
    color.g = pow(color.g, dynamicColor.g);
    color.b = pow(color.b, dynamicColor.b);
    float mulcolor = color.r + color.g;
    mulcolor = mulcolor + color.b;
    float3 rgb = lerp(color.rgb, particleColor.rgb, particleColor.a);
    rgb = mul(rgb, mulcolor);
    color = float4(rgb, alpha);
    

    return color;
}

technique11 T0
{
    PASS_RS_BS_VP(P0, CullBack, AlphaBlendState, StormVS, PS)
//    PASS_RS_SP(P0, CullNone, MeshVS, PS)
//	PASS_RS_SP(P0, ShadowRaster, MeshVS, PS)
};

#endif
