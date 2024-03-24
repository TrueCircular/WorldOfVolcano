#include "pch.h"
#include "AncientSword.h"
#include "ItemData.h"
#include "AncientSwordData.h"
#include "CharacterInfo.h"
#include <memory>

AncientSword::AncientSword()
{
}

AncientSword::~AncientSword()
{
}

bool AncientSword::Init(const shared_ptr<GameObject>& owner)
{
	if (owner == nullptr)
		return false;

	const auto& ownerInfo = owner->GetComponent<CharacterInfo>();
	_itemOwnerInfo = ownerInfo;

	const auto& itemData = make_shared<AncientSwordData>();
	_itemData = itemData;
	_itemData->SetOwnerUnitInfo(_itemOwnerInfo);

	const auto& shader = MANAGER_RESOURCES()->GetResource<Shader>(L"Default");
	_itemModel = make_shared<ModelRenderer>(shader);
	_itemModel->SetModel(_itemData->GetItemMesh());
	_itemModel->SetPass(1);
	AddComponent(_itemModel);

	GetOrAddTransform();
	GetTransform()->_isItemTransform = true;
	owner->GetChildByName(L"Model")->AddChild(shared_from_this());

	MANAGER_SCENE()->GetCurrentScene()->Add(shared_from_this());

	return true;
}

bool AncientSword::ApplyItem(const bool& apply)
{
	if (_itemData == nullptr)
	{
		return false;
	}
	else
	{
		if (apply)
		{
			_itemData->ItemEffectToSelf(apply);
		}
		else
		{
			_itemData->ItemEffectToSelf(apply);
			_itemData = nullptr;
			_itemModel = nullptr;
		}

	}

	return true;
}

void AncientSword::Awake()
{
	Super::Awake();
}

void AncientSword::Start()
{
	Super::Start();
}

void AncientSword::FixedUpdate()
{
	Super::FixedUpdate();
}

void AncientSword::Update()
{
	Super::Update();
}

void AncientSword::LateUpdate()
{
	Super::LateUpdate();
}
