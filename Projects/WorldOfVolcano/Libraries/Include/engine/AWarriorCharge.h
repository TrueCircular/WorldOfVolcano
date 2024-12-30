#pragma once
#include "Ability.h"
#include "ParticleObj.h"
#pragma region Forward Declaration
class CharacterInfo;
class PlayerController;
class AbilitySlot;
class Explode;
class Polar;
#pragma endregion

class AWarriorCharge : public Ability
{
public:
	AWarriorCharge();
	virtual ~AWarriorCharge();

private:
	bool _isCharging = false;
	shared_ptr<PlayerController>   _playerController;
	shared_ptr<ModelAnimator>		 _ownerAnimator;
	
	shared_ptr<GameObject>	 _ownerPicked;
	shared_ptr<Polar>		 _polarParticle;
	shared_ptr<Explode>		 _explodeParticle;

	shared_ptr<ParticleInstance> _explodeInstance;
	shared_ptr<ParticleInstance> _polarInstance;
	uint16						 _ownerAtk = 0;
	uint32						 _consumedMp = 0;
	float						 _abilityRange = 0.f;
	float						 _abilityDamage = 0.f;

	//Need To moreConfigure
	float						 _chargeSpeed = 100.0f;
private:
	void Charging();
	void DamageToTargets();
public:
	virtual void Enter(const shared_ptr<GameObject>& target) override;
	virtual void Execute() override;
	virtual void UpdateCoolTime() override;
};

