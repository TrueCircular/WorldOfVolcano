#include "pch.h"
#include "MagicCircle.h"


void MagicCircle::Update()
{
	ParticleObj::Update();

	for (int i = 0; i < instanceList.size(); ++i) {
		if (instanceList[i]->isDestroy) {
			OnDestroy(instanceList[i]);
			auto iter = instanceList.begin() + i;
			instanceList.erase(iter);
			instanceCounter--;
			--i;
			continue;
		}

		instanceList[i]->data.world = instanceList[i]->particleTransform->GetWorldMatrix();
	}
	instanceBuffer->ClearData();
}

void MagicCircle::LateUpdate()
{
	ParticleObj::LateUpdate();

	if (!instanceList.empty())
		//auto ctimes = shader->GetScalar("duration")->SetFloat(instanceList[0].duration);
	circleSRV->SetResource(circleTexture->GetTexture().Get());
	NoiseSRV->SetResource(circleTexture->GetTexture().Get());
	meshRenderer->Render(instanceList);
}

void MagicCircle::OnDestroy(shared_ptr<ParticleInstance>& instance)
{
}

MagicCircle::MagicCircle()
{
	instanceBuffer = make_shared<ParticleInstancingBuffer>();
	particleMesh = MANAGER_RESOURCES()->GetResource<Mesh>(L"QuadPlain");

	shader = MANAGER_RESOURCES()->GetResource<Shader>(L"MagicCircleEffect");
	if (shader == nullptr) {
		shader = make_shared<Shader>(L"MagicCircle.fx");
		MANAGER_RESOURCES()->AddResource<Shader>(L"MagicCircle", shader);
	}
	_colorDesc.baseColor = Vec4(1, 0.2, 0.2, 1);
	_colorDesc.subColor = Vec4(1, 1, 0, 1);
	colorBuffer = shader->GetConstantBuffer("ColorBuffer");
	colorData = make_shared<ConstantBuffer<ColorDesc>>();
	colorData->CreateConstantBuffer();

	circleTexture = MANAGER_RESOURCES()->GetResource<Texture>(L"MagicCircle");
	if (circleTexture == nullptr) {
		circleTexture = make_shared<Texture>();
		circleTexture->Load(L"../../Resources/Texture/Effect/T_ky_magicCircle020.PNG");
		MANAGER_RESOURCES()->AddResource(L"MagicCircle", circleTexture);
	}

	circleSRV = shader->GetSRV("EffectMap");
	NoiseSRV = shader->GetSRV("EffectMap");
	meshRenderer = make_shared<ParticleMeshRenderer>();
	meshRenderer->SetMesh(particleMesh);
	meshRenderer->SetShader(shader);
	meshRenderer->SetBuffer(instanceBuffer);
	meshRenderer->SetPass(0);
}

MagicCircle::~MagicCircle()
{
}
