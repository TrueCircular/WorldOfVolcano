#include "Global.fx"

Texture2D Ky_PrimNoiseMap;

struct EffectPrimTexController
{
    float tilingX;// = 1;
    float tilingY;//= 1;
    float textureSpeed;// = 0;
    float extend_value;// = 0;
};

float4 ComputePrimTexControl(float2 uv, EffectPrimTexController effStruct)
{

    float2 mixuv;
    mixuv.x = uv.x * effStruct.tilingX;
    mixuv.y = uv.y * effStruct.tilingY;
    
    float2 speed_control = { effStruct.textureSpeed, effStruct.textureSpeed };
    
    speed_control = speed_control * time;
    
    mixuv = mixuv + speed_control;
    mixuv = mixuv + effStruct.extend_value;
    
    float4 color = Ky_PrimNoiseMap.Sample(LinearSampler, mixuv);
    
    return color;
    
}






//cbuffer ky_Effectbuffer_Prim
//{
//    //texture tiling, time, texturespeed
//    float3 prim_tile_time_speed;
//};
//
//float4 KyPrim_ComputeTexControll(float2 uv)
//{
//    float2 mixuv = uv * prim_tile_time_speed.x;
//    
//    float2 speed_control = { prim_tile_time_speed.z, prim_tile_time_speed.z };
//    float2 offset = speed_control;
//    speed_control = speed_control * prim_tile_time_speed.y;
//    offset = speed_control + offset;
//    mixuv = offset + mixuv;
//    
//    float4 color = NoiseMap.Sample(LinearSampler,mixuv);
//    
//    
//    return color;
//    
//}

