#include "pch.h"
#include "AWarriorCharge.h"

#include "PlayerController.h"
#include "AIController.h"
#include "EnemyUnit.h"
#include "CharacterInfo.h"
#include "AbilitySlot.h"
#include "Polar.h"
#include "Explode.h"

AWarriorCharge::AWarriorCharge()
{
	auto tempPolar = dynamic_pointer_cast<Polar>(MANATER_PARTICLE()->GetParticleFromName(L"Polar"));
	if (tempPolar != nullptr)
	{
		_polarParticle = tempPolar;
	}
	else
	{
		_polarParticle = make_shared<Polar>();
		MANATER_PARTICLE()->AddManagingParticle(L"Polar", _polarParticle);
	}
	auto tempExplode = dynamic_pointer_cast<Explode>(MANATER_PARTICLE()->GetParticleFromName(L"Explode"));
	if (tempExplode != nullptr)
	{
		_explodeParticle = tempExplode;
	}
	else
	{
		_explodeParticle = make_shared<Explode>();
		MANATER_PARTICLE()->AddManagingParticle(L"Explode", _explodeParticle);
	}

	_polarInstance = make_shared<ParticleInstance>();
	_explodeInstance = make_shared<ParticleInstance>();

}

AWarriorCharge::~AWarriorCharge()
{
}

void AWarriorCharge::Charging()
{

	if (_ownerPicked->GetComponent<AIController>()->_isAlive) {
		Vec3 ownerPos = _ownerTransform->GetLocalPosition();
		Vec3 targetPos = _ownerPicked->GetTransform()->GetLocalPosition();
		Vec3 velocity = targetPos - ownerPos;
		velocity.y = 0;
		velocity.Normalize();
		float height=MANAGER_SCENE()->GetCurrentScene()->GetCurrentTerrain()->GetHeight(ownerPos.x,ownerPos.z);
		ownerPos += velocity * (_chargeSpeed * MANAGER_TIME()->GetDeltaTime());
		ownerPos.y = height;
		_ownerTransform->SetLocalPosition(ownerPos);
		targetPos.y = 0;
		ownerPos.y = 0;
		float dist = Vec3::Distance(targetPos, ownerPos);
		dist = abs(dist);
		if (dist < (_chargeSpeed * MANAGER_TIME()->GetDeltaTime()+30)) {
			_isCharging = false;
			DamageToTargets();
		}
		else {
			auto trailParticle = MANATER_PARTICLE()->GetParticleFromName(L"Smoke4");
			shared_ptr<Transform> form = make_shared<Transform>();
			ownerPos.y = height;
			form->SetLocalPosition(ownerPos);
			form->SetScale(Vec3(8,8,8));
			shared_ptr<ParticleInstance> ins = make_shared<ParticleInstance>(0.3f,form,nullptr,0);
			trailParticle->AddParticle(ins);
		}
	}
	else {
		_isCharging = false;
	}
}

void AWarriorCharge::DamageToTargets()
{

	//적에게 데미지, 이펙트 재생

	auto targets = _ownerPicked;
	if(targets!=nullptr){
					
		 auto targetCon = targets->GetComponent<CharacterController>();
		if (targetCon != nullptr)
		{
			if (targetCon->_isAlive == true)
			{
				targetCon->TakeDamage(_playerController->GetGameObject(), _abilityDamage);
				MANAGER_IMGUI()->UpdatePicked(true, targetCon->GetGameObject());
			}
		}
	
	}

	if (_polarInstance != nullptr) {
		_polarInstance->isDestroy = true;
		_polarInstance->isLoop = false;
	}
	if (_explodeParticle != nullptr)
	{
		_explodeInstance->Reset();
		Vec3 _explodePos = targets->GetTransform()->GetLocalPosition();
		_explodePos.y += 8;
		_explodeInstance->particleTransform->SetLocalPosition(_explodePos);
		_explodeParticle->AddParticle(_explodeInstance);
	}

	_playerController->SetCurrentState(PlayerUnitState::Stand);
	_playerController->SetAnimState(PlayerAnimType::Stand);
	_ownerAbilitySlot->_selectNumber = -1;
	_isCoolTime = true;

}

void AWarriorCharge::Enter(const shared_ptr<GameObject>& target)
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
		_ownerAnimator = _playerController->GetAnimator();
		_ownerTransform = _playerController->GetTransform();
		_ownerInfo = _playerController->GetUnitInformation();
		_ownerAbilitySlot = _playerController->GetGameObject()->GetComponent<AbilitySlot>();
		_ownerAtk = _ownerInfo->GetCharacterInfo()._atk;
		_coolTime = _abilityData->GetAbilityData().AbilityCoolTime;
		_abilityRange = _abilityData->GetAbilityData().AbilityRange;
		_abilityDamage = _ownerAtk * _abilityData->GetAbilityData().AbilityPow;
		_consumedMp = _abilityData->GetAbilityData().ConsumedMp;
		_ownerPicked = target;

	}
	if(target&& !_isCharging){
		auto tInfo = _ownerInfo->GetCharacterInfo();
		float mpSomo = tInfo._mp - (float)_consumedMp;

		if (mpSomo > 1.f + FLT_EPSILON)
		{
			uint32 lastMp = (uint32)mpSomo;
			auto tempInfo = _ownerInfo->GetCharacterInfo();
			tempInfo._mp = lastMp;

			_ownerInfo->SetCharacterInfo(tempInfo);
			MANAGER_IMGUI()->UpdatePicked(true, _playerController->GetGameObject());

			if (_ownerController != nullptr && _playerController == nullptr)
			{
				const auto& temp = dynamic_pointer_cast<PlayerController>(_ownerController);

				if (temp != nullptr)
				{
					_playerController = temp;
				}
			}
			
			if (_polarInstance != nullptr)
			{
				shared_ptr<Transform> pos = make_shared<Transform>();	
				pos->SetParent(_ownerTransform->GetTransform());
				pos->SetLocalPosition(Vec3(-1, 8,3));
				pos->SetLocalRotation(Vec3(::XMConvertToRadians(90), ::XMConvertToRadians(90),::XMConvertToRadians(-90)));
				pos->SetLocalScale(Vec3(1.5, 1.5, 1.5));

				_polarInstance->SetInstance(1.2f, pos, nullptr,0,true);
			}
			if (_explodeInstance != nullptr)
			{
				shared_ptr<Transform> pos = make_shared<Transform>();
				pos->SetLocalPosition(_playerController->GetTransform()->GetLocalPosition());
				pos->SetScale(Vec3(100.f));
				_explodeInstance->SetInstance(1.6f, pos, nullptr, false);
			}
			if (_polarParticle != nullptr)
			{
				_polarInstance->Reset();
				_polarInstance->isLoop = true;
				_polarParticle->AddParticle(_polarInstance);
			}

	//TODO
	///
	///	MAKE Rotate To Target;
	/// Disable Player Controll When Charging
	/// 
			_isCharging = true;
			_playerController->SetCurrentState(PlayerUnitState::Ability1);
			_playerController->SetAnimState(PlayerAnimType::Ability1);
		}

	}
	//else
	//{
	//	_ownerAbilitySlot->_selectNumber = -1;
	//}
}

void AWarriorCharge::Execute()
{
	if (_isCharging) {
		Charging();
	}
}

void AWarriorCharge::UpdateCoolTime()
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