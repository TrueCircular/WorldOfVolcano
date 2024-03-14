#include "Global.fx"


struct TopUnder_EffectStruct
{
    float topMaskPower;//=1;
    float underMaskPower;//=1;
    float topMultifly; //=0;
    float underMultifly; //=1;
};

float TopUnder_ComputeTexControll(float uv_y,TopUnder_EffectStruct effStruct)// Don't Use MixedUV!!!!!!!
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



//cbuffer TopUnder_Effectbuffer
//{
//    //texture Muilti&mask
//    float2 multiply_powermask;
//};
//
//float TopUnder_ComputeTexControll(float uv_y)// Don't Use MixedUV!!!!!!!
//{
//
//    float under_mask = 1.0f - uv_y;
//    under_mask = pow(under_mask, multiply_powermask.y);
//    under_mask = under_mask * multiply_powermask.x;
//    under_mask = clamp(under_mask, 0.0f, 1.0f);
//    
//    float top_under = pow(uv_y, multiply_powermask.x);
//    top_under = top_under * multiply_powermask.x;
//    
//    top_under = top_under * under_mask;
//    top_under = clamp(top_under, 0.0f, 1.0f);
//    
//    
//    
//    return top_under;
//    
//}

