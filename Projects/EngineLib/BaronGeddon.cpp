#include "pch.h"
#include "BaronGeddon.h"
#include "SphereCollider.h"
#include "HeightGetter.h"
#include "CharacterInfo.h"
#include "AIController.h"

BaronGeddon::BaronGeddon()
{
}

BaronGeddon::~BaronGeddon()
{
}

void BaronGeddon::CharacterInit()
{
	auto chinfo = make_shared<CharacterInfo>();
	{
		wstring LoadPath = DATA_ADDR_UNIT;
		LoadPath += L"BaronGeddon/Information.xml";
		chinfo->LoadCharacterInformationFromFile(LoadPath);
		AddComponent(chinfo);
	}

	//Character
	{
		_childModel = make_shared<GameObject>();

		shared_ptr<Model> model = make_shared<Model>();
		{
			AddModelAndMaterial(model, L"BaronGeddon");
			AddAnimation(model, L"BaronGeddon", L"Stand");
			AddAnimation(model, L"BaronGeddon", L"Walk");
			AddAnimation(model, L"BaronGeddon", L"Run");
			AddAnimation(model, L"BaronGeddon", L"Damaged");
			AddAnimation(model, L"BaronGeddon", L"Death");
			AddAnimation(model, L"BaronGeddon", L"Battle");
			AddAnimation(model, L"BaronGeddon", L"Attack1");
			AddAnimation(model, L"BaronGeddon", L"Attack2");
		}
		const auto& shader = MANAGER_RESOURCES()->GetResource<Shader>(L"Default");
		shared_ptr<ModelAnimator> tempAnimator = make_shared<ModelAnimator>(shader);
		{
			tempAnimator->SetModel(model);
			tempAnimator->SetPlay(true);
			tempAnimator->SetLoop(true);
			tempAnimator->SetPass(2);
		}
		_childModel->AddComponent(tempAnimator);
		_childModel->Awake();
		_childModel->Start();
		_childModel->SetName(L"Model");

		Vec3 rot = _childModel->GetTransform()->GetLocalRotation();
		rot.x += ::XMConvertToRadians(90.f);
		rot.y -= ::XMConvertToRadians(90.f);
		_childModel->GetTransform()->SetLocalScale(Vec3(0.5f));
		_childModel->GetTransform()->SetLocalRotation(rot);
		_childModel->GetTransform()->SetLocalPosition(Vec3(0, 35, 0));
		AddChild(_childModel);
	}
	{
		auto height = make_shared<HeightGetter>();
		height->Set(MANAGER_SCENE()->GetCurrentScene()->GetCurrentTerrain().get());
		AddComponent(height);
	}

	{
		auto collider = make_shared<SphereCollider>();
		collider->SetRadius(35.f);
		AddComponent(collider);
	}

	auto aiCon = make_shared<AIController>();
	{
		aiCon->SetAIType(AIType::EnemyUnit);
		AddComponent(aiCon);
	}

	SetName(L"BaronGeddon");
}

void BaronGeddon::Awake()
{
	Super::Awake();
	CharacterInit();
}

void BaronGeddon::Start()
{
	Super::Start();
}

void BaronGeddon::FixedUpdate()
{
	Super::FixedUpdate();
}

void BaronGeddon::Update()
{
	Super::Update();
}

void BaronGeddon::LateUpdate()
{
	Super::LateUpdate();
}
