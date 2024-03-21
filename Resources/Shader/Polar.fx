#ifndef _RENDER_FX_
#define _RENDER_FX_
#include "Global.fx"
#include "EffectBase.fx"


#define MAX_MODEL_TRANSFORMS 250
#define MAX_MODEL_KEYFRAMES 250
#define MAX_MODEL_INSTANCE 500

Texture2D NoiseMap;

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
struct EffectModel
{
    float4 position : POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float4 blendIndices : BLEND_INDICES;
    float4 blendWeights : BLEND_WEIGHTS;
    uint instanceID : SV_INSTANCEID;
    matrix world : INST;
    float time : INSTTIME;
};
cbuffer BoneBuffer
{
    matrix BoneTransform[MAX_MODEL_TRANSFORMS];
};
uint BoneIndex;

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
EffectOutput StormModelVS(EffectModel input)
{
    EffectOutput output;
    
   // output.position = mul(input.position, W);
    output.position = mul(input.position, BoneTransform[BoneIndex]);
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
    
       
    RadialGradientExponetialDesc radialDesc1;
    RadialGradientExponetialDesc radialDesc2;
    VectorToRadialValueDesc radialdesc;
    radialDesc1 = Init_RadialGradientExponetialDesc();
    radialDesc2 = Init_RadialGradientExponetialDesc();
    radialdesc = Init_VectorToRadialValueDesc();
    
    radialdesc.uv = input.uv;
    radialdesc.power = 2.0f;
    radialDesc1.radius = 0.3;
    radialDesc1.density = 55;
    radialDesc2.radius = radialDesc1.radius - 0.01;
    radialDesc2.density = 15;
    bool typeB1 = false;
    bool typeB2 = false;
    
    
    float2 radialuv = VectorToRadialValue(radialdesc);
    float2 noiseuv1 = float2(0, 1);
    float2 noisespeed = float2(0, -1);
    noisespeed = mul(noisespeed, input.time);
    noiseuv1 = mul(input.uv, noiseuv1);
    noiseuv1 = noiseuv1 + noisespeed;
    radialuv = radialuv + noiseuv1;
    float4 alpha = NoiseMap.Sample(LinearSampler, radialuv);
    //return alpha;
    float density = pow(alpha.a, 1.0f);
    density = density * 0.3f;
    float2 realuv = input.uv + float2(density, density);
    
    
    float radial1 = RadialGradientExponetial(realuv, radialDesc1);
    float radial2 = RadialGradientExponetial(realuv, radialDesc2);
    float radialValue = abs(radial1 - radial2);
    float4 finalColor = particleColor;
    float3 rgbcolor = lerp(float3(0, 0, 0), particleColor.rgb, radialValue);
    if (typeB1)
    {
        finalColor.rgb = rgbcolor;
    }
    if (typeB2)
    {
        radialValue = radial1;
    }
    float opacity = particleColor.a * radialValue;
    float total = finalColor.r + finalColor.g + finalColor.b;
    //if (total < opacity * 3)
    //{
    finalColor.a = opacity;
    //    return finalColor;
    //}
    return finalColor;

}

technique11 T0
{
    PASS_RS_BS_VP(P0, CullBack, AlphaBlendState, StormVS, PS)
    PASS_RS_BS_VP(P1, CullNone, AlphaBlendState, StormModelVS, PS)
//    PASS_RS_SP(P0, CullNone, MeshVS, PS)
//	PASS_RS_SP(P0, ShadowRaster, MeshVS, PS)
};

#endif
