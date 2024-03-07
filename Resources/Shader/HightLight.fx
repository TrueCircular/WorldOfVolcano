#include "Global.fx"

Texture2D HightLightMap;

struct HightLightStruct
{
    float2 noiseAScalar; //= { 0.3,0.1};
    float2 noiseBScalar; //= {0.2,0.2 };
    
    float multiply;//=50;
    float hilight;//= 1;
};

float HightLight_ComputeTexControl(float2 uv,HightLightStruct effStruct)// Don't Use MixedUV!!!!!!!
{
    //NoiseSpeed
    float2 mixuvB = time * effStruct.noiseAScalar;
    float2 mixuvA = time * effStruct.noiseBScalar;
    
    //UV
    mixuvB = uv + mixuvB;
    mixuvA = uv * mixuvA;
    
    float colorB = HightLightMap.Sample(LinearSampler, mixuvB).b;
    float colorA = HightLightMap.Sample(LinearSampler, mixuvA).r;
    
    float color = colorA + colorB;
    colorA = colorA * colorB;
    color = color * effStruct.multiply;
    colorA = colorA * effStruct.multiply;
    
    color = color * colorA;
    
    color = color * effStruct.hilight; //May need to cbufferScalar;
    return color;
    
}

//cbuffer HightLight_Effectbuffer
//{
//    float hightLight_time;
//};
//
//float HightLight_ComputeTexControl(float2 uv)// Don't Use MixedUV!!!!!!!
//{
//    //NoiseSpeed
//    float2 mixuvB = hightLight_time * float2(0.2, 0.2);
//    float2 mixuvA = hightLight_time * float2(0.3, 0.1);
//    
//    //UV
//    mixuvB = uv + noiseSpdB;
//    mixuvA = uv * noiseSpdA;
//    
//    float colorB = HightLightMap.Sample(LinearSampler, mixuvB).b;
//    float colorA = HightLightMap.Sample(LinearSampler, mixuvA).r;
//    
//    float color = colorA + colorB;
//    colorA = colorA * colorB;
//    color = color * 50;
//    colorA = colorA * 50;
//    
//    color = color * colorA;
//    
//    Fresnel();
////    color= color* 1 //May need to cbufferScalar;
//    return color;
//    
//}


