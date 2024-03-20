#include "pch.h"
#include "FireStorm.h"

void FireStorm::Update()
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
		Vec3 rotation = instanceList[i]->particleTransform->GetLocalRotation();

		rotation.x = ::XMConvertToRadians(90.f);
		rotation.y = ::XMConvertToRadians(90.f);
		rotation.z += (5.5f / PI) * (MANAGER_TIME()->GetDeltaTime());
		instanceList[i]->particleTransform->SetLocalRotation(rotation);
		if (instanceList[i]->isTargeting) {
			Vec3 velocity = instanceList[i]->particleTransform->GetLocalPosition() - instanceList[i]->targetTransform->GetLocalPosition();
			velocity.Normalize();
			velocity = velocity* (instanceList[i]->speed * MANAGER_TIME()->GetDeltaTime());
			Vec3 pos = instanceList[i]->particleTransform->GetLocalPosition();
			pos += velocity;
			//Vec3 targetPos = instanceList[i]->targetTransform->GetLocalPosition();
			//float distance = Vec3::Distance(pos,targetPos);
			//if (distance < instanceList[i]->speed) {
			//	OnDestroy(instanceList[i]);
			//	auto iter = instanceList.begin() + i;
			//	instanceList.erase(iter);
			//	--i;
			//}
			instanceList[i]->particleTransform->SetLocalPosition(pos);
		}
		instanceList[i]->particleTransform->Update();
		instanceList[i]->data.world = instanceList[i]->particleTransform->GetWorldMatrix();
	}
	instanceBuffer->ClearData();
}

void FireStorm::LateUpdate()
{
	ParticleObj::LateUpdate();

	_fresneldesc.eyeLook = MANAGER_SCENE()->GetCurrentScene()->GetCamera()->GetCamera()->GetCameraLookVector();
	_fresneldesc.eyePos = MANAGER_SCENE()->GetCurrentScene()->GetCamera()->GetCamera()->GetCameraPosition();
	if (fresnelData) {
		fresnelData->CopyData(_fresneldesc);
		if (fresnelBuffer)
			fresnelBuffer->SetConstantBuffer(fresnelData->GetBuffer().Get());
	}
	noiseSRV->SetResource(noiseTexture->GetTexture().Get());
	primNoiseSRV->SetResource(noiseTexture->GetTexture().Get());
	hightLightSRV->SetResource(noiseTexture->GetTexture().Get());;
	staticRenderer->Render(instanceList);
}

void FireStorm::OnDestroy(shared_ptr<ParticleInstance>& instance)
{
}

FireStorm::FireStorm()
{
	instanceBuffer = make_shared<ParticleInstancingBuffer>();
	particleModel = MANAGER_RESOURCES()->GetResource<Model>(L"Tornado");
	if (particleModel == nullptr) {
		particleModel = make_shared<Model>();
		wstring modelPath = RESOURCES_ADDR_MESH_STATIC;
		modelPath += L"SM_ky_tornado07/SM_ky_tornado07.mesh";
		particleModel->ReadModel(modelPath);
		MANAGER_RESOURCES()->AddResource<Model>(L"Tornado", particleModel);
	}
	shader = MANAGER_RESOURCES()->GetResource<Shader>(L"StormEffect");
	if (shader == nullptr) {
		shader = make_shared<Shader>(L"Storm.fx");
		MANAGER_RESOURCES()->AddResource<Shader>(L"StormEffect", shader);
	}
	_colorDesc.baseColor = Vec4(1, 0.2, 0.2, 1);
	_colorDesc.subColor = Vec4(0.8, 0.3, 0.3, 1);
	colorBuffer= shader->GetConstantBuffer("ColorBuffer"); 
	colorData = make_shared<ConstantBuffer<ColorDesc>>();
	colorData->CreateConstantBuffer();
	_fresneldesc.eyeLook = {};
	_fresneldesc.eyePos = {};

	fresnelBuffer = shader->GetConstantBuffer("FresnelBuffer");
	fresnelData = make_shared<ConstantBuffer<FresnelDesc>>();
	fresnelData->CreateConstantBuffer();
	noiseTexture = MANAGER_RESOURCES()->GetResource<Texture>(L"NoiseMap17");
	if (noiseTexture == nullptr) {
		noiseTexture = make_shared<Texture>();
		noiseTexture->Load(L"../../Resources/Texture/Effect/T_ky_noise17.PNG");
		MANAGER_RESOURCES()->AddResource(L"NoiseMap17", noiseTexture);
	}

	noiseSRV= shader->GetSRV("HightLightMap");
	primNoiseSRV= shader->GetSRV("Ky_PrimNoiseMap");
	hightLightSRV= shader->GetSRV("Ky_NoiseMap");
	staticRenderer = make_shared<ParticleStaticRenderer>();
	staticRenderer->SetModel(particleModel);
	staticRenderer->SetShader(shader);
	staticRenderer->SetBuffer(instanceBuffer); 
	staticRenderer->SetPass(1);
}

FireStorm::~FireStorm()
{
}
