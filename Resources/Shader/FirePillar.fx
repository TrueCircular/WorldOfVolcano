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

Texture2D FireNoise;
Texture2D FireMask;

cbuffer BoneBuffer
{
    matrix BoneTransform[MAX_MODEL_TRANSFORMS];
};
uint BoneIndex;
EffectOutput PillarVS(EffectMesh input)
{
    EffectOutput output;
    
    //output.position = mul(input.position, W);
    output.position = mul(input.position, input.world);
    output.worldPosition = output.position;
    output.position = mul(output.position, VP);
    output.normal = mul(input.normal, VInv);
    
    output.uv = input.uv;
    output.time = input.time;
    return output;
}

EffectOutput PillarModelVS(EffectModel input)
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
    output.normal = input.normal;
    
    output.uv = abs(1 - input.uv);
    output.time = input.time;
    return output;
}

float4 PS(EffectOutput input) : SV_TARGET
{
    //Lowparticle == Inner Color
    //Particle == OutLine Color
    /// Base to hightLight
    float2 pannedUV1 = Panner(input.uv, float2(-0.2, -0.3), input.time);
    float4 pannedSample1 = FireNoise.Sample(LinearSampler, pannedUV1);
    
    float2 pannedUV2 = Panner(input.uv, float2(0.2, 0.5), input.time);
    float4 pannedSample2 = FireNoise.Sample(LinearSampler, pannedUV2);
    
    pannedUV1 = Panner(input.uv, float2(-0.1, 0.2), input.time);
    pannedSample1.b = pow(pannedSample1.b, 2);
    pannedSample1.b = pannedSample1.b * 5;
    pannedUV1 = pannedUV1 * float2(pannedSample1.b, pannedSample1.b);
    
    pannedSample2.b = pow(pannedSample2.b, 2);
    pannedSample2.b = pannedSample2.b * 5;
    
    pannedSample2 = FireNoise.Sample(LinearSampler, float2(pannedSample2.b, pannedSample2.b));
    
    
    float hightLightDensity = 1.8f;
    float hightLightPower = 20;
    float baseOpacity = 0.1f;
    
    pannedSample2.g = pow(pannedSample2.g, hightLightDensity);
    pannedSample2.g = mul(pannedSample2.g, 350);
    
    pannedSample1 = FireNoise.Sample(LinearSampler, pannedUV1);
    float hightlight = pannedSample1.g * pannedSample2.g;
    hightlight = hightlight * hightLightPower;
    hightlight = hightlight + baseOpacity;

    /// Fin Based to HightLight
    /// base Texture Control
    
        
    pannedUV1 = float2(input.uv.x * 2, input.uv.y);
    pannedUV1 = Panner(pannedUV1, float2(0.5, 3.0), input.time);
    
    pannedSample1 = FireNoise.Sample(LinearSampler, pannedUV1);
    
    float noiseDensity = 4;
    float noisePower = 5;
    float outDensity = 20;
    float outPower = 20;
    
    float controlscalar= pannedSample1.b;
    controlscalar = pow(controlscalar, noiseDensity);
    controlscalar = controlscalar * noisePower;
    pannedUV2 = input.uv + float2(controlscalar, controlscalar);
    pannedSample1 = FireMask.Sample(LinearSampler, pannedUV2);
    
    controlscalar = pow(pannedSample1.r, outDensity);
    controlscalar = controlscalar * outPower;
    controlscalar = clamp(controlscalar, 0, 1);
    float tempscalar = controlscalar;
    
    float texscalar = pannedSample1.r;
    texscalar = texscalar - controlscalar;
    
    controlscalar = texscalar + controlscalar;
    controlscalar = clamp(controlscalar, 0, 1);
    controlscalar = particleColor.a * controlscalar;
    
    float3 color = mul(particleColor.rgb,10);
    color = mul(color, float3(texscalar, texscalar, texscalar));
    color = mul(float3(hightlight, hightlight, hightlight), color);
    color = lerp(color, particleColor.rgb, controlscalar);
    color = lerp(color, LowParticleColor.rgb, tempscalar);
    //color = lerp(particleColor.rgb, color, tempscalar);
    //Being alphaOpacity;
    
    
    
    float4 finalColor = float4(color, controlscalar);
   // finalColor = lerp(finalColor, float4(1, 0, 0, 0), controlscalar);
    return finalColor;

}

technique11 T0
{
    PASS_RS_BS_VP(P0, CullBack, AlphaBlendState, PillarVS, PS)
    PASS_RS_BS_VPNOD(P1, CullNone, AlphaBlendState, PillarModelVS, PS)
//    PASS_RS_SP(P0, CullNone, MeshVS, PS)
//	PASS_RS_SP(P0, ShadowRaster, MeshVS, PS)
};

#endif
