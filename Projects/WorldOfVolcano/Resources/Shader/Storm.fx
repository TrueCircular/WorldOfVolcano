#ifndef _RENDER_FX_
#define _RENDER_FX_
#include "Global.fx"
#include "Light.fx"
#include "EffectBase.fx"


#define MAX_MODEL_TRANSFORMS 250
#define MAX_MODEL_KEYFRAMES 250
#define MAX_MODEL_INSTANCE 500

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
    float3 reflect : REFLECT;
    float3 worldPosition : POSITION1;
    float time : TIME;
};
cbuffer FresnelBuffer
{
    float4 eyePos;
    float4 eyeLook;
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
    float4x4 normalmat = VInv;
    normalmat._41_42_43 = 0;
    normalmat._44 = 1;
    output.normal = normalize(mul(input.normal, normalmat));
    
    float3 Incident = normalize(output.worldPosition - eyePos.xyz);
    output.reflect = Reflect(Incident, output.normal);
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
    float4x4 normalmat = VInv;
    normalmat._41_42_43 = 0;
    normalmat._44 = 1;
    output.normal = normalize(mul(input.normal, normalmat));
    
    float3 Incident = normalize(output.worldPosition - eyePos.xyz);
    output.reflect = Reflect(Incident, output.normal);
    output.uv = input.uv;
    output.time = input.time;
    return output;
}

float4 PS(EffectOutput input) : SV_TARGET
{

    EffectTexController effectController;
    effectController.tilingX = 5;
    effectController.tilingY = 3;
    effectController.offsetX = 0;
    effectController.offSetY = 0;
    effectController.textureSpeed = float2(3,2);
    effectController.texPower = 0.5;
    effectController.multiply = 0.6;
    effectController.y_compression_scalar = 1;
    effectController.extend_value = 0;
    
    float4 controlColor = ComputeTexControl(input.uv,effectController,input.time);
    EffectPrimTexController primeffectController;
    primeffectController.tilingX = 3;
    primeffectController.tilingY = 3;
    primeffectController.textureSpeed = float2(0,2);
    primeffectController.extend_value= controlColor.g;
    float4 primColor = ComputePrimTexControl(input.uv, primeffectController, input.time);
    
    HightLightStruct hightLightStruct;
    hightLightStruct.noiseAScalar = float2(-0.5, 0.5);
    hightLightStruct.noiseBScalar = float2(0.5, 0.75);
    hightLightStruct.multiply = 50;
    hightLightStruct.hilight =1.2f;
    float hightLightColor = HightLight_ComputeTexControl(input.uv, hightLightStruct, input.time);
    
    float hightlightdensity = pow(primColor.r, 2.0f);
    hightLightColor = hightLightColor * hightlightdensity;
    
    float baseColorDensity = 4.0f;
    float baseColorDecay = 5.0f;
    
    baseColorDensity = pow(primColor.r, baseColorDensity);
    
    baseColorDecay = pow(primColor.r, baseColorDecay);
    float subBaseColorDecay = baseColorDecay;
    baseColorDecay = baseColorDecay + 1.0f;
    baseColorDecay = baseColorDecay * particleColor.a;
    
    EffectPrimTexController subColorControl;
    subColorControl.tilingX = 1;
    subColorControl.tilingY = 1;
    subColorControl.textureSpeed = float2(0, 3);
    subColorControl.extend_value= 1;
    
    float4 subColor = ComputePrimTexControl(input.uv, subColorControl, input.time);
    float subColorDensity = 1.3f;
    subColorDensity=subColor.r * subColorDensity;
    subColor = float4(30,50,0,1);
    subColor = subColorDensity * subColor;
    
    float3 ptColor= baseColorDensity*particleColor.rgb;
    ptColor = hightLightColor * ptColor;
    
    subColor = subColor + float4(ptColor,1.0f);
    
    subColor = lerp(LowParticleColor,subColor, subBaseColorDecay);
    
    
    
    TopUnder_EffectStruct topUnder_Effstruct = {1.2f,4.0f,0.0f,1.0f};
    float topUnderMask= TopUnder_ComputeTexControll(input.uv.y,topUnder_Effstruct);
    topUnderMask = clamp(topUnderMask, 0,1);
    baseColorDecay = baseColorDecay * topUnderMask;
    
    float inci = 1.33f;
    inci = pow(1.0 - inci, 2.0) / pow(1.0 + inci, 2.0);
    float fresnelScalar= ComputeFresnel(input.reflect,input.normal,inci);
    fresnelScalar = 1.0f - fresnelScalar;
    fresnelScalar = pow(fresnelScalar, 1.7);
    fresnelScalar = fresnelScalar * 2.5;
    float outlinepower = baseColorDecay * fresnelScalar;
    float backpower = baseColorDecay * 0.02;
    float3 tempoutline;
    tempoutline.r = outlinepower;
    tempoutline.g = outlinepower;
    tempoutline.b = outlinepower;
    float3 opacity;
    opacity.r = backpower;
    opacity.g = backpower;
    opacity.b = backpower;
    opacity = ComputeTwosided(tempoutline, opacity,input.normal);
    
    
    
    subColor = lerp(float4(0, 0, 0, 0), subColor, 1 - opacity.r);
    
    if (subColor.r + subColor.g + subColor.b / 3 < outlinepower*3)
    {
        return 0;
        
    }
    else
    {
        return subColor;
    }
}

technique11 T0
{
    PASS_RS_BS_VP(P0, CullBack, AlphaBlendState, StormVS, PS)
    PASS_RS_BS_VP(P1, CullBack, AlphaBlendState, StormModelVS, PS)
//    PASS_RS_SP(P0, CullNone, MeshVS, PS)
//	PASS_RS_SP(P0, ShadowRaster, MeshVS, PS)
};

#endif
