#include "pch.h"
#include "Warrior.h"

//Controller
#include "PlayerController.h"
#include "CharacterInfo.h"
#include "HeightGetter.h"
//Item
#include "EquipmentSlot.h"
#include "ItemData.h"
#include "AncientHelm.h"
#include "AncientLShoulder.h"
#include "AncientRShoulder.h"
#include "AncientBelt.h"
#include "AncientSword.h"
#include "AncientShield.h"
//Ability
#include "AbilitySlot.h"
#include "AbilityData.h"
#include "AWarriorRoarData.h"
#include "AWarriorRoar.h"
#include "AWarriorChargeData.h"
#include "AWarriorCharge.h"
//CommonAbility
#include "AManaPotion.h"
#include "AManaPotionData.h"
#include "AHealPotion.h"
#include "AHealPotionData.h"

Warrior::Warrior()
{
	_objType = ObjectType::PlayableUnit;
}

Warrior::~Warrior()
{
}

void Warrior::CharacterInit()
{
	//Set Information
	auto UnitInfo = make_shared<CharacterInfo>();
	{

		{
			//CHARACTER_INFO saveInfo;
			//saveInfo._name = L"Warrior";
			//saveInfo._hp = 2000;
			//saveInfo._mp = 350;
			//saveInfo._atk = 250;
			//saveInfo._def = 600;
			//saveInfo._moveSpeed = 40;
			//saveInfo._aggroLevel = 100;
			//saveInfo._attackRange = 15;
			//saveInfo._attackTime = 0.8f;
			//saveInfo._traceRadius = 0.f;

			//UnitInfo->SetCharacterInfo(saveInfo);
			//wstring savePath = DATA_ADDR_UNIT;
			//savePath += L"Warrior/Information.xml";
			//UnitInfo->SaveCharacterInformationFromFile(savePath);
		}

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
			AddAnimation(model, L"Warrior", L"Damaged");
			AddAnimation(model, L"Warrior", L"Stun");
			AddAnimation(model, L"Warrior", L"Loot");
			AddAnimation(model, L"Warrior", L"Damaged");
			AddAnimation(model, L"Warrior", L"Death");
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

		//Helm
		shared_ptr<AncientHelm> helm = make_shared<AncientHelm>();
		helm->Init(shared_from_this());
		equipmentSlot->EquipmentItem(0, helm);

		//LShoulder
		shared_ptr<AncientLShoulder> LShoulder = make_shared<AncientLShoulder>();
		LShoulder->Init(shared_from_this());
		equipmentSlot->EquipmentItem(1, LShoulder);

		//RShoulder
		shared_ptr<AncientRShoulder> RShoulder = make_shared<AncientRShoulder>();
		RShoulder->Init(shared_from_this());
		equipmentSlot->EquipmentItem(2, RShoulder);

		//Belt
		shared_ptr<AncientBelt> Belt = make_shared<AncientBelt>();
		Belt->Init(shared_from_this());
		equipmentSlot->EquipmentItem(3, Belt);

		//Weapon
		shared_ptr<AncientSword> sword = make_shared<AncientSword>();
		sword->Init(shared_from_this());
		equipmentSlot->EquipmentItem(4, sword);

		//Shield
		shared_ptr<AncientShield> shield = make_shared<AncientShield>();
		shield->Init(shared_from_this());
		equipmentSlot->EquipmentItem(5, shield);

		AddComponent(equipmentSlot);
	}

	//Set Ability
	{
		auto abSlot = make_shared<AbilitySlot>();

		auto chargeData = make_shared<AWarriorChargeData>();
		auto _charge = make_shared<AWarriorCharge>();
		_charge->SetAbilityData(chargeData);
		abSlot->SetAbility(0, _charge);

		auto roarData = make_shared<AWarriorRoarData>();
		auto roar = make_shared<AWarriorRoar>();
		roar->SetAbilityData(roarData);
		abSlot->SetAbility(1, roar);

		auto healPotionData = make_shared<AHealPotionData>();
		auto healPotion = make_shared<AHealPotion>();
		healPotion->SetAbilityData(healPotionData);
		abSlot->SetAbility(2, healPotion);

		auto manaPotionData = make_shared<AManaPotionData>();
		auto manaPotion = make_shared<AManaPotion>();
		manaPotion->SetAbilityData(manaPotionData);
		abSlot->SetAbility(3, manaPotion);

		AddComponent(abSlot);
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
