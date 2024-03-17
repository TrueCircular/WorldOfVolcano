#pragma once
#include "ParticleObj.h"
class WarriorRoar: public ParticleObj
{
private:
	shared_ptr<ModelAnimator> particleAnimator;

	ComPtr<ID3DX11EffectShaderResourceVariable> maskSRV;
	shared_ptr<Texture> maskTexture;

	InstancedTweenDesc instanceTweenDesc;
	vector<TweenDesc> eachTweenData;
private:
	void RemoveTweenData(int index);
	virtual void AddParticle(ParticleInstance& data) override {};
public:
	void AddParticle(ParticleInstance& data, TweenDesc& desc);
	void AddTweenData(TweenDesc& tweenData);
	void SetAnimator(shared_ptr<ModelAnimator> _animator);
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void OnDestroy(ParticleInstance& instance) override;
	
	WarriorRoar();
	~WarriorRoar();
};

//if (gameObject->GetModelAnimator() == nullptr)
//{
//	gameObject->GetChildByName(L"Model")->GetModelAnimator()->UpdateTweenData();
//	tweenDesc->tweens[i] = *gameObject->GetChildByName(L"Model")->GetModelAnimator()->GetTweenDesc();
//	data.world = gameObject->GetChildByName(L"Model")->GetTransform()->GetWorldMatrix();
//
//}

