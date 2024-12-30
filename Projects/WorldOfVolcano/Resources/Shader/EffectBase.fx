#ifndef _EFFECT_FX
#define _EFFECT_FX
#include "Global.fx"

Texture2D Ky_PrimNoiseMap;
Texture2D Ky_NoiseMap;
Texture2D HightLightMap;


struct EffectPrimTexController
{
    float tilingX; // = 1;
    float tilingY; //= 1;
    float2 textureSpeed; // = 0;
    float extend_value; // = 0;
};
struct EffectTexController
{
    float tilingX; //=1;
    float tilingY; //=1;
    float offsetX; // = 0;
    float offSetY; // = 0;
    float2 textureSpeed; //=0;
    float y_compression_scalar; // = 1;
    
    float extend_value; //=0;
    float texPower; //=1;
    float multiply; //=1;
};
struct TopUnder_EffectStruct
{
    float topMaskPower; //=1;
    float underMaskPower; //=1;
    float topMultifly; //=0;
    float underMultifly; //=1;
};
struct HightLightStruct
{
    float2 noiseAScalar; //= { 0.3,0.1};
    float2 noiseBScalar; //= {0.2,0.2 };
    
    float multiply; //=50;
    float hilight; //= 1;
};
struct RadialGradientExponetialDesc
{
    float2 center;
    float radius;
    float density;
    bool doInvert;
};
struct VectorToRadialValueDesc
{
    float2 uv;
    float density;
    float detail;
    float power;
    bool doCoordinate;
};

VectorToRadialValueDesc Init_VectorToRadialValueDesc()
{
    VectorToRadialValueDesc desc;
    desc.uv = float2(0, 0);
    desc.power = 1;
    desc.detail = 6.283185f;
    desc.density = 1;
    desc.doCoordinate = true;
    
    return desc;
}

RadialGradientExponetialDesc Init_RadialGradientExponetialDesc()
{
    RadialGradientExponetialDesc desc;
    desc.center = float2(0.5f, 0.5f);
    desc.radius = 0.5f;
    desc.density = 2.33;
    desc.doInvert = false;
    
    return desc;
}

float ComputeFresnel(float3 vReflect, float3 vNormal, float F0)
{
    float cosAngle = 1 - saturate(dot(vReflect, vNormal));
    float result = pow(cosAngle, 5.0f);
    result = saturate(mad(result, 1 - saturate(F0), F0));
    return result;
}

float3 ComputeTwosided(float3 sideA, float3 sideB, float3 normal)
{
    float3 inverseNormal = -normal;
    float color = lerp(sideB, sideA, inverseNormal);
    
    return color;
}

float2 Panner(float2 uv, float2 speed,float time)
{
    return uv + (speed * time);
}
float ExponetialDensity(float depth, float density, bool doInvert)
{
    float scalar = 0;
    if (depth > 0)
    {
        scalar = depth * density;
        if (doInvert)
        {
            scalar = scalar * scalar;
        }
        scalar = pow(scalar, 2.718282f);
        scalar = 1 / scalar;
        return scalar;
    }
    return 1.0f;
}

//float ComputeFresnel(float3 vLight, float3 vNormal,float incidence)// Don't Use MixedUV!!!!!!!
//{
//    //float incidence =1;
//
//    float R0 = pow(1.0 - incidence, 2.0) / pow(1.0 + incidence, 2.0);
//    float cosAngle = 1 - saturate(dot(vLight, vNormal));
//    float result = cosAngle * cosAngle;
//    
//    result = result * result;
//    result = result * cosAngle;
//    result = saturate(mad(result, 1 - saturate(R0), R0));
//    return result;
//    
//}




float4 ComputePrimTexControl(float2 uv, EffectPrimTexController effStruct,float time)
{

    float2 mixuv;
    mixuv.x = uv.x * effStruct.tilingX;
    mixuv.y = uv.y * effStruct.tilingY;
    
    float2 speed_control = effStruct.textureSpeed;
    
    speed_control = speed_control * time;
    
    mixuv = mixuv + speed_control;
    mixuv = mixuv + effStruct.extend_value;
    
    float4 color = Ky_PrimNoiseMap.Sample(LinearSampler, mixuv);
    
    return color;
    
}




float4 ComputeTexControl(float2 uv, EffectTexController effStruct, float time)
{

    float2 mixuv;
    mixuv.x = uv.x * effStruct.tilingX;
    mixuv.y = uv.y * effStruct.tilingY;
    
    float2 speed_control =  effStruct.textureSpeed;
    float2 offset = { effStruct.offsetX, effStruct.offSetY };
    
    speed_control = speed_control * time;
    offset = speed_control + offset;
    mixuv = offset + mixuv;
    
    mixuv.y = pow(mixuv.y, effStruct.y_compression_scalar);
    
    mixuv = mixuv + effStruct.extend_value;
    float4 color = Ky_NoiseMap.Sample(LinearSampler, mixuv);
    color = pow(color, effStruct.texPower);
    color = color * effStruct.multiply;
    
    return color;
    
}


float TopUnder_ComputeTexControll(float uv_y, TopUnder_EffectStruct effStruct)// Don't Use MixedUV!!!!!!!
{

    float under_mask = 1.0f - uv_y;
    under_mask = pow(under_mask, effStruct.underMaskPower);
    under_mask = under_mask * effStruct.underMultifly;
    under_mask = clamp(under_mask, 0.0f, 1.0f);
    
    float top_under = pow(uv_y, effStruct.topMaskPower);
    top_under = top_under * effStruct.underMaskPower;
    
    top_under = top_under * under_mask;
    top_under = clamp(top_under, 0.0f, 1.0f);
    
    
    
    return top_under;
    
}

float HightLight_ComputeTexControl(float2 uv, HightLightStruct effStruct, float time)// Don't Use MixedUV!!!!!!!
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


float3 Reflect(float3 incident, float3 normal)
{
    float cosI = dot(normal, incident);
    return incident - 2 * cosI * normal;
}




float RadialGradientExponetial(float2 uv, RadialGradientExponetialDesc desc)// Don't Use MixedUV!!!!!!!
{
    float dist = distance(uv, desc.center);
    dist = dist * 1.0f;
    dist = dist / desc.radius;
    if (desc.doInvert)
    {
        dist = ExponetialDensity(dist, desc.density, true);
    }
    else
    {
        dist = 1 - dist;
        dist = ExponetialDensity(dist, desc.density, true);
        dist = 1 - dist;
    }
    return dist;
}


float2 VectorToRadialValue(VectorToRadialValueDesc desc)// Don't Use MixedUV!!!!!!!
{
    float2 mixuv = mul(desc.uv, desc.power);
    mixuv = mixuv - float2(desc.power, desc.power);
    
    float opacity = atan2(mixuv.r, mixuv.g);
    
    if (opacity > 0)
    {
        opacity = opacity / desc.detail;
    }
    else
    {
        opacity = opacity + desc.detail;
        opacity = opacity / desc.detail;
    }
    mixuv = mul(mixuv.r, mixuv.g);
    float scalar = mixuv.r + mixuv.g;
    scalar = sqrt(scalar);
    
    scalar = pow(scalar, desc.density);
    
    if (desc.doCoordinate)
    {
        mixuv = float2(opacity, scalar);
        
    }
    else
    {
        mixuv = float2(scalar, opacity);
    }
    return mixuv;
}



#endif

