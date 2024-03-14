#include "pch.h"
#include "AncientHelm.h"
#include "ItemData.h"
#include "AncientHelmData.h"
#include "CharacterInfo.h"
#include <memory>

AncientHelm::AncientHelm()
{
}

AncientHelm::~AncientHelm()
{
}

bool AncientHelm::Init(const shared_ptr<GameObject>& owner)
{
	if (owner == nullptr)
		return false;

	const auto& ownerInfo = owner->GetComponent<CharacterInfo>();
	_itemOwnerInfo = ownerInfo;

	const auto& itemData = make_shared<AncientHelmData>();
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

bool AncientHelm::ApplyItem(const bool& apply)
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

void AncientHelm::Awake()
{
	Super::Awake();
}

void AncientHelm::Start()
{
	Super::Start();
}

void AncientHelm::FixedUpdate()
{
	Super::FixedUpdate();
}

void AncientHelm::Update()
{
	Super::Update();
}

void AncientHelm::LateUpdate()
{
	Super::LateUpdate();
}
