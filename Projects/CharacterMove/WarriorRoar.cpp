#include "pch.h"
#include "WarriorRoar.h"

void WarriorRoar::RemoveTweenData(int index)
{
	auto iter = eachTweenData.begin() + index;
	eachTweenData.erase(iter);
}

void WarriorRoar::AddParticle(ParticleInstance& data, shared_ptr<TweenDesc> desc)
{
	instanceList.push_back(data);
	AddTweenData(desc);
	++instanceCounter;
}

void WarriorRoar::AddTweenData(shared_ptr<TweenDesc> tweenData)
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
			instanceCounter--;
			--i;
			continue;
		}

		instanceList[i].data.world = instanceList[i].particleTransform->GetWorldMatrix();
	}
	instanceBuffer->ClearData();
}

void WarriorRoar::LateUpdate()
{
	ParticleObj::LateUpdate();

	for (int i = 0; i < instanceCounter; ++i) {
		instanceTweenDesc.tweens[i] = *eachTweenData[i];
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
	shader = MANAGER_RESOURCES()->GetResource<Shader>(L"WarriorRoar");
	if (shader == nullptr) {
		shader = make_shared<Shader>(L"WarriorRoar.fx");
		MANAGER_RESOURCES()->AddResource<Shader>(L"WarriorRoar", shader);
	}
	_colorDesc.baseColor = Vec4(1, 0, 0, 1);
	_colorDesc.subColor = Vec4(1, 1, 0, 1);
	colorBuffer = shader->GetConstantBuffer("ColorBuffer");
	colorData = make_shared<ConstantBuffer<ColorDesc>>();
	colorData->CreateConstantBuffer();

	animRenderer = make_shared<ParticleAnimRenderer>();
	animRenderer->SetShader(shader);
	animRenderer->SetBuffer(instanceBuffer);
	animRenderer->SetPass(0);
}

WarriorRoar::~WarriorRoar()
{

}
