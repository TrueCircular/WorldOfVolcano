#include "pch.h"
#include "WarriorRoar.h"

void WarriorRoar::RemoveTweenData(int index)
{
	auto iter = eachTweenData.begin() + index;
	eachTweenData.erase(iter);
}

void WarriorRoar::AddParticle(ParticleInstance& data, TweenDesc& desc)
{
	instanceList.push_back(data);
	AddTweenData(desc);
	++instanceCounter;
}

void WarriorRoar::AddTweenData(TweenDesc& tweenData)
{
	eachTweenData.push_back(tweenData);
}

void WarriorRoar::SetAnimator(shared_ptr<ModelAnimator> _animator)
{
	particleAnimator = _animator;
	if (animRenderer) {
		animRenderer->SetAnimator(_animator);
	}
}

void WarriorRoar::Update()
{
	ParticleObj::Update();

	for (int i = 0; i < instanceList.size(); ++i) {
		if (instanceList[i].isDestroy) {
			OnDestroy(instanceList[i]);
			RemoveTweenData(i);
			auto iter = instanceList.begin() + i;
			instanceList.erase(iter);
			--i;
		}

		instanceList[i].data.world = instanceList[i].particleTransform->GetWorldMatrix();
	}
	instanceBuffer->ClearData();
}

void WarriorRoar::LateUpdate()
{
	ParticleObj::LateUpdate();

	maskSRV->SetResource(maskTexture->GetTexture().Get());
	for (int i = 0; i < instanceCounter; ++i) {
		instanceTweenDesc.tweens[i] = eachTweenData[i];
	}
	shader->PushTweenData(instanceTweenDesc);
	animRenderer->Render(instanceList);
	
}

void WarriorRoar::OnDestroy(ParticleInstance& instance)
{

}

WarriorRoar::WarriorRoar()
{
	instanceBuffer = make_shared<ParticleInstancingBuffer>();
	shader = MANAGER_RESOURCES()->GetResource<Shader>(L"StormEffect");
	if (shader == nullptr) {
		shader = make_shared<Shader>(L"Storm.fx");
		MANAGER_RESOURCES()->AddResource<Shader>(L"StormEffect", shader);
	}
	_colorDesc.baseColor = Vec4(1, 0, 0, 1);
	_colorDesc.subColor = Vec4(1, 1, 0, 1);
	colorBuffer = shader->GetConstantBuffer("ColorBuffer");
	colorData = make_shared<ConstantBuffer<ColorDesc>>();

	maskTexture = MANAGER_RESOURCES()->GetResource<Texture>(L"NoiseMap17");
	if (maskTexture == nullptr) {
		maskTexture = make_shared<Texture>();
		maskTexture->Load(L"../../Resources/Texture/Effect/T_ky_noise17.PNG");
		MANAGER_RESOURCES()->AddResource(L"NoiseMap17", maskTexture);
	}

	maskSRV = shader->GetSRV("HightLightMap");
	animRenderer = make_shared<ParticleAnimRenderer>();
	animRenderer->SetShader(shader);
	animRenderer->SetBuffer(instanceBuffer);
	animRenderer->SetPass(0);
}

WarriorRoar::~WarriorRoar()
{

}
