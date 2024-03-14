
float3 ComputeTwosided(float3 sideA, float3 sideB,float3 normal)
{
    float3 inverseNormal = -normal;
    float color=lerp(sideB, sideA, inverseNormal);
    
    return color;
}