#include "pch.h"
#include "LineSpark.h"

void LineSpark::Update()
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

void LineSpark::LateUpdate()
{
	ParticleObj::LateUpdate();

	if (!instanceList.empty())
		//auto ctimes = shader->GetScalar("duration")->SetFloat(instanceList[0].duration);
		smokeSRV->SetResource(smokeTexture->GetTexture().Get());
	meshRenderer->Render(instanceList);
}

void LineSpark::OnDestroy(ParticleInstance& instance)
{
}

LineSpark::LineSpark()
{
	instanceBuffer = make_shared<ParticleInstancingBuffer>();
	particleMesh = MANAGER_RESOURCES()->GetResource<Mesh>(L"Quad");

	shader = MANAGER_RESOURCES()->GetResource<Shader>(L"LineSpark");
	if (shader == nullptr) {
		shader = make_shared<Shader>(L"LineSpark.fx");
		MANAGER_RESOURCES()->AddResource<Shader>(L"LineSpark", shader);
	}
	_colorDesc.baseColor = Vec4(1, 1, 1, 1);
	_colorDesc.subColor = Vec4(1, 1, 0, 1);
	colorBuffer = shader->GetConstantBuffer("ColorBuffer");
	colorData = make_shared<ConstantBuffer<ColorDesc>>();
	colorData->CreateConstantBuffer();

	smokeTexture = MANAGER_RESOURCES()->GetResource<Texture>(L"LineSpark");
	if (smokeTexture == nullptr) {
		smokeTexture = make_shared<Texture>();
		smokeTexture->Load(L"../../Resources/Texture/Effect/T_Sparks.PNG");
		MANAGER_RESOURCES()->AddResource(L"LineSpark", smokeTexture);
	}

	smokeSRV = shader->GetSRV("EffectMap");
	meshRenderer = make_shared<ParticleMeshRenderer>();
	meshRenderer->SetMesh(particleMesh);
	meshRenderer->SetShader(shader);
	meshRenderer->SetBuffer(instanceBuffer);
	meshRenderer->SetPass(0);
}

LineSpark::~LineSpark()
{
}
