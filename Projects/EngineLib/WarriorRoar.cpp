#include "pch.h"
#include "WarriorRoar.h"

void WarriorRoar::RemoveTweenData(int index)
{
	auto iter = eachTweenData.begin() + index;
	eachTweenData.erase(iter);
}

void WarriorRoar::AddParticle(shared_ptr<ParticleInstance> data, shared_ptr<TweenDesc> desc)
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
		if (instanceList[i]->isDestroy) {
			OnDestroy(instanceList[i]);
			RemoveTweenData(i);
			auto iter = instanceList.begin() + i;
			instanceList.erase(iter);
			instanceCounter--;
			--i;
			continue;
		}
//		instanceList[i]->particleTransform->UpdateTransform();
		Vec3 scale = instanceList[i]->particleTransform->GetLocalScale();
		
		scale.x = 0.1f+ instanceList[i]->data.currentime*0.1;
		scale.y = 0.1f+ instanceList[i]->data.currentime * 0.1;
		scale.z = 0.1f+ instanceList[i]->data.currentime * 0.1;
		instanceList[i]->particleTransform->SetScale(scale);
		instanceList[i]->particleTransform->UpdateTransform();
		instanceList[i]->data.world = instanceList[i]->particleTransform->GetWorldMatrix();
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

void WarriorRoar::OnDestroy(shared_ptr<ParticleInstance>& instance)
{

}

WarriorRoar::WarriorRoar()
{
	instanceBuffer = make_shared<ParticleInstancingBuffer>();
//	shader = MANAGER_RESOURCES()->GetResource<Shader>(L"Default");
	shader = MANAGER_RESOURCES()->GetResource<Shader>(L"WarriorRoar");
	if (shader == nullptr) {
		shader = make_shared<Shader>(L"WarriorRoar.fx");
		MANAGER_RESOURCES()->AddResource<Shader>(L"WarriorRoar", shader);
	}
	_colorDesc.baseColor = Vec4(1,0.5,0 , 1);
	_colorDesc.subColor = Vec4(1, 1, 0, 1);
	colorBuffer = shader->GetConstantBuffer("ColorBuffer");
	colorData = make_shared<ConstantBuffer<ColorDesc>>();
	colorData->CreateConstantBuffer();

	maskTexture = MANAGER_RESOURCES()->GetResource<Texture>(L"NoiseMap17");
	if (maskTexture == nullptr) {
		maskTexture = make_shared<Texture>();
		maskTexture->Load(wstring(RESOURCES_ADDR_TEXTURE_EFFECT) + L"T_ky_noise17.PNG");
		MANAGER_RESOURCES()->AddResource(L"NoiseMap17", maskTexture);
	}

	animRenderer = make_shared<ParticleAnimRenderer>();
	animRenderer->SetShader(shader);
	animRenderer->SetBuffer(instanceBuffer);
	animRenderer->SetPass(0);
}

WarriorRoar::~WarriorRoar()
{

}
