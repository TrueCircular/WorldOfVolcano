#include "pch.h"
#include "Smoke1.h"

void Smoke1::Update()
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
		if (instanceList[i]->isTargeting) {
			Vec3 velocity = instanceList[i]->targetTransform->GetLocalPosition() - instanceList[i]->particleTransform->GetLocalPosition();
			velocity.Normalize();
			velocity = velocity * (instanceList[i]->speed * MANAGER_TIME()->GetDeltaTime());
			Vec3 pos = instanceList[i]->particleTransform->GetLocalPosition();
			pos += velocity;

			instanceList[i]->particleTransform->SetLocalPosition(pos);
		}
		Vec3 insPos= instanceList[i]->particleTransform->GetPosition();
		auto look = MANAGER_SCENE()->GetCurrentScene()->GetCamera()->GetCamera()->GetCameraLookVector();
		Matrix billmat = Matrix::CreateBillboard(insPos, MANAGER_SCENE()->GetCurrentScene()->GetCamera()->GetCamera()->GetCameraPosition(), MANAGER_SCENE()->GetCurrentScene()->GetCamera()->GetCamera()->GetCameraUpVector(), &look);

		Vec3 dePos;
		Vec3 deScale;
		Quaternion deRot;
		billmat.Decompose(deScale, deRot, dePos);
		Vec3 scales =instanceList[i]->particleTransform->GetScale();
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

void Smoke1::LateUpdate()
{
	ParticleObj::LateUpdate();

	if (!instanceList.empty())
		auto ctimes = shader->GetScalar("duration")->SetFloat(instanceList[0]->duration);
	noiseSRV->SetResource(noiseTexture->GetTexture().Get());
	smokeSRV->SetResource(smokeTexture->GetTexture().Get());
	meshRenderer->Render(instanceList);
}

void Smoke1::OnDestroy(shared_ptr<ParticleInstance>& instance)
{
}

Smoke1::Smoke1()
{
	instanceBuffer = make_shared<ParticleInstancingBuffer>();
	particleMesh = MANAGER_RESOURCES()->GetResource<Mesh>(L"Quad");

	shader = MANAGER_RESOURCES()->GetResource<Shader>(L"Smoke1");
	if (shader == nullptr) {
		shader = make_shared<Shader>(L"Smoke.fx");
		MANAGER_RESOURCES()->AddResource<Shader>(L"Smoke1", shader);
	}
	_colorDesc.baseColor = Vec4(0, 0.7, 0.8, 1);
	_colorDesc.subColor = Vec4(1, 1, 0, 1);
	colorBuffer = shader->GetConstantBuffer("ColorBuffer");
	colorData = make_shared<ConstantBuffer<ColorDesc>>();
	colorData->CreateConstantBuffer();

	noiseTexture = MANAGER_RESOURCES()->GetResource<Texture>(L"NoiseMap6");
	if (noiseTexture == nullptr) {
		noiseTexture = make_shared<Texture>();
		noiseTexture->Load(wstring(RESOURCES_ADDR_TEXTURE_EFFECT) + L"T_ky_noise6.PNG");
		MANAGER_RESOURCES()->AddResource(L"NoiseMap6", noiseTexture);
	}
	smokeTexture = MANAGER_RESOURCES()->GetResource<Texture>(L"Smoke1");
	if (smokeTexture == nullptr) {
		smokeTexture = make_shared<Texture>();
		smokeTexture->Load(wstring(RESOURCES_ADDR_TEXTURE_EFFECT) + L"T_ky_smoke1.PNG");
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

Smoke1::~Smoke1()
{
}
