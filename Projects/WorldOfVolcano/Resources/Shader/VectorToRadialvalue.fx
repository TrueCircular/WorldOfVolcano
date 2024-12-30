#include "Global.fx"


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


