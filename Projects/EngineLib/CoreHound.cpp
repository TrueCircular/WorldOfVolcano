#include "pch.h"
#include "CoreHound.h"
#include "SphereCollider.h"
#include "HeightGetter.h"
#include "CharacterInfo.h"
#include "AIController.h"

CoreHound::CoreHound()
{
	_objType = ObjectType::EnemyUnit;
}

CoreHound::~CoreHound()
{
}

void CoreHound::CharacterInit()
{
	//Character Info
	auto chinfo = make_shared<CharacterInfo>();
	{

		{
			//CHARACTER_INFO info;
			//info._name = L"CoreHound";
			//info._hp = 12000;
			//info._maxHp = info._hp;
			//info._mp = 500;
			//info._maxMp = info._mp;
			//info._atk = 1500;
			//info._def = 250;
			//info._moveSpeed = 30;
			//info._aggroLevel = 0;
			//info._attackRange = 15;
			//info._attackTime = 1.5f;
			//info._traceRadius = 60.f;

			//chinfo->SetDefaultCharacterInfo(info);
			//wstring savePath = DATA_ADDR_UNIT;
			//savePath += L"CoreHound/Information.xml";
			//chinfo->SaveCharacterInformationFromFile(savePath);
		}

		wstring LoadPath = DATA_ADDR_UNIT;
		LoadPath += L"CoreHound/Information.xml";
		chinfo->LoadCharacterInformationFromFile(LoadPath);
		AddComponent(chinfo);
	}

	//Character Model
	{
		_childModel = make_shared<GameObject>();

		shared_ptr<Model> model = make_shared<Model>();
		{
			AddModelAndMaterial(model, L"CoreHound");
			AddAnimation(model, L"CoreHound", L"Stand");
			AddAnimation(model, L"CoreHound", L"Walk");
			AddAnimation(model, L"CoreHound", L"Run");
			AddAnimation(model, L"CoreHound", L"Damaged");
			AddAnimation(model, L"CoreHound", L"Death");
			AddAnimation(model, L"CoreHound", L"Battle");
			AddAnimation(model, L"CoreHound", L"Attack1");
			AddAnimation(model, L"CoreHound", L"Attack2");
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
		_childModel->GetTransform()->SetLocalScale(Vec3(0.15f));
		_childModel->GetTransform()->SetLocalRotation(rot);
		_childModel->GetTransform()->SetLocalPosition(Vec3(0, 25, 0));
		AddChild(_childModel);
	}

	{
		auto height = make_shared<HeightGetter>();
		height->Set(MANAGER_SCENE()->GetCurrentScene()->GetCurrentTerrain().get());
		AddComponent(height);
	}

	{
		auto collider = make_shared<SphereCollider>();
		collider->SetRadius(20.f);
		AddComponent(collider);
	}

	SetName(L"CoreHound");
}

void CoreHound::Awake()
{
	Super::Awake();
	CharacterInit();
}

void CoreHound::Start()
{
	Super::Start();
}

void CoreHound::FixedUpdate()
{
	Super::FixedUpdate();
}

void CoreHound::Update()
{
	Super::Update();
}

void CoreHound::LateUpdate()
{
	Super::LateUpdate();
}
