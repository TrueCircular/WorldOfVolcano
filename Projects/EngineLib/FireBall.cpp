#include "pch.h"
#include "FireBall.h"
#include "Utils.h"
#include "CharacterController.h"
#include "PlayerController.h"
#include "AIController.h"


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
			Vec3 velocity = instanceList[i]->targetTransform->GetLocalPosition() - instanceList[i]->particleTransform->GetLocalPosition();
			velocity.Normalize();
			//Vec3 up(0,1,0);
			//if (instanceList[i]->parentTransform) {
			//	up = instanceList[i]->parentTransform->GetLookVector();
			//}
//			Quaternion::
			//Matrix lookMat= Matrix::CreateLookAt(instanceList[i]->particleTransform->GetLocalPosition(), instanceList[i]->targetTransform->GetLocalPosition(),
			//	up);
			//Quaternion qlookat;
			//Vec3 trans;
			//Vec3 scale;
			//lookMat.Decompose(scale, qlookat,trans );
			//Vec3 rotate;// = Utils::QuadToYawPitchRoll(qlookat); //Yaw,Pitch,Roll;
			////TODO Need To mul to Model abRotate;
			//
			////
			//rotate.x += ::XMConvertToRadians(90.f);
			////rotate.y += ::XMConvertToRadians(90.f);
			//rotate.z = 0;
			//
			//Quaternion qut = Quaternion::CreateFromYawPitchRoll(90,90,0);
			//qlookat = Quaternion::Concatenate(qlookat, qut);
			//rotate =	Utils::QuadToYawPitchRoll(qlookat);
			//instanceList[i]->particleTransform->SetLocalRotation(rotate);
			velocity = velocity* (instanceList[i]->speed * MANAGER_TIME()->GetDeltaTime());
			Vec3 pos = instanceList[i]->particleTransform->GetLocalPosition();
			pos += velocity;
			Vec3 targetPos = instanceList[i]->targetTransform->GetLocalPosition();
			float distance = Vec3::Distance(pos,targetPos);
			if (distance  < instanceList[i]->speed * MANAGER_TIME()->GetDeltaTime()) {
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
	auto clapParticle = MANATER_PARTICLE()->GetParticleFromName(L"Explode");
	shared_ptr<Transform> pos2 = make_shared<Transform>();
	pos2->SetLocalPosition(instance->particleTransform->GetPosition());
	shared_ptr<ParticleInstance>  instancedata2 = make_shared<ParticleInstance>(3, pos2, nullptr, 0);
	clapParticle->AddParticle(instancedata2);

	if (_target != nullptr)
	{
		auto controller = _target->GetComponent<CharacterController>();
		controller->TakeDamage(_target, _damage);
	}
}

void FireBall::AddParticle(shared_ptr<ParticleInstance> data)
{

	Vec3 rotate =data->particleTransform->GetLocalRotation();
	rotate.x += ::XMConvertToRadians(180);
	//Vec3 a = data->particleTransform->GetLocalPosition();
	//Vec3 b = data->targetTransform->GetLocalPosition();
	//Vec2 ax(a.x, a.y);
	//Vec2 bx(b.x, b.y);
	//
	//float dot=ax.Dot(bx);
	//float v1 = sqrt(ax.x*ax.x + ax.y*ax.y);
	//float v2 = sqrt(bx.x * bx.x + bx.y * bx.y);
	//float angle = acos(dot / (v1 * v2));
	//auto Cr = [](Vec2& a, Vec2& b) {
	//	return a.x * b.y - a.y * b.x;
	//};
	//int direction = (Cr(ax,bx) >0) ? 1 : (Cr(ax,bx) < 0) ? -1 : 0;
	//if (direction < 0) {

	//	rotate.x -= 2 - angle;
	//}
	//else
	//{

	//	rotate.x -= 2 - angle;
	//}
	data->particleTransform->SetLocalRotation(rotate);
	{
		instanceList.push_back(data);
		++instanceCounter;
	}

	//
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
