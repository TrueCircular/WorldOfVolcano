#pragma once
#include "Ability.h"
#include "ParticleObj.h"

#pragma region Forward Declaration
class PlayerController;
class CharacterInfo;
class AbilitySlot;
class WarriorRoar;
class Clap;
class ShineHelper;
#pragma endregion

class AWarriorRoar : public Ability
{
public:
	AWarriorRoar();
	virtual ~AWarriorRoar();
private:
	weak_ptr<PlayerController>   _playerController;
	weak_ptr<ModelAnimator>		 _ownerAnimator;
	weak_ptr<TweenDesc>			 _tweenDesc;
	shared_ptr<WarriorRoar>		 _roarParticle;
	shared_ptr<Clap>			 _clapParticle;
	shared_ptr<ShineHelper>      _sparkParticle;
	shared_ptr<ParticleInstance> _roarInstance;
	shared_ptr<ParticleInstance> _clapInstance;
	shared_ptr<ParticleInstance> _sparkInstance;
	uint16						 _ownerAtk = 0;
	uint32						 _consumedMp = 0;
	float						 _abilityRange = 0.f;
	float						 _abilityDamage = 0.f;
private:
	void AoeDamageToTargets();
public:
	virtual void Enter(const shared_ptr<GameObject>& target) override;
	virtual void Execute() override;
	virtual void UpdateCoolTime() override;
};

