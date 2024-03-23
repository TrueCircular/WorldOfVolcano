#include "pch.h"
#include "MoltenGiant.h"
#include "SphereCollider.h"
#include "HeightGetter.h"
#include "CharacterInfo.h"
#include "AIController.h"

MoltenGiant::MoltenGiant()
{
	_objType = ObjectType::EnemyUnit;
}

MoltenGiant::~MoltenGiant()
{
}

void MoltenGiant::CharacterInit()
{
	//Character Info
	auto chinfo = make_shared<CharacterInfo>();
	{
		{
			//CHARACTER_INFO info;
			//info._name = L"MoltenGiant";
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
			//savePath += L"MoltenGiant/Information.xml";
			//chinfo->SaveCharacterInformationFromFile(savePath);
		}

		wstring LoadPath = DATA_ADDR_UNIT;
		LoadPath += L"MoltenGiant/Information.xml";
		chinfo->LoadCharacterInformationFromFile(LoadPath);
		AddComponent(chinfo);
	}

	//Character Model
	{
		_childModel = make_shared<GameObject>();

		shared_ptr<Model> model = make_shared<Model>();
		{
			AddModelAndMaterial(model, L"MoltenGiant");
			AddAnimation(model, L"MoltenGiant", L"Stand");
			AddAnimation(model, L"MoltenGiant", L"Walk");
			AddAnimation(model, L"MoltenGiant", L"Run");
			AddAnimation(model, L"MoltenGiant", L"Damaged");
			AddAnimation(model, L"MoltenGiant", L"Death");
			AddAnimation(model, L"MoltenGiant", L"Battle");
			AddAnimation(model, L"MoltenGiant", L"Attack1");
			AddAnimation(model, L"MoltenGiant", L"Attack2");
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
		AddChild(_childModel);
	}

	{
		auto height = make_shared<HeightGetter>();
		height->Set(MANAGER_SCENE()->GetCurrentScene()->GetCurrentTerrain().get());
		AddComponent(height);
	}

	{
		auto collider = make_shared<SphereCollider>();
		collider->SetRadius(30.f);
		Vec3 offset = Vec3(0, 25.f, 0);
		collider->SetOffset(offset);
		AddComponent(collider);
	}

	SetName(L"MoltenGiant");
}

void MoltenGiant::Awake()
{
	Super::Awake();
	CharacterInit();
}

void MoltenGiant::Start()
{
	Super::Start();
}

void MoltenGiant::FixedUpdate()
{
	Super::FixedUpdate();
}

void MoltenGiant::Update()
{
	Super::Update();
}

void MoltenGiant::LateUpdate()
{
	Super::LateUpdate();
}
