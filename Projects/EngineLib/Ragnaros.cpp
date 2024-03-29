#include "pch.h"
#include "Ragnaros.h"

#include "AIController.h"
#include "CharacterInfo.h"
#include "SphereCollider.h"
#include "HeightGetter.h"

#include "AbilitySlot.h"

Ragnaros::Ragnaros()
{
	_objType = ObjectType::EnemyUnit;
}

Ragnaros::~Ragnaros()
{
}

void Ragnaros::CharacterInit()
{
	//Character Info
	auto chinfo = make_shared<CharacterInfo>();
	{
		wstring LoadPath = DATA_ADDR_UNIT;
		LoadPath += L"Ragnaros/Information.xml";
		chinfo->LoadCharacterInformationFromFile(LoadPath);
		AddComponent(chinfo);
	}

	//Character Model
	{
		_childModel = make_shared<GameObject>();

		shared_ptr<Model> model = make_shared<Model>();
		{
			AddModelAndMaterial(model, L"Ragnaros");
			AddAnimation(model, L"Ragnaros", L"Stand");
			AddAnimation(model, L"Ragnaros", L"Stun");
			AddAnimation(model, L"Ragnaros", L"Birth");
			AddAnimation(model, L"Ragnaros", L"Submerge");
			AddAnimation(model, L"Ragnaros", L"Submerged");
			AddAnimation(model, L"Ragnaros", L"Roar");
			AddAnimation(model, L"Ragnaros", L"Death");
			AddAnimation(model, L"Ragnaros", L"Battle");
			AddAnimation(model, L"Ragnaros", L"Attack1");
			AddAnimation(model, L"Ragnaros", L"Attack2");
			AddAnimation(model, L"Ragnaros", L"Ability");
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
		_childModel->GetTransform()->SetLocalScale(Vec3(0.1f));
		_childModel->GetTransform()->SetLocalRotation(rot);
		_childModel->GetTransform()->SetLocalPosition(Vec3(0, 10, 0));
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

		AddComponent(abilitySlot);
	}

	SetName(L"Ragnaros");
}

void Ragnaros::Awake()
{
	Super::Awake();
	CharacterInit();
}

void Ragnaros::Start()
{
	Super::Start();
}

void Ragnaros::FixedUpdate()
{
	Super::FixedUpdate();
}

void Ragnaros::Update()
{
	Super::Update();
}

void Ragnaros::LateUpdate()
{
	Super::LateUpdate();
}
