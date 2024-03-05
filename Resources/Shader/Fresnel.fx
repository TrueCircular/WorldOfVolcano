
float ComputeFresnel(float3 vLight, float3 vNormal)// Don't Use MixedUV!!!!!!!
{
    float Ref_at_norm_incidence = 1.0f;

    float R0 = pow(1.0 - ref_at_norm_incidence, 2.0) / pow(1.0 + ref_at_norm_incidence, 2.0);
    float cosAngle = 1 - saturate(dot(vLight, vNormal));
    float result = cosAngle * cosAngle;
    
    result = result * result;
    result = result * cosAngle;
    result = saturate(mad(result, 1 - saturate(R0), R0));
    return result;
    
}

float ComputeFresnelforEnvMap(float3 vReflect, float3 vNormal)
{
    float cosAngle = 1 - saturate(dot(vReflect, vNormal));
    float result = pow(cosAngle, 5.0f);
    result = saturate(mad(result, 1 - saturate(F0), F0));
    return result;
}


