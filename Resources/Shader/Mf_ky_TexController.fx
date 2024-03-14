#include "Global.fx"

Texture2D Ky_NoiseMap;

//cbuffer ky_Effectbuffer
//{
//    //texture tiling, time, texturespeed
//    float3 ky_tile_time_speed;
//    // ycompression, power,multiply
//    float3 ky_comp_power_multi;
//    
//    //float texture_tiling_scalar;
//    //float2 time_scalar_textureSpeed;
//    //float texture_y_compression_scalar;
//    //float extend_value;
//    //float2 texPower_Muliply;
//};

struct EffectTexController
{
    float tilingX;//=1;
    float tilingY;//=1;
    float offsetX;// = 0;
    float offSetY;// = 0;
    float textureSpeed;//=0;
    float y_compression_scalar;// = 1;
    
    float extend_value;//=0;
    float texPower;//=1;
    float multiply;//=1;
};

float4 ComputeTexControl(float2 uv,EffectTexController effStruct)
{

    float2 mixuv;
    mixuv.x = uv.x * effStruct.tilingX;
    mixuv.y = uv.y * effStruct.tilingY;
    
    float2 speed_control = {effStruct.textureSpeed,effStruct.textureSpeed };
    float2 offset = {effStruct.offsetX,effStruct.offSetY};
    
    speed_control = speed_control * time ;
    offset = speed_control + offset;
    mixuv = offset + mixuv;
    
    mixuv.y = pow(mixuv.y, effStruct.y_compression_scalar);
    
    mixuv = mixuv + effStruct.extend_value;
    float4 color = Ky_NoiseMap.Sample(LinearSampler, mixuv);
    color = pow(color, effStruct.texPower);
    color = color * effStruct.multiply;
    
    return color;
    
}

//float4 Ky_ComputeTexControll(float2 uv)
//{
//    float2 mixuv = uv * ky_tile_time_speed.x;
//    
//    float2 speed_control = { ky_tile_time_speed.z, ky_tile_time_speed.z };
//    float2 offset = speed_control;
//    speed_control = speed_control * ky_tile_time_speed.y;
//    offset = speed_control + offset;
//    mixuv = offset + mixuv;
//    
//    mixuv.y = pow(mixuv.y, ky_comp_power_multi.x);
//    
////    mixuv = mixuv + float2(comp_power_multi.y, comp_power_multi.y);
//
//    mixuv = mixuv + ky_comp_power_multi.y;
//    float4 color = Ky_NoiseMap.Sample(LinearSampler, mixuv);
//    color = pow(color, ky_comp_power_multi.y);
//    color = color * ky_comp_power_multi.z;
//    
//    return color;
//    
//}

