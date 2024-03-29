#include "pch.h"
#include "AHealPotion.h"

#include "PlayerController.h"
#include "AIController.h"
#include "CharacterInfo.h"
#include "AbilitySlot.h"

AHealPotion::AHealPotion()
{

}

AHealPotion::~AHealPotion()
{
}

void AHealPotion::HealPotion()
{
	auto tInfo = _ownerInfo.lock()->GetCharacterInfo();


	auto tempInfo = _ownerInfo.lock()->GetCharacterInfo();
	tempInfo._hp = tInfo._hp+(tInfo._maxHp * _abilityData->GetAbilityData().AbilityPow);
	if (tempInfo._hp >= tInfo._maxHp) {
		tempInfo._hp = tInfo._maxHp;
	}
	_ownerInfo.lock()->SetCharacterInfo(tempInfo);
	MANAGER_IMGUI()->UpdatePicked(true, _playerController.lock()->GetGameObject());
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
	_ownerAbilitySlot.lock()->_selectNumber = -1;
	_isCoolTime = true;
	_ownerAbilitySlot.lock()->_selectNumber = -1;
	_isCoolTime = true;
	

}

void AHealPotion::Enter(const shared_ptr<GameObject>& target)
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

void AHealPotion::Execute()
{
	HealPotion();
}

void AHealPotion::UpdateCoolTime()
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