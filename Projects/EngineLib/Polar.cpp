#include "pch.h"
#include "Polar.h"
//SM_ky_planeBend

void Polar::Update()
{
	ParticleObj::Update();

	for (int i = 0; i < instanceList.size(); ++i) {
		if (instanceList[i]->isDestroy) {
			OnDestroy(instanceList[i]);
			auto iter = instanceList.begin() + i;
			instanceList.erase(iter);
			--i;
			instanceCounter--;
			continue;
		}

		instanceList[i]->particleTransform->UpdateTransform();
		instanceList[i]->data.world = instanceList[i]->particleTransform->GetWorldMatrix();
	}

}

void Polar::LateUpdate()
{
	instanceBuffer->ClearData();
	ParticleObj::LateUpdate();
	maskSRV->SetResource(maskTexture->GetTexture().Get());;
	staticRenderer->Render(instanceList);

}

void Polar::OnDestroy(shared_ptr<ParticleInstance>& instance)
{
}

Polar::Polar()
{
	instanceBuffer = make_shared<ParticleInstancingBuffer>();
	particleModel = MANAGER_RESOURCES()->GetResource<Model>(L"QuadPlaneBend");
	if (particleModel == nullptr) {
		particleModel = make_shared<Model>();
		wstring modelPath = RESOURCES_ADDR_MESH_STATIC;
		modelPath += L"SM_ky_planeBend/SM_ky_planeBend.mesh";
		particleModel->ReadModel(modelPath);
	}
	shader = MANAGER_RESOURCES()->GetResource<Shader>(L"Polar");
	if (shader == nullptr) {
		shader = make_shared<Shader>(L"Polar.fx");
		MANAGER_RESOURCES()->AddResource<Shader>(L"Polar", shader);
	}
	_colorDesc.baseColor = Vec4(1, 1, 1, 1);
	_colorDesc.subColor = Vec4(1, 1, 0, 1);
	colorBuffer = shader->GetConstantBuffer("ColorBuffer");
	colorData = make_shared<ConstantBuffer<ColorDesc>>();
	colorData->CreateConstantBuffer();
	maskTexture = MANAGER_RESOURCES()->GetResource<Texture>(L"Polar");
	if (maskTexture == nullptr) {
		maskTexture = make_shared<Texture>();
		maskTexture->Load(wstring(RESOURCES_ADDR_TEXTURE_EFFECT) + L"T_ky_decoLinesB.PNG");
		MANAGER_RESOURCES()->AddResource(L"Polar", maskTexture);
	}
	maskSRV = shader->GetSRV("NoiseMap");
	staticRenderer = make_shared<ParticleStaticRenderer>();
	staticRenderer->SetModel(particleModel);
	staticRenderer->SetShader(shader);
	staticRenderer->SetBuffer(instanceBuffer);
	staticRenderer->SetPass(1);

}

Polar::~Polar()
{
}
