#include "pch.h"
#include "FireBall.h"
#include "Utils.h"

void FireBall::Update()
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

		if (instanceList[i]->isTargeting) {
			Matrix lookMat= Matrix::CreateLookAt(instanceList[i]->particleTransform->GetLocalPosition(), instanceList[i]->targetTransform->GetLocalPosition(),
				Vec3(0,1,0));
			Quaternion qlookat;
			Vec3 trans;
			Vec3 scale;
			lookMat.Decompose(scale, qlookat,trans );
			Vec3 rotate = Utils::QuadToYawPitchRoll(qlookat); //Yaw,Pitch,Roll;
			//TODO Need To mul to Model abRotate;

			//
			rotate.x += ::XMConvertToRadians(90.f);
			rotate.y += ::XMConvertToRadians(90.f);
			//rotate.z += ::XMConvertToRadians(180.f);
			instanceList[i]->particleTransform->SetLocalRotation(rotate);
			Vec3 velocity = instanceList[i]->targetTransform->GetLocalPosition() -instanceList[i]->particleTransform->GetLocalPosition() ;
			velocity.Normalize();
			velocity = velocity* (instanceList[i]->speed * MANAGER_TIME()->GetDeltaTime());
			Vec3 pos = instanceList[i]->particleTransform->GetLocalPosition();
			pos += velocity;
			Vec3 targetPos = instanceList[i]->targetTransform->GetLocalPosition();
			float distance = Vec3::Distance(pos,targetPos);
			if (distance < instanceList[i]->speed) {
				OnDestroy(instanceList[i]);
				auto iter = instanceList.begin() + i;
				instanceList.erase(iter);
				--i;
				instanceCounter--;
				continue;
			}
			instanceList[i]->particleTransform->SetLocalPosition(pos);
		}
		instanceList[i]->particleTransform->UpdateTransform();
		instanceList[i]->data.world = instanceList[i]->particleTransform->GetWorldMatrix();
	}

}

void FireBall::LateUpdate()
{
	instanceBuffer->ClearData();
	ParticleObj::LateUpdate();
	noiseSRV->SetResource(noiseTexture->GetTexture().Get());
	maskSRV->SetResource(maskTexture->GetTexture().Get());;
	staticRenderer->Render(instanceList);
	
}

void FireBall::OnDestroy(shared_ptr<ParticleInstance>& instance)
{
}

FireBall::FireBall()
{
	instanceBuffer = make_shared<ParticleInstancingBuffer>();
	particleModel = MANAGER_RESOURCES()->GetResource<Model>(L"QuadModel");
	if (particleModel == nullptr) {
		particleModel = make_shared<Model>();
		wstring modelPath = RESOURCES_ADDR_MESH_STATIC;
		modelPath += L"SM_ky_quads/SM_ky_quads.mesh";
		particleModel->ReadModel(modelPath);
	}
	shader = MANAGER_RESOURCES()->GetResource<Shader>(L"FireEffect");
	if (shader == nullptr) {
		shader = make_shared<Shader>(L"FirePillar.fx");
		MANAGER_RESOURCES()->AddResource<Shader>(L"FireEffect", shader);
	}
	_colorDesc.baseColor = Vec4(1, 0, 0, 1);
	_colorDesc.subColor = Vec4(1, 1, 0, 1);
	colorBuffer = shader->GetConstantBuffer("ColorBuffer");
	colorData = make_shared<ConstantBuffer<ColorDesc>>();
	colorData->CreateConstantBuffer();
	noiseTexture = MANAGER_RESOURCES()->GetResource<Texture>(L"NoiseMap18");
	if (noiseTexture == nullptr) {
		noiseTexture = make_shared<Texture>();
		noiseTexture->Load(L"../../Resources/Texture/Effect/T_ky_maskRGB5.PNG");
		MANAGER_RESOURCES()->AddResource(L"NoiseMap18", noiseTexture);
	}
	maskTexture = MANAGER_RESOURCES()->GetResource<Texture>(L"FireMask");
	if (maskTexture == nullptr) {
		maskTexture = make_shared<Texture>();
		maskTexture->Load(L"../../Resources/Texture/Effect/T_ky_flare3.PNG");
		MANAGER_RESOURCES()->AddResource(L"FireMask", maskTexture);
	}
	noiseSRV = shader->GetSRV("FireNoise");
	maskSRV = shader->GetSRV("FireMask");
	staticRenderer = make_shared<ParticleStaticRenderer>();
	staticRenderer->SetModel(particleModel);
	staticRenderer->SetShader(shader);
	staticRenderer->SetBuffer(instanceBuffer);
	staticRenderer->SetPass(1);

}

FireBall::~FireBall()
{
}
