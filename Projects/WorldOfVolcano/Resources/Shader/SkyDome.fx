#ifndef _RENDER_FX_
#define _RENDER_FX_
#include "Global.fx"

Texture2D ImageMap;
Texture2D BlendMap;

struct DomeMesh
{
    float4 position : POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
};

struct DomeOutput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 worldPosition : POSITION1;
};

DomeOutput StormVS(DomeMesh input)
{
    DomeOutput output;
    
    output.position = mul(input.position, W);
    output.worldPosition = output.position;
    output.position = mul(output.position, VP);
    output.normal = input.normal;
    output.uv = input.uv;
    output.tangent = input.tangent;
    return output;
}

float4 PS(DomeOutput input) : SV_TARGET
{
    
    float4 color = ImageMap.Sample(LinearSampler, input.uv);
    float2 invertUV = float2(1, 1) - input.uv;
    float4 blendColor = BlendMap.Sample(LinearSampler, invertUV);
    color.rgb = lerp(color.rgb, blendColor.rgb, blendColor.a);
    
    //float4 color = DiffuseMap.Sample(LinearSampler, input.uv);

        return color;
    
    
}

technique11 T0
{
    PASS_RS_VP(P0, CullNone, StormVS, PS)
//    PASS_RS_SP(P0, CullNone, MeshVS, PS)
//	PASS_RS_SP(P0, ShadowRaster, MeshVS, PS)
};

#endif
