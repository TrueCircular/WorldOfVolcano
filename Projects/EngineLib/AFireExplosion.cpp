#include "pch.h"
#include "AFireExplosion.h"

#include "AIController.h"
#include "PlayableUnit.h"
#include "AbilitySlot.h"
#include "CharacterInfo.h"

#include "RagExplode.h"
AFireExplosion::AFireExplosion()
{
	auto FireExplosion = dynamic_pointer_cast<RagExplode>(MANATER_PARTICLE()->GetParticleFromName(L"RagExplode"));
	if (FireExplosion != nullptr)
	{
		_fireExplosionParticle = FireExplosion;
	}
	else
	{
		_fireExplosionParticle = make_shared<RagExplode>();
		MANATER_PARTICLE()->AddManagingParticle(L"RagExplode", _fireExplosionParticle);
	}

	_fireExplosionInstance = make_shared<ParticleInstance>();
}

AFireExplosion::~AFireExplosion()
{
}

void AFireExplosion::Enter(const shared_ptr<GameObject>& target)
{
	if (_ownerController != nullptr && _aiController == nullptr)
	{
		const auto& temp = dynamic_pointer_cast<AIController>(_ownerController);

		if (temp != nullptr)
		{
			_aiController = temp;
		}
	}

	if (_aiController != nullptr)
	{
		_ownerTransform = _aiController->GetTransform();
		_ownerTargetTransform = _aiController->GetTargetTransform();
		_ownerInfo = _aiController->GetCharacterInfo();
		_ownerAbilitySlot = _aiController->GetGameObject()->GetComponent<AbilitySlot>();
		_coolTime = _abilityData->GetAbilityData().AbilityCoolTime;
		_ownerAtk = _ownerInfo->GetCharacterInfo()._atk;
		_abilityRange = _abilityData->GetAbilityData().AbilityRange;
		_abilityDamage = _ownerAtk * _abilityData->GetAbilityData().AbilityPow;
	}

	if (_fireExplosionInstance != nullptr)
	{
		shared_ptr<Transform> pos = make_shared<Transform>();
		//임시수정
		if (_ownerTargetTransform)
		{
			Vec3 offset = _ownerTargetTransform->GetGameObject()->GetChildByName(L"Model")->GetTransform()->GetPosition();
			offset.y += 10.f;
			pos->SetLocalPosition(offset);
			pos->SetLocalScale(Vec3(50.f));
		}

		_fireExplosionInstance->SetInstance(1, pos, nullptr, 100, false);
	}
}

void AFireExplosion::Execute()
{
	if (_isCoolTime == false)
	{
		if (_fireExplosionParticle != nullptr)
		{
			_fireExplosionParticle->SetTargetObject(_ownerTargetTransform->GetGameObject());
			_fireExplosionParticle->SetEffectDamage(_abilityDamage);
			_fireExplosionInstance->Reset();
			_fireExplosionParticle->AddParticle(_fireExplosionInstance);
			_ownerAbilitySlot->_selectNumber = -1;
		}
	}
}

void AFireExplosion::UpdateCoolTime()
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
