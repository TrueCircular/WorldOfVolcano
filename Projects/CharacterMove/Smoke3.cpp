#include "pch.h"
#include "Smoke3.h"

void Smoke3::Update()
{
	ParticleObj::Update();

	for (int i = 0; i < instanceList.size(); ++i) {
		if (instanceList[i].isDestroy) {
			OnDestroy(instanceList[i]);
			auto iter = instanceList.begin() + i;
			instanceList.erase(iter);
			instanceCounter--;
			--i;
			continue;
		}
		Vec3 insPos = instanceList[i].particleTransform->GetPosition();
		auto look = MANAGER_SCENE()->GetCurrentScene()->GetCamera()->GetCamera()->GetCameraLookVector();
		Matrix billmat = Matrix::CreateBillboard(insPos, MANAGER_SCENE()->GetCurrentScene()->GetCamera()->GetCamera()->GetCameraPosition(), MANAGER_SCENE()->GetCurrentScene()->GetCamera()->GetCamera()->GetCameraUpVector(), &look);

		Vec3 dePos;
		Vec3 deScale;
		Quaternion deRot;
		billmat.Decompose(deScale, deRot, dePos);
		Vec3 scales = instanceList[i].particleTransform->GetScale();
		Matrix scaleMat = Matrix::CreateScale(scales);
		Matrix rotate = Matrix::CreateFromQuaternion(deRot);
		Matrix transmat = Matrix::CreateTranslation(insPos);
		Matrix tempMat;
		tempMat *= scaleMat;
		tempMat *= rotate;
		tempMat *= transmat;

		instanceList[i].data.world = tempMat;
	}
	instanceBuffer->ClearData();
}

void Smoke3::LateUpdate()
{
	ParticleObj::LateUpdate();

	if (!instanceList.empty())
		//auto ctimes = shader->GetScalar("duration")->SetFloat(instanceList[0].duration);
	smokeSRV->SetResource(smokeTexture->GetTexture().Get());
	meshRenderer->Render(instanceList);
}

void Smoke3::OnDestroy(ParticleInstance& instance)
{
}

Smoke3::Smoke3()
{
	instanceBuffer = make_shared<ParticleInstancingBuffer>();
	particleMesh = MANAGER_RESOURCES()->GetResource<Mesh>(L"Quad");

	shader = MANAGER_RESOURCES()->GetResource<Shader>(L"Smoke3");
	if (shader == nullptr) {
		shader = make_shared<Shader>(L"Smoke3.fx");
		MANAGER_RESOURCES()->AddResource<Shader>(L"Smoke3", shader);
	}
	_colorDesc.baseColor = Vec4(1,1,1, 1);
	_colorDesc.subColor = Vec4(1, 1, 0, 1);
	colorBuffer = shader->GetConstantBuffer("ColorBuffer");
	colorData = make_shared<ConstantBuffer<ColorDesc>>();
	colorData->CreateConstantBuffer();

	smokeTexture = MANAGER_RESOURCES()->GetResource<Texture>(L"Smoke3");
	if (smokeTexture == nullptr) {
		smokeTexture = make_shared<Texture>();
		smokeTexture->Load(L"../../Resources/Texture/Effect/T_ky_circleDust.PNG");
		MANAGER_RESOURCES()->AddResource(L"Smoke3", smokeTexture);
	}

	smokeSRV = shader->GetSRV("MaskMap");
	meshRenderer = make_shared<ParticleMeshRenderer>();
	meshRenderer->SetMesh(particleMesh);
	meshRenderer->SetShader(shader);
	meshRenderer->SetBuffer(instanceBuffer);
	meshRenderer->SetPass(0);
}

Smoke3::~Smoke3()
{
}
