#include "pch.h"
#include "AncientRShoulder.h"
#include "ItemData.h"
#include "AncientRShoulderData.h"
#include "CharacterInfo.h"
#include <memory>
AncientRShoulder::AncientRShoulder()
{
}

AncientRShoulder::~AncientRShoulder()
{
}

bool AncientRShoulder::Init(const shared_ptr<GameObject>& owner)
{
	if (owner == nullptr)
		return false;

	const auto& ownerInfo = owner->GetComponent<CharacterInfo>();
	_itemOwnerInfo = ownerInfo;

	const auto& itemData = make_shared<AncientRShoulderData>();
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

bool AncientRShoulder::ApplyItem(const bool& apply)
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

void AncientRShoulder::Awake()
{
    Super::Awake();
}

void AncientRShoulder::Start()
{
    Super::Start();
}

void AncientRShoulder::FixedUpdate()
{
    Super::FixedUpdate();
}

void AncientRShoulder::Update()
{
    Super::Update();
}

void AncientRShoulder::LateUpdate()
{
    Super::LateUpdate();
}