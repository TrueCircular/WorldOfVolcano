#include "Global.fx"

Texture2D Ky_NoiseMap;

cbuffer ky_Effectbuffer
{
    //texture tiling, time, texturespeed
    float3 ky_tile_time_speed;
    // ycompression, power,multiply
    float3 ky_comp_power_multi;
    
    //float texture_tiling_scalar;
    //float2 time_scalar_textureSpeed;
    //float texture_y_compression_scalar;
    //float extend_value;
    //float2 texPower_Muliply;
};

float4 Ky_ComputeTexControll(float2 uv)
{
    float2 mixuv = uv * ky_tile_time_speed.x;
    
    float2 speed_control = { ky_tile_time_speed.z, ky_tile_time_speed.z };
    float2 offset = speed_control;
    speed_control = speed_control * ky_tile_time_speed.y;
    offset = speed_control + offset;
    mixuv = offset + mixuv;
    
    mixuv.y = pow(mixuv.y, ky_comp_power_multi.x);
    
//    mixuv = mixuv + float2(comp_power_multi.y, comp_power_multi.y);

    mixuv = mixuv + ky_comp_power_multi.y;
    float4 color = Ky_NoiseMap.Sample(LinearSampler, mixuv);
    color = pow(color, ky_comp_power_multi.y);
    color = color * ky_comp_power_multi.z;
    
    return color;
    
}

