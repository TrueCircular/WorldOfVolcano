#pragma once
#include "ParticleObj.h"
class FireStorm : public ParticleObj
{
private:
	shared_ptr<Model> particleModel;
	ComPtr<ID3DX11EffectShaderResourceVariable> noiseSRV;
	shared_ptr<ConstantBuffer<FresnelDesc>> fresnelBuffer;

	ColorDesc _desc;
	_desc.baseColor = Vec4(1, 0, 0, 1);
	_desc.subColor = Vec4(0.8, 0.2, 0.2, 1);
	_materialBuffer->CopyData(_desc);
	data->SetConstantBuffer(_materialBuffer->GetBuffer().Get());
};

