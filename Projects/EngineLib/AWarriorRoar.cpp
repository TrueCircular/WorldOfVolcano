#include "pch.h"
#include "AWarriorRoar.h"

#include "PlayerController.h"
#include "AIController.h"
#include "EnemyUnit.h"
#include "CharacterInfo.h"
#include "AbilitySlot.h"
#include "WarriorRoar.h"
#include "Clap.h"
#include "ShineHelper.h"

AWarriorRoar::AWarriorRoar()
{
	auto tempRoar = dynamic_pointer_cast<WarriorRoar>(MANATER_PARTICLE()->GetParticleFromName(L"WarriorRoar"));
	if (tempRoar != nullptr)
	{
		_roarParticle = tempRoar;
	}
	else
	{
		_roarParticle = make_shared<WarriorRoar>();
		MANATER_PARTICLE()->AddManagingParticle(L"WarriorRoar", _roarParticle);
	}

	auto tempClap = dynamic_pointer_cast<Clap>(MANATER_PARTICLE()->GetParticleFromName(L"Clap"));
	if (tempClap != nullptr)
	{
		_clapParticle = tempClap;
	}
	else
	{
		_clapParticle = make_shared<Clap>();
		MANATER_PARTICLE()->AddManagingParticle(L"Clap", _clapParticle);
	}

	auto tempSpark = dynamic_pointer_cast<ShineHelper>(MANATER_PARTICLE()->GetParticleFromName(L"SparkHelper"));
	if (tempSpark != nullptr)
	{
		_sparkParticle = tempSpark;
	}
	else
	{
		_sparkParticle = make_shared<ShineHelper>();
		MANATER_PARTICLE()->AddManagingParticle(L"SparkHelper", _sparkParticle);
	}

	_roarInstance = make_shared<ParticleInstance>();
	_clapInstance = make_shared<ParticleInstance>();
	_sparkInstance = make_shared<ParticleInstance>();
}

AWarriorRoar::~AWarriorRoar()
{
}

void AWarriorRoar::AoeDamageToTargets()
{
	auto tInfo = _ownerInfo.lock()->GetCharacterInfo();
	float mpSomo = tInfo._mp - (float)_consumedMp;

	if (mpSomo > 1.f + FLT_EPSILON)
	{
		uint32 lastMp = (uint32)mpSomo;
		auto tempInfo = _ownerInfo.lock()->GetCharacterInfo();
		tempInfo._mp = lastMp;

		_ownerInfo.lock()->SetCharacterInfo(tempInfo);
		MANAGER_IMGUI()->UpdatePicked(true, _playerController.lock()->GetGameObject());

		//적에게 데미지, 이펙트 재생
		{
			auto targets = MANAGER_SCENE()->GetCurrentScene()->GetEnemyUnits();

			vector<shared_ptr<GameObject>> toDamageTargets;

			for (auto& target : targets)
			{
				Vec3 myPos = _ownerTransform.lock()->GetLocalPosition();
				Vec3 targetPos = target->GetTransform()->GetLocalPosition();
				targetPos.y = myPos.y;
				float dist = Vec3::Distance(myPos, targetPos);

				if (dist <= _abilityRange)
				{
					toDamageTargets.push_back(target);
				}
			}

			if (toDamageTargets.size() > 0)
			{
				for (auto& target : toDamageTargets)
				{
					auto targetCon = target->GetComponent<CharacterController>();
					if (targetCon != nullptr)
					{
						if (targetCon->_isAlive == true)
						{
							targetCon->TakeDamage(_playerController.lock()->GetGameObject(), _abilityDamage);
							MANAGER_IMGUI()->UpdatePicked(true, targetCon->GetGameObject());
						}
					}
				}
			}

			_playerController.lock()->SetCurrentState(PlayerUnitState::Ability2);
			_playerController.lock()->SetAnimState(PlayerAnimType::Ability2);

			if (_roarParticle != nullptr)
			{
				_roarParticle->SetAnimator(_ownerAnimator.lock());
				_roarInstance->Reset();
				_roarParticle->AddParticle(_roarInstance, _tweenDesc.lock());
			}
			if (_clapParticle != nullptr) 
			{
				_clapInstance->Reset();
				_clapParticle->AddParticle(_clapInstance);
			}
			if (_sparkParticle != nullptr) 
			{
				_sparkInstance->Reset();
				_sparkParticle->AddParticle(_sparkInstance);
			}
			_ownerAbilitySlot.lock()->_selectNumber = -1;
			_isCoolTime = true;
		}
	}
	else
	{
		_ownerAbilitySlot.lock()->_selectNumber = -1;
	}
}

void AWarriorRoar::Enter(const shared_ptr<GameObject>& target)
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
		_ownerAnimator = _playerController.lock()->GetAnimator();
		_tweenDesc = _ownerAnimator.lock()->GetTweenDesc();
		_ownerTransform = _playerController.lock()->GetTransform();
		_ownerInfo = _playerController.lock()->GetUnitInformation();
		_ownerAbilitySlot = _playerController.lock()->GetGameObject()->GetComponent<AbilitySlot>();
		_ownerAtk = _ownerInfo.lock()->GetCharacterInfo()._atk;
		_coolTime = _abilityData->GetAbilityData().AbilityCoolTime;
		_abilityRange = _abilityData->GetAbilityData().AbilityRange;
		_abilityDamage = _ownerAtk * _abilityData->GetAbilityData().AbilityPow;
		_consumedMp = _abilityData->GetAbilityData().ConsumedMp;
	}

	if (_roarInstance != nullptr)
	{
		shared_ptr<Transform> pos = make_shared<Transform>();
		pos->SetParent(_playerController.lock()->GetGameObject()->GetChildByName(L"Model")->GetTransform());

		_roarInstance->SetInstance(1.2f, pos, nullptr, false);
	}

	if (_clapInstance != nullptr)
	{
		shared_ptr<Transform> pos = make_shared<Transform>();
		pos->SetLocalPosition(_playerController.lock()->GetTransform()->GetLocalPosition());
		pos->SetScale(Vec3(100.f));

		_clapInstance->SetInstance(1.6f, pos, nullptr, false);
	}

	if (_sparkInstance != nullptr)
	{
		shared_ptr<Transform> pos = make_shared<Transform>();
		pos->SetLocalPosition(_playerController.lock()->GetTransform()->GetLocalPosition());
		pos->SetScale(Vec3(100.f));

		_sparkInstance->SetInstance(1.6f, pos, nullptr, false);
	}
}

void AWarriorRoar::Execute()
{
	AoeDamageToTargets();
}

void AWarriorRoar::UpdateCoolTime()
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