#ifndef _RENDER_FX_
#define _RENDER_FX_
#include "Global.fx"
#include "Light.fx"

#include "EffectBase.fx"
#include "Fresnel.fx"
#include "Mf_ky_TexController.fx"
#include "Mf_ky_TexController_Primitive.fx"
#include "MaskToTopUnder.fx"
#include "HightLight.fx"

Texture2D StormNoise;

BlendState DamageBlendState
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
struct StormMesh
{
    float4 position : POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    uint instanceID : SV_INSTANCEID;
    matrix world : INST;
    uint num : INSTU;
};
struct NumOutput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
    float3 worldPosition : POSITION1;
    uint num : NUM;
};
NumOutput MeshVS(NumMesh input)
{
    NumOutput output;

    output.position = mul(input.position, input.world);
    output.worldPosition = output.position;
    output.position = mul(output.position, VP);
    
    output.uv = input.uv;
    output.num = input.num;
    
    return output;
}

float4 PS(NumOutput input) : SV_TARGET
{
    HightLightStruct hightLightStruct;
    EffectTexController effectController;
    EffectPrimTexController primeffectController;
    EffectPrimTexController subColorControll;
    TopUnder_EffectStruct topUnder_Effstruct;
    
    //ComputeFresnel(,);
    return color;
}

technique11 T0
{
    PASS_RS_BS_VP(P0, CullNone, DamageBlendState, MeshVS, PS)
//    PASS_RS_SP(P0, CullNone, MeshVS, PS)
//	PASS_RS_SP(P0, ShadowRaster, MeshVS, PS)
};

#endif
