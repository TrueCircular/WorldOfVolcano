#include "Global.fx"
#include "Light.fx"

#define MAX_MODEL_TRANSFORMS 250
#define MAX_MODEL_KEYFRAMES 250

Texture2D  DepthMap;

struct KeyframeDesc
{
    int animIndex;
    uint currentFrame;
    uint nextFrame;
    float ratio;
    float sumTime;
    float speed;
    float2 padding;
};

struct TweenFrameDesc
{
    float tweenDuration;
    float tweenRatio;
    float tweenSumTime;
    KeyframeDesc current;
    KeyframeDesc next;
};

cbuffer TweenBuffer
{
    TweenFrameDesc TweenFrames;
};

cbuffer BoneBuffer
{
    matrix BoneTransform[MAX_MODEL_TRANSFORMS];
};

uint BoneIndex;
Texture2DArray TransformMap;
Texture2D dissolve;
float time;

matrix GetAnimationMatrix(VertexTextureNormalTangentBlend input)
{
    float indices[4] = { input.blendIndices.x, input.blendIndices.y, input.blendIndices.z, input.blendIndices.w };
    float weights[4] = { input.blendWeights.x, input.blendWeights.y, input.blendWeights.z, input.blendWeights.w };
    
    int animIndex[2];
    int currFrame[2];
    int nextFrame[2];
    float ratio[2];
    
    animIndex[0] = TweenFrames.current.animIndex;
    currFrame[0] = TweenFrames.current.currentFrame;
    nextFrame[0] = TweenFrames.current.nextFrame;
    ratio[0] = TweenFrames.current.ratio;
    
    animIndex[1] = TweenFrames.next.animIndex;
    currFrame[1] = TweenFrames.next.currentFrame;
    nextFrame[1] = TweenFrames.next.nextFrame;
    ratio[1] = TweenFrames.next.ratio;
    
    float4 c0, c1, c2, c3;
    float4 n0, n1, n2, n3;
    
    matrix curr = 0;
    matrix next = 0;
    matrix transform = 0;
    
    for (int i = 0; i < 4; i++)
    {
        c0 = TransformMap.Load(int4(indices[i] * 4 + 0, currFrame[0], animIndex[0], 0));
        c1 = TransformMap.Load(int4(indices[i] * 4 + 1, currFrame[0], animIndex[0], 0));
        c2 = TransformMap.Load(int4(indices[i] * 4 + 2, currFrame[0], animIndex[0], 0));
        c3 = TransformMap.Load(int4(indices[i] * 4 + 3, currFrame[0], animIndex[0], 0));
        curr = matrix(c0, c1, c2, c3);
        
        n0 = TransformMap.Load(int4(indices[i] * 4 + 0, nextFrame[0], animIndex[0], 0));
        n1 = TransformMap.Load(int4(indices[i] * 4 + 1, nextFrame[0], animIndex[0], 0));
        n2 = TransformMap.Load(int4(indices[i] * 4 + 2, nextFrame[0], animIndex[0], 0));
        n3 = TransformMap.Load(int4(indices[i] * 4 + 3, nextFrame[0], animIndex[0], 0));
        next = matrix(n0, n1, n2, n3);
        matrix result = lerp(curr, next, ratio[0]);
        
        if(animIndex[1] >= 0)
        {
            c0 = TransformMap.Load(int4(indices[i] * 4 + 0, currFrame[1], animIndex[1], 0));
            c1 = TransformMap.Load(int4(indices[i] * 4 + 1, currFrame[1], animIndex[1], 0));
            c2 = TransformMap.Load(int4(indices[i] * 4 + 2, currFrame[1], animIndex[1], 0));
            c3 = TransformMap.Load(int4(indices[i] * 4 + 3, currFrame[1], animIndex[1], 0));
            curr = matrix(c0, c1, c2, c3);
        
            n0 = TransformMap.Load(int4(indices[i] * 4 + 0, nextFrame[1], animIndex[1], 0));
            n1 = TransformMap.Load(int4(indices[i] * 4 + 1, nextFrame[1], animIndex[1], 0));
            n2 = TransformMap.Load(int4(indices[i] * 4 + 2, nextFrame[1], animIndex[1], 0));
            n3 = TransformMap.Load(int4(indices[i] * 4 + 3, nextFrame[1], animIndex[1], 0));
            next = matrix(n0, n1, n2, n3);
            
            matrix nextResult = lerp(curr, next, ratio[1]);
            result = lerp(result, nextResult, TweenFrames.tweenRatio);
        }
        
        transform += mul(weights[i], result);
    }
    
    return transform;
}

MeshOutput SkeletalMesh(VertexTextureNormalTangentBlend input)
{
    MeshOutput output;
    matrix m = GetAnimationMatrix(input);
    
    output.position = mul(input.position, m);
    output.position = mul(output.position, W);
    output.worldPosition = output.position.xyz;
    output.position = mul(output.position, VP);
	
    output.uv = input.uv;
    output.normal = mul(input.normal, (float3x3) W);
    output.tangent = mul(input.tangent, (float3x3) W);
    
    return output;
}

MeshOutput StaticMesh(VertexTextureNormalTangentBlend input)
{
    MeshOutput output;
    
    output.position = mul(input.position, BoneTransform[BoneIndex]);
    output.position = mul(output.position, W);
    output.worldPosition = output.position.xyz;
    output.position = mul(output.position, VP);
	
    output.uv = input.uv;
    output.normal = mul(input.normal, (float3x3) W);
    output.tangent = mul(input.tangent, (float3x3) W);
    
    return output;
}

float4 PS(MeshOutput input) : SV_TARGET
{
    ComputeNormalMapping(input.normal, input.tangent, input.uv);
    float4 color = ComputeLight(input.normal, input.uv, input.worldPosition);

    return color;
}

ShadowOutput SkeletalMeshSHADOW(VertexTextureNormalTangentBlend input)
{
    ShadowOutput output;
    matrix m = GetAnimationMatrix(input);
    
    output.position = mul(input.position, m);
    output.position = mul(output.position, W);
    output.worldPosition = output.position.xyz;
	output.position = mul(output.position, VP);
	
    output.uv = input.uv;
	output.normal = mul(input.normal, (float3x3)W);
    output.tangent = mul(input.tangent, (float3x3)W);
    
    output.texShadow = mul(input.position, matShadow);
	return output;
}

ShadowOutput StaticMeshSHADOW(VertexTextureNormalTangentBlend input)
{
    ShadowOutput output;
    
    output.position = mul(input.position, BoneTransform[BoneIndex]);
    output.position = mul(output.position, W);
    output.worldPosition = output.position.xyz;
    output.position = mul(output.position, VP);
	
    output.uv = input.uv;
    output.normal = mul(input.normal, (float3x3) W);
    output.tangent = mul(input.tangent, (float3x3) W);
    output.texShadow = mul(input.position, matShadow);
    
    return output;
}

float4 PSSHADOW(ShadowOutput input) : SV_TARGET
{
    ComputeNormalMapping(input.normal, input.tangent, input.uv);
    float4 color = ComputeLight(input.normal, input.uv, input.worldPosition);
	{
        float LightAmount = 1.0f;
        float4 vPosLight = input.texShadow;
		
        for (int iLight = 0; iLight < 1; iLight++)
        {
	//transform from RT space to texture space.
            float2 ShadowTexC = vPosLight.xy / vPosLight.w;
	// transform to texel space
            float2 texelpos = SMAP_SIZE * ShadowTexC;
	 // Determine the lerp amounts           
            float2 lerps = frac(texelpos);
	 //read in bilerp stamp, doing the shadow checks
            float sourcevals[4];
            sourcevals[0] = (DepthMap.Sample(ShadowSampler, ShadowTexC)
	+ SHADOW_EPSILON < vPosLight.z / vPosLight.w) ? 0.0f : 1.0f;
            sourcevals[1] = (DepthMap.Sample(ShadowSampler, ShadowTexC
	+ float2(1.0 / SMAP_SIZE, 0)) + SHADOW_EPSILON < vPosLight.z / vPosLight.w) ? 0.0f : 1.0f;
            sourcevals[2] = (DepthMap.Sample(ShadowSampler, ShadowTexC
	+ float2(0, 1.0 / SMAP_SIZE)) + SHADOW_EPSILON < vPosLight.z / vPosLight.w) ? 0.0f : 1.0f;
            sourcevals[3] = (DepthMap.Sample(ShadowSampler, ShadowTexC
	+ float2(1.0 / SMAP_SIZE, 1.0 / SMAP_SIZE)) + SHADOW_EPSILON < vPosLight.z / vPosLight.w) ? 0.0f : 1.0f;
	// lerp between the shadow values to calculate our light amount
            LightAmount = lerp(lerp(sourcevals[0], sourcevals[1], lerps.x),
	lerp(sourcevals[2], sourcevals[3], lerps.x),
	 lerps.y);
        }
        float finalColor = color * max(0.5f, LightAmount);
        return finalColor;
    }
}

float4 DissolvePS(MeshOutput input) : SV_TARGET
{
    ComputeNormalMapping(input.normal, input.tangent, input.uv);
    float4 color = ComputeLight(input.normal, input.uv, input.worldPosition);
 
    float dissolveValue = dissolve.Sample(PointSampler, input.uv).r;
    dissolveValue = saturate(dissolveValue);
    
    float isVisible = dissolveValue - time;
    clip(isVisible);

    return color;
}

float4 PS_RED(MeshOutput input) : SV_TARGET
{
    return float4(1, 0, 0, 1);
}

ShadowDepthOutPut SHADOW_VSSKELETAL(VertexTextureNormalTangentBlend input)
{
    matrix m = GetAnimationMatrix(input);
    
    ShadowDepthOutPut output;
    output.position = mul(input.position, m);
    output.position = mul(output.position, W);
    output.position = mul(output.position, VP);

    output.position = mul(float4(input.position), W);
    output.position = mul(output.position, VP);
	
    output.d = output.position.zw;
    return output;
}
ShadowDepthOutPut SHADOW_VSSTATIC(VertexTextureNormalTangentBlend input)
{
    
    ShadowDepthOutPut output;
    output.position = mul(input.position, BoneTransform[BoneIndex]);
    output.position = mul(output.position, W);
    output.position = mul(output.position, VP);

    output.position = mul(float4(input.position), W);
    output.position = mul(output.position, VP);
	
    output.d = output.position.zw;
    return output;
}

ShadowDepthOutPut SHADOW_VSMESH(VertexTextureNormalTangent input)
{
	
    ShadowDepthOutPut output;
    output.position = mul(float4(input.position), W);
    output.position = mul(output.position, VP);
	
    output.d = output.position.zw;
    return output;
}

float4 PSG(ShadowDepthOutPut input) : SV_TARGET
{
    float shadow = input.d.x / input.d.y;
    return float4(shadow, shadow, shadow, 1.0f);
}


technique11 T0
{
    //StaticMesh
	PASS_VP(P0, StaticMesh, PS)
    //SkeletalMesh
    PASS_VP(P1, SkeletalMesh, PS)
    //WireFrame StaticMesh
	PASS_RS_VP(P2, FillModeWireFrame, StaticMesh, PS_RED)
    //WireFrame SkeletalMesh
	PASS_RS_VP(P3, FillModeWireFrame, SkeletalMesh, PS_RED)
    //Dissolve
    PASS_VP(P4, SkeletalMesh, DissolvePS)

    PASS_RS_SP(P5,ShadowRaster,SHADOW_VSSTATIC,PSG)
    PASS_RS_SP(P6,ShadowRaster,SHADOW_VSSKELETAL,PSG)
    //StaticMesh
	PASS_VP(P7, StaticMeshSHADOW, PSSHADOW)
    //SkeletalMesh
    PASS_VP(P8, SkeletalMeshSHADOW, PSSHADOW)
};
