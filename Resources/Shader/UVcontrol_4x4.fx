#include "Global.fx"


struct Uvcontrol44Desc
{
    float uvExtention;
    bool pickUpCh;
    float uvNum;
};
Texture2D UVImage;
float4 UV44Control(float2 uv, Uvcontrol44Desc desc)// Don't Use MixedUV!!!!!!!
{

    float scalarA = desc.uvNum - 16.0f;
    if (scalarA > 16.0f)
    {
        
    }
    else
    {
        scalarA = 16.0f;
    }
    float scalarB = desc.uvNum - 32.0f;
    if (scalarB > 32.0f)
    {
        
    }
    else
    {
        scalarB = scalarA;
    }
    float scalarC = 0.0f;
    if (scalarB > 4.0f)
    {
        scalarC = 1;
    }
    
    if (scalarB > 8.0f)
    {
        scalarC = 2.0f;
    }
    if (scalarC > 12.0f)
    {
        scalarC = 3.0f;
    }
    float2 uvDynamic = float2(desc.uvNum, scalarC);
    uvDynamic = floor(uvDynamic);
    uvDynamic = uvDynamic / 4.0f;
    uvDynamic = float2(desc.uvExtention, desc.uvExtention) + uvDynamic;
    uvDynamic = uv + uvDynamic;
    float4 color = UVImage.Sample(LinearSampler, uvDynamic);
    
    float3 rgbColor;
    if (desc.pickUpCh)
    {
        rgbColor = color.rgb;
    }
    else
    {
        rgbColor = float3(color.r, color.r, color.r);
    }
    if (uvDynamic.x > 16.0f &&uvDynamic.y > 16.0f)
    {
        rgbColor = float3(color.g, color.g, color.g);
    }
    if (uvDynamic.x > 32.0f && uvDynamic.y > 32.0f)
    {
        rgbColor = float3(color.b, color.b, color.b);
    }
    color.rgb = rgbColor;
    
    return color;
    
}

