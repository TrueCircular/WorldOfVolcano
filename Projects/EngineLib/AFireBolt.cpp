#include "pch.h"
#include "AFireBolt.h"

#include "AIController.h"
#include "PlayableUnit.h"
#include "AbilitySlot.h"
#include "CharacterInfo.h"

#include "FireBall.h"

AFireBolt::AFireBolt()
{
	auto tempFireBolt = dynamic_pointer_cast<FireBall>(MANATER_PARTICLE()->GetParticleFromName(L"FireBall"));
	if(tempFireBolt != nullptr)
	{
		_fireBoltParticle = tempFireBolt;
	}
	else
	{
		_fireBoltParticle = make_shared<FireBall>();
		MANATER_PARTICLE()->AddManagingParticle(L"FireBall", _fireBoltParticle);
	}

	_fireBoltInstance = make_shared<ParticleInstance>();
}

AFireBolt::~AFireBolt()
{
}

void AFireBolt::Enter(const shared_ptr<GameObject>& target)
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

	if (_fireBoltInstance != nullptr)
	{
		shared_ptr<Transform> pos = make_shared<Transform>();
		pos->SetLocalPosition(_ownerTransform.lock()->GetGameObject()->GetChildByName(L"Model")->GetTransform()->GetPosition());
		pos->SetLocalRotation(_ownerTransform.lock()->GetLocalRotation());
		pos->SetLocalScale(Vec3(5.f));

		_fireBoltInstance->SetInstance(100, pos, _ownerTargetTransform.lock(), 100, true);
		_fireBoltInstance->parentTransform = pos;
	}
}

void AFireBolt::Execute()
{
	if (_isCoolTime == false)
	{
		if (_fireBoltParticle != nullptr)
		{
			_fireBoltParticle->SetTargetObject(_ownerTargetTransform.lock()->GetGameObject());
			_fireBoltParticle->SetEffectDamage(_abilityDamage);
			_fireBoltParticle->AddParticle(_fireBoltInstance);
			_ownerAbilitySlot.lock()->_selectNumber = -1;
		}
	}
}

void AFireBolt::UpdateCoolTime()
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
