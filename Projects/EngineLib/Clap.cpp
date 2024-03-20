#include "pch.h"
#include "Clap.h"

void Clap::Update()
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

void Clap::LateUpdate()
{
	ParticleObj::LateUpdate();

	if(!instanceList.empty())
	auto ctimes = shader->GetScalar("duration")->SetFloat(instanceList[0]->duration);
	quakeSRV->SetResource(quakeTexture->GetTexture().Get());
	meshRenderer->Render(instanceList);
}

void Clap::OnDestroy(shared_ptr<ParticleInstance>& instance)
{
}

Clap::Clap()
{
	instanceBuffer = make_shared<ParticleInstancingBuffer>();
	particleMesh = MANAGER_RESOURCES()->GetResource<Mesh>(L"QuadPlain");

	shader = MANAGER_RESOURCES()->GetResource<Shader>(L"EarthCrackEffect");
	if (shader == nullptr) {
		shader = make_shared<Shader>(L"EarthCrack.fx");
		MANAGER_RESOURCES()->AddResource<Shader>(L"EarthCrackEffect", shader);
	}
	_colorDesc.baseColor = Vec4(1, 1, 0.2, 1);
	_colorDesc.subColor = Vec4(1, 1, 0, 1);
	colorBuffer = shader->GetConstantBuffer("ColorBuffer");
	colorData = make_shared<ConstantBuffer<ColorDesc>>();
	colorData->CreateConstantBuffer();

	quakeTexture = MANAGER_RESOURCES()->GetResource<Texture>(L"EarthCrack");
	if (quakeTexture == nullptr) {
		quakeTexture = make_shared<Texture>();
		quakeTexture->Load(L"../../Resources/Texture/Effect/T_Earth_Crack.PNG");
		MANAGER_RESOURCES()->AddResource(L"EarthCrack", quakeTexture);
	}

	quakeSRV = shader->GetSRV("EffectMap");
	meshRenderer = make_shared<ParticleMeshRenderer>();
	meshRenderer->SetMesh(particleMesh);
	meshRenderer->SetShader(shader);
	meshRenderer->SetBuffer(instanceBuffer);
	meshRenderer->SetPass(0);
}

Clap::~Clap()
{
}
