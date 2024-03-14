#pragma once
#include "ParticleObj.h"
class FireStorm : public ParticleObj
{
private:
	shared_ptr<Model> particleModel;
	ComPtr<ID3DX11EffectShaderResourceVariable> noiseSRV;
	ComPtr<ID3DX11EffectShaderResourceVariable> primNoiseSRV;
	ComPtr<ID3DX11EffectShaderResourceVariable> hightLightSRV;
	shared_ptr<Texture> noiseTexture;
	shared_ptr<ConstantBuffer<FresnelDesc>> fresnelData;
	ComPtr<ID3DX11EffectConstantBuffer> fresnelBuffer;
	FresnelDesc _fresneldesc;
public:
	virtual void Update() override;
	virtual void LateUpdate() override;

	virtual void OnDestroy(ParticleInstance& instance) override;
public:
	FireStorm();
	~FireStorm();
};

