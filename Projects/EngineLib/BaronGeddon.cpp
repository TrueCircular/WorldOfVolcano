#include "pch.h"
#include "BaronGeddon.h"

#include "SphereCollider.h"
#include "HeightGetter.h"
#include "CharacterInfo.h"
#include "AIController.h"

#include "AbilitySlot.h"
#include "AFireBolt.h"
#include "AFireBoltData.h"

BaronGeddon::BaronGeddon()
{
	_objType = ObjectType::EnemyUnit;
}

BaronGeddon::~BaronGeddon()
{
}

void BaronGeddon::CharacterInit()
{
	//Character Info
	auto chinfo = make_shared<CharacterInfo>();
	{
		wstring LoadPath = DATA_ADDR_UNIT;
		LoadPath += L"BaronGeddon/Information.xml";
		chinfo->LoadCharacterInformationFromFile(LoadPath);
		AddComponent(chinfo);
	}

	//Character Model
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
			AddAnimation(model, L"BaronGeddon", L"Attack1");
			AddAnimation(model, L"BaronGeddon", L"Attack2");
			AddAnimation(model, L"BaronGeddon", L"Casting");
			AddAnimation(model, L"BaronGeddon", L"Ability");
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
	//Ability Set
	{
		auto abilitySlot = make_shared<AbilitySlot>();

		auto fireBoltData = make_shared<AFireBoltData>();
		auto fireBolt = make_shared<AFireBolt>();

		fireBolt->SetAbilityData(fireBoltData);
		abilitySlot->SetAbility(0, fireBolt);

		AddComponent(abilitySlot);
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
