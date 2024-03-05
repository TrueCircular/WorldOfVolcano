#include "Global.fx"

Texture2D Ky_PrimNoiseMap;

cbuffer ky_Effectbuffer_Prim
{
    //texture tiling, time, texturespeed
    float3 prim_tile_time_speed;
};

float4 KyPrim_ComputeTexControll(float2 uv)
{
    float2 mixuv = uv * prim_tile_time_speed.x;
    
    float2 speed_control = { prim_tile_time_speed.z, prim_tile_time_speed.z };
    float2 offset = speed_control;
    speed_control = speed_control * prim_tile_time_speed.y;
    offset = speed_control + offset;
    mixuv = offset + mixuv;
    
    float4 color = NoiseMap.Sample(LinearSampler,mixuv);
    
    
    return color;
    
}

