#include "pch.h"
#include "AFireStorm.h"

#include "AIController.h"
#include "PlayableUnit.h"
#include "AbilitySlot.h"
#include "CharacterInfo.h"

#include "FireStorm.h"
#include "MagicCircle.h"

AFireStorm::AFireStorm()
{
	auto tempFireStorm = dynamic_pointer_cast<FireStorm>(MANATER_PARTICLE()->GetParticleFromName(L"FireStorm"));
	if (tempFireStorm != nullptr)
	{
		_fireStormParticle = tempFireStorm;
	}
	else
	{
		_fireStormParticle = make_shared<FireStorm>();
		MANATER_PARTICLE()->AddManagingParticle(L"FireStorm", _fireStormParticle);
	}
	auto tempMagicCircle = dynamic_pointer_cast<MagicCircle>(MANATER_PARTICLE()->GetParticleFromName(L"MagicCircleEffect"));
	if (tempMagicCircle != nullptr)
	{
		_magicCircleParticle = tempMagicCircle;
	}
	else
	{
		_magicCircleParticle = make_shared<MagicCircle>();
		MANATER_PARTICLE()->AddManagingParticle(L"MagicCircleEffect", _magicCircleParticle);
	}

	_fireStormInstance = make_shared<ParticleInstance>();
	_magicCircleInstance = make_shared<ParticleInstance>();
}

AFireStorm::~AFireStorm()
{
}

void AFireStorm::Enter(const shared_ptr<GameObject>& target)
{
	if (_ownerController.lock() != nullptr && _aiController.lock() == nullptr)
	{
		const auto& temp = dynamic_pointer_cast<AIController>(_ownerController.lock());

		if (temp != nullptr)
		{
			_aiController = temp;
		}
	}

	if (_aiController.lock() != nullptr)
	{
		_ownerTransform = _aiController.lock()->GetTransform();
		_ownerTargetTransform = _aiController.lock()->GetTargetTransform();
		_ownerInfo = _aiController.lock()->GetCharacterInfo();
		_ownerAbilitySlot = _aiController.lock()->GetGameObject()->GetComponent<AbilitySlot>();
		_coolTime = _abilityData->GetAbilityData().AbilityCoolTime;
		_ownerAtk = _ownerInfo.lock()->GetCharacterInfo()._atk;
		_abilityRange = _abilityData->GetAbilityData().AbilityRange;
		_abilityDamage = _ownerAtk * _abilityData->GetAbilityData().AbilityPow;
	}

	if (_fireStormInstance != nullptr)
	{
		shared_ptr<Transform> pos = make_shared<Transform>();
		pos->SetLocalPosition(_ownerTransform.lock()->GetGameObject()->GetChildByName(L"Model")->GetTransform()->GetPosition());
		pos->SetLocalScale(Vec3(2.5f));

		_fireStormInstance->SetInstance(100, pos, _ownerTargetTransform.lock(), 100, false);
	}
	if (_magicCircleInstance != nullptr)
	{
		shared_ptr<Transform> pos = make_shared<Transform>();
		pos->SetParent(_ownerTargetTransform.lock());
		pos->SetPosition(Vec3(0, 0, 0));
		pos->SetScale(Vec3(30));
		_magicCircleInstance->SetInstance(3, pos, nullptr, 0, false);
	}
}

void AFireStorm::Execute()
{
	if (_isCoolTime == false)
	{
		if (_magicCircleParticle != nullptr)
		{
			_magicCircleInstance->Reset();
			_magicCircleParticle->AddParticle(_magicCircleInstance);
		}
		if (_fireStormParticle != nullptr)
		{
			_fireStormParticle->SetTargetObject(_ownerTargetTransform.lock()->GetGameObject());
			_fireStormParticle->SetEffectDamage(_abilityDamage);
			_fireStormInstance->Reset();
			_fireStormInstance->particleTransform->SetLocalPosition(_ownerTransform.lock()->GetLocalPosition());
			_fireStormParticle->AddParticle(_fireStormInstance);
			_ownerAbilitySlot.lock()->_selectNumber = -1;
		}		
	}
}

void AFireStorm::UpdateCoolTime()
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
