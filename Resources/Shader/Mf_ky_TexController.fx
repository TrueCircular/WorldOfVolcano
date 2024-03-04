#ifndef _RENDER_FX_
#define _RENDER_FX_
#include "Global.fx"

Texture2D NoiseMap;

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
struct NumMesh
{
    float4 position : POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
};
struct NumOutput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
    float3 worldPosition : POSITION1;
    uint num : NUM;
};

cbuffer Effectbuffer
{
    float texture_tiling_scalar;
    float2 time_scalar_textureSpeed;
    float texture_y_compression_scalar;
    float extend_value;
    float2 texPower_Muliply;
};

NumOutput MeshVS(NumMesh input)
{
    NumOutput output;

    output.position = mul(input.position, input.world);
    output.worldPosition = output.position;
    output.position = mul(output.position, VP);
    
    output.uv = input.uv;
    
    return output;
}

float4 PS(NumOutput input) : SV_TARGET
{
    float2 mixuv = input.uv*texture_tiling_scalar;
    float2 speed = 
}
technique11 T0
{
    PASS_RS_BS_VP(P0,CullNone,DamageBlendState,MeshVS,PS)
//    PASS_RS_SP(P0, CullNone, MeshVS, PS)
//	PASS_RS_SP(P0, ShadowRaster, MeshVS, PS)
};

#endif
