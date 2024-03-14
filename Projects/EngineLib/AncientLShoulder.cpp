#include "pch.h"
#include "AncientLShoulder.h"
#include "ItemData.h"
#include "AncientLShoulderData.h"
#include "CharacterInfo.h"
#include <memory>
AncientLShoulder::AncientLShoulder()
{
}

AncientLShoulder::~AncientLShoulder()
{
}

bool AncientLShoulder::Init(const shared_ptr<GameObject>& owner)
{
	if (owner == nullptr)
		return false;

	const auto& ownerInfo = owner->GetComponent<CharacterInfo>();
	_itemOwnerInfo = ownerInfo;

	const auto& itemData = make_shared<AncientLShoulderData>();
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

bool AncientLShoulder::ApplyItem(const bool& apply)
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

void AncientLShoulder::Awake()
{
    Super::Awake();
}

void AncientLShoulder::Start()
{
    Super::Start();
}

void AncientLShoulder::FixedUpdate()
{
    Super::FixedUpdate();
}

void AncientLShoulder::Update()
{
    Super::Update();
}

void AncientLShoulder::LateUpdate()
{
    Super::LateUpdate();
}
