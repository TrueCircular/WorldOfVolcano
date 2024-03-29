#include "pch.h"
#include "AManaPotion.h"
#include "PlayerController.h"
#include "AIController.h"
#include "CharacterInfo.h"
#include "AbilitySlot.h"

AManaPotion::AManaPotion()
{

}

AManaPotion::~AManaPotion()
{
}

void AManaPotion::ManaPotion()
{
	auto tInfo = _ownerInfo.lock()->GetCharacterInfo();


	auto tempInfo = _ownerInfo.lock()->GetCharacterInfo();
	tempInfo._mp = tInfo._mp + (tInfo._maxMp * _abilityData->GetAbilityData().AbilityPow);
	if (tempInfo._mp >= tInfo._maxMp) {
		tempInfo._mp = tInfo._maxMp;
	}
	_ownerInfo.lock()->SetCharacterInfo(tempInfo);
	MANAGER_IMGUI()->UpdatePicked(true, _playerController.lock()->GetGameObject());

	_ownerAbilitySlot.lock()->_selectNumber = -1;
	_isCoolTime = true;


}

void AManaPotion::Enter(const shared_ptr<GameObject>& target)
{
	if (_ownerController.lock() != nullptr && _playerController.lock() == nullptr)
	{
		const auto& temp = dynamic_pointer_cast<PlayerController>(_ownerController.lock());

		if (temp != nullptr)
		{
			_playerController = temp;
		}
	}
	if (_playerController.lock() != nullptr)
	{
		_ownerTransform = _playerController.lock()->GetTransform();
		_ownerInfo = _playerController.lock()->GetUnitInformation();
		_ownerAbilitySlot = _playerController.lock()->GetGameObject()->GetComponent<AbilitySlot>();
		_coolTime = _abilityData->GetAbilityData().AbilityCoolTime;


	}


}

void AManaPotion::Execute()
{
	ManaPotion();
}

void AManaPotion::UpdateCoolTime()
{
	if (_isCoolTime)
	{
		_coolTimer += MANAGER_TIME()->GetDeltaTime();

		if (_coolTimer > _coolTime)
		{
			_isCoolTime = false;
			_coolTimer = 0.f;
		}
	}
}