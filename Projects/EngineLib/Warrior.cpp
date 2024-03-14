#include "pch.h"
#include "Warrior.h"
#include "PlayerController.h"
#include "CharacterInfo.h"
#include "HeightGetter.h"
#include "ItemData.h"
#include "AncientHelmData.h"
#include "AncientSword.h"
#include "EquipmentSlot.h"

Warrior::Warrior()
{
	_device = GRAPHICS()->GetDevice();
	_deviceContext = GRAPHICS()->GetDeviceContext();
	_objType = ObjectType::PlayableUnit;
}

Warrior::~Warrior()
{
}

void Warrior::CharacterInit()
{
	//Set Information 
	{
		auto UnitInfo = make_shared<CharacterInfo>();
		wstring LoadPath = DATA_ADDR_UNIT;
		LoadPath += L"Warrior/Information.xml";
		UnitInfo->LoadCharacterInformationFromFile(LoadPath);
		AddComponent(UnitInfo);
	}

	//Set Model 
	{
		_childModel = make_shared<GameObject>();
		shared_ptr<Model> model = make_shared<Model>();
		{
			AddModelAndMaterial(model, L"Warrior");
			AddAnimation(model, L"Warrior", L"Stand");
			AddAnimation(model, L"Warrior", L"FrontWalk");
			AddAnimation(model, L"Warrior", L"BackWalk");
			AddAnimation(model, L"Warrior", L"FrontRun");
			AddAnimation(model, L"Warrior", L"BackRun");
			AddAnimation(model, L"Warrior", L"JumpStart");
			AddAnimation(model, L"Warrior", L"JumpFall");
			AddAnimation(model, L"Warrior", L"JumpEnd");
			AddAnimation(model, L"Warrior", L"Battle");
			AddAnimation(model, L"Warrior", L"Attack1");
			AddAnimation(model, L"Warrior", L"Attack2");
			AddAnimation(model, L"Warrior", L"Ability1");
			AddAnimation(model, L"Warrior", L"Ability2");
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
		_childModel->GetTransform()->SetLocalRotation(rot);
		_childModel->GetTransform()->SetLocalScale(Vec3(0.1f));
		AddChild(_childModel);
	}

	//Set HeightGetter
	{
		auto height = make_shared<HeightGetter>();
		height->Set(MANAGER_SCENE()->GetCurrentScene()->GetCurrentTerrain().get());
		AddComponent(height);
	}

	//Set Equipment
	{
		auto equipmentSlot = make_shared<EquipmentSlot>();

		shared_ptr<AncientSword> sword = make_shared<AncientSword>();
		sword->Init(shared_from_this());

		equipmentSlot->EquipmentItem(4, sword);
		AddComponent(equipmentSlot);
	}

	SetName(L"Warrior");
}

void Warrior::Awake()
{
	Super::Awake();

	CharacterInit();
}

void Warrior::Start()
{
	Super::Start();
}

void Warrior::FixedUpdate()
{
	Super::FixedUpdate();
}

void Warrior::Update()
{
	Super::Update();
}

void Warrior::LateUpdate()
{
	Super::LateUpdate();
}
