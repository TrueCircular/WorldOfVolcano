#include "pch.h"
#include "RagExplode.h"
#include "Utils.h"

#include "CharacterController.h"
#include "PlayerController.h"
#include "AIController.h"
#include "Sounds.h"
#include "PlayableUnit.h"

void RagExplode::Update()
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
		Vec3 insPos = instanceList[i]->particleTransform->GetPosition();
		auto look = MANAGER_SCENE()->GetCurrentScene()->GetCamera()->GetCamera()->GetCameraLookVector();
		{
			auto particle1 = MANATER_PARTICLE()->GetParticleFromName(L"Smoke1");
			for (int j = 0; j < 4; ++j) {
				shared_ptr<Transform> particletargetTransform = make_shared<Transform>();
				shared_ptr<Transform> particleTransform = make_shared<Transform>();
				Vec3 particlePos = instanceList[i]->particleTransform->GetPosition();
				Vec3 particleScale = instanceList[i]->particleTransform->GetScale();
				Vec3 velocity;
				velocity.x = Utils::Randstep(-1, 1);
				velocity.y = Utils::Randstep(-1, 1);
				velocity.z = Utils::Randstep(-1, 1);
				particleTransform->SetLocalPosition(particlePos);
				particleTransform->SetLocalScale(Vec3(6, 6, 6));
				if (velocity.x < 0) {
					particlePos.x *= -1;
				}
				particlePos.x *= 100;
				if (velocity.y < 0) {
					particlePos.y *= -1;
				}
				particlePos.y *= 100;
				if (velocity.z < 0) {
					particlePos.z *= -1;
				}
				particlePos.z *= 100;

				particletargetTransform->SetLocalPosition(particlePos);
				particletargetTransform->SetLocalScale(particleScale);
				shared_ptr<ParticleInstance> particles;
				particles = make_shared<ParticleInstance>(0.6, particleTransform, particletargetTransform, 10);
				particle1->AddParticle(particles);

			}
		}
		Matrix billmat = Matrix::CreateBillboard(insPos, MANAGER_SCENE()->GetCurrentScene()->GetCamera()->GetCamera()->GetCameraPosition(), MANAGER_SCENE()->GetCurrentScene()->GetCamera()->GetCamera()->GetCameraUpVector(), &look);

		Vec3 dePos;
		Vec3 deScale;
		Quaternion deRot;
		billmat.Decompose(deScale, deRot, dePos);
		Vec3 scales = instanceList[i]->particleTransform->GetScale();
		Matrix scaleMat = Matrix::CreateScale(scales);
		Matrix rotate = Matrix::CreateFromQuaternion(deRot);
		Matrix transmat = Matrix::CreateTranslation(insPos);
		Matrix tempMat;
		tempMat *= scaleMat;
		tempMat *= rotate;
		tempMat *= transmat;

		instanceList[i]->data.world = tempMat;
	}
	instanceBuffer->ClearData();
}

void RagExplode::LateUpdate()
{
	ParticleObj::LateUpdate();

	if (!instanceList.empty())
		auto ctimes = shader->GetScalar("duration")->SetFloat(instanceList[0]->duration);
	noiseSRV->SetResource(noiseTexture->GetTexture().Get());
	smokeSRV->SetResource(smokeTexture->GetTexture().Get());
	meshRenderer->Render(instanceList);
}

void RagExplode::OnDestroy(shared_ptr<ParticleInstance>& instance)
{
	if (_target != nullptr)
	{
		auto tempSound = MANAGER_RESOURCES()->GetResource<Sounds>(L"Ragnaros_Ability1_Impact");
		shared_ptr<Sounds> sound = nullptr;
		if (tempSound == nullptr)
		{
			shared_ptr<Sounds> sound = make_shared<Sounds>();
			wstring soundPath = RESOURCES_ADDR_SOUND;
			soundPath += L"Skill/Ragnaros/Ragnaros_Ability1_Impact.mp3";
			sound->Load(soundPath);
			sound->SetVolume(50);
			MANAGER_RESOURCES()->AddResource<Sounds>(L"Ragnaros_Ability1_Impact", sound);

			sound = sound->Clone();
		}
		else
		{
			sound = tempSound->Clone();
		}

		if (sound != nullptr)
		{
			sound->Play(false);
		}

		_target->GetComponent<CharacterController>()->TakeDamage(_target, _damage);
	}
}

RagExplode::RagExplode()
{
	instanceBuffer = make_shared<ParticleInstancingBuffer>();
	particleMesh = MANAGER_RESOURCES()->GetResource<Mesh>(L"Quad");

	shader = MANAGER_RESOURCES()->GetResource<Shader>(L"RagExplode");
	if (shader == nullptr) {
		shader = make_shared<Shader>(L"RagExplode.fx");
		MANAGER_RESOURCES()->AddResource<Shader>(L"RagExplode", shader);
	}
	_colorDesc.baseColor = Vec4(0, 0.7, 0.8, 1);
	_colorDesc.subColor = Vec4(1, 1, 0, 1);
	colorBuffer = shader->GetConstantBuffer("ColorBuffer");
	colorData = make_shared<ConstantBuffer<ColorDesc>>();
	colorData->CreateConstantBuffer();

	noiseTexture = MANAGER_RESOURCES()->GetResource<Texture>(L"NoiseMap6");
	if (noiseTexture == nullptr) {
		noiseTexture = make_shared<Texture>();
		noiseTexture->Load(L"../../Resources/Texture/Effect/T_ky_noise6.PNG");
		MANAGER_RESOURCES()->AddResource(L"NoiseMap6", noiseTexture);
	}
	smokeTexture = MANAGER_RESOURCES()->GetResource<Texture>(L"Smoke1");
	if (smokeTexture == nullptr) {
		smokeTexture = make_shared<Texture>();
		smokeTexture->Load(L"../../Resources/Texture/Effect/T_ky_smoke1.PNG");
		MANAGER_RESOURCES()->AddResource(L"Smoke1", smokeTexture);
	}

	noiseSRV = shader->GetSRV("NoiseMap");
	smokeSRV = shader->GetSRV("MaskMap");
	meshRenderer = make_shared<ParticleMeshRenderer>();
	meshRenderer->SetMesh(particleMesh);
	meshRenderer->SetShader(shader);
	meshRenderer->SetBuffer(instanceBuffer);
	meshRenderer->SetPass(0);
}

RagExplode::~RagExplode()
{
}
