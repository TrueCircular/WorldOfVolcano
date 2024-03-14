#include "pch.h"
#include "AncientBelt.h"
#include "ItemData.h"
#include "AncientBeltData.h"
#include "CharacterInfo.h"
#include <memory>

AncientBelt::AncientBelt()
{
}

AncientBelt::~AncientBelt()
{
}

bool AncientBelt::Init(const shared_ptr<GameObject>& owner)
{
	if (owner == nullptr)
		return false;

	const auto& ownerInfo = owner->GetComponent<CharacterInfo>();
	_itemOwnerInfo = ownerInfo;

	const auto& itemData = make_shared<AncientBeltData>();
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

bool AncientBelt::ApplyItem(const bool& apply)
{
	if (_itemData == nullptr)
	{
		return false;
	}
	else
	{
		_itemData->ItemEffectToSelf(apply);
	}

	return true;
}

void AncientBelt::Awake()
{
	Super::Awake();
}

void AncientBelt::Start()
{
	Super::Start();
}

void AncientBelt::FixedUpdate()
{
	Super::FixedUpdate();
}

void AncientBelt::Update()
{
	Super::Update();
}

void AncientBelt::LateUpdate()
{
	Super::LateUpdate();
}

