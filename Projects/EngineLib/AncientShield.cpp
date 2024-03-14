#include "pch.h"
#include "AncientShield.h"
#include "ItemData.h"
#include "AncientShieldData.h"
#include "CharacterInfo.h"
#include <memory>
AncientShield::AncientShield()
{
}

AncientShield::~AncientShield()
{
}

bool AncientShield::Init(const shared_ptr<GameObject>& owner)
{
	if (owner == nullptr)
		return false;

	const auto& ownerInfo = owner->GetComponent<CharacterInfo>();
	_itemOwnerInfo = ownerInfo;

	const auto& itemData = make_shared<AncientShieldData>();
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

bool AncientShield::ApplyItem(const bool& apply)
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

void AncientShield::Awake()
{
	Super::Awake();
}

void AncientShield::Start()
{
	Super::Start();
}

void AncientShield::FixedUpdate()
{
	Super::FixedUpdate();
}

void AncientShield::Update()
{
	Super::Update();
}

void AncientShield::LateUpdate()
{
	Super::LateUpdate();
}
