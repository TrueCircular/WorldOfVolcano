#ifndef _RENDER_FX_
#define _RENDER_FX_
#include "Global.fx"
#include "Light.fx"
#include "EffectBase.fx"

BlendState AlphaBlendState
{
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
struct EffectMesh
{
    float4 position : POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
};

struct EffectOutput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float3 worldPosition : POSITION1;
};
Texture2D NoiseImage;
Texture2D SpellImage;
cbuffer ColorBuffer
{
    float4 particleColor;
    float4 LowParticleColor;
};

EffectOutput CircleVS(EffectMesh input)
{
    EffectOutput output;
    
    output.position = mul(input.position, W);
    //output.position = mul(input.position, input.world);
    output.worldPosition = output.position;
    output.position = mul(output.position, VP);
    
    
    output.normal = input.normal;
    
    output.uv = input.uv;
    
    return output;
}

float4 PS(EffectOutput input) : SV_TARGET
{
    ///Noise
    VectorToRadialValueDesc vectorDesc;
    vectorDesc = Init_VectorToRadialValueDesc();
    vectorDesc.detail = 0.785f;
    vectorDesc.uv = mul(input.uv, 2);
    float2 mixeduv = VectorToRadialValue(vectorDesc);
    
    float2 panneduv1 = Panner(input.uv, float2(-0.05, -0.35));
    float2 panneduv2 = Panner(input.uv, float2(0.05, -0.2));
    
    float4 pannedColor1 = NoiseImage.Sample(LinearSampler,panneduv1);
    float4 pannedColor2 = NoiseImage.Sample(LinearSampler, panneduv2);
    
    float noisePower = 0.15f;
    
    float opacity = pannedColor1.b * noisePower;
    noisePower = noisePower * -1.0f;
    noisePower = noisePower * pannedColor2.b;
    noisePower = lerp(opacity, noisePower, 0.5f);
    
    RadialGradientExponetialDesc desc;
    desc = Init_RadialGradientExponetialDesc();
    desc.density = 4.0f;
    
    opacity = RadialGradientExponetial(input.uv,desc);
    opacity = 1 - opacity;
    noisePower = noisePower * opacity;
    mixeduv = input.uv + float2(noisePower, noisePower);
    
    float4 colorSpell = SpellImage.Sample(LinearSampler, input.uv);
    float alphaDensity = pow(colorSpell.g, 1.2f);
    
    float4 opacityColor = SpellImage.Sample(LinearSampler, mixeduv);
    
    colorSpell = mul(LowParticleColor, colorSpell.g);
    float4 color = mul(particleColor, opacityColor.r);
    colorSpell = particleColor + colorSpell;
    opacity = pow(opacityColor.r, 1.3f);
    opacity = alphaDensity + opacity;
    opacity = particleColor.a * opacity;
    colorSpell.a = opacity;
    
    return colorSpell;
}

technique11 T0
{
    PASS_RS_BS_VP(P0, CullBack, AlphaBlendState, CircleVS, PS)
//    PASS_RS_SP(P0, CullNone, MeshVS, PS)
//	PASS_RS_SP(P0, ShadowRaster, MeshVS, PS)
};

#endif
