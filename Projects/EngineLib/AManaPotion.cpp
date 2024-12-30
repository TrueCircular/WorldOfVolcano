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
	auto tInfo = _ownerInfo->GetCharacterInfo();


	auto tempInfo = _ownerInfo->GetCharacterInfo();
	tempInfo._mp = tInfo._mp + (tInfo._maxMp * _abilityData->GetAbilityData().AbilityPow);
	if (tempInfo._mp >= tInfo._maxMp) {
		tempInfo._mp = tInfo._maxMp;
	}
	_ownerInfo->SetCharacterInfo(tempInfo);
	MANAGER_IMGUI()->UpdatePicked(true, _playerController->GetGameObject());

	{
		auto tempSound = MANAGER_RESOURCES()->GetResource<Sounds>(L"PotionSound");
		shared_ptr<Sounds> sound = nullptr;
		if (tempSound == nullptr)
		{
			shared_ptr<Sounds> sound = make_shared<Sounds>();
			wstring soundPath = RESOURCES_ADDR_SOUND;
			soundPath += L"Item/Potion.mp3";
			sound->Load(soundPath);
			sound->SetVolume(100);
			MANAGER_RESOURCES()->AddResource<Sounds>(L"PotionSound", sound);

			sound = sound->Clone();
		}
		else
		{
			sound = tempSound->Clone();
		}

		if (sound != nullptr)
		{
			sound->Play(false);
		}
	}
	_ownerAbilitySlot->_selectNumber = -1;
	_isCoolTime = true;


}

void AManaPotion::Enter(const shared_ptr<GameObject>& target)
{
	if (_ownerController != nullptr && _playerController == nullptr)
	{
		const auto& temp = dynamic_pointer_cast<PlayerController>(_ownerController);

		if (temp != nullptr)
		{
			_playerController = temp;
		}
	}
	if (_playerController != nullptr)
	{
		_ownerTransform = _playerController->GetTransform();
		_ownerInfo = _playerController->GetUnitInformation();
		_ownerAbilitySlot = _playerController->GetGameObject()->GetComponent<AbilitySlot>();
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