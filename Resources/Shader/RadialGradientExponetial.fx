#include "Global.fx"


struct RadiusGradientExponetialDesc
{
    float2 center;
    float radius;
    float density;
    bool doInvert;
};

RadiusGradientExponetialDesc Init_RadiusGradientExponetialDesc()
{
    RadiusGradientExponetialDesc desc;
    desc.center = float2(0.5f, 0.5f);
    desc.radius = 0.5f;
    desc.density = 2.33;
    desc.doInvert = true;
    
    return desc;
}
float ExponetialDensity(float depth, float density, bool doInvert)
{
    float scalar=0;
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

float RadiusGradientExponetial(float2 uv,RadiusGradientExponetialDesc desc)// Don't Use MixedUV!!!!!!!
{
    float dist = distance(uv, desc.center);
    dist = dist*1.0f;
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

