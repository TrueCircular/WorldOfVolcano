#pragma once
#include "Ability.h"

#pragma region Forward Declaration
class AIController;
class CharacterInfo;
class FireStorm;
class MagicCircle;
#pragma endregion

class AFireStorm : public Ability
{
public:
	AFireStorm();
	virtual ~AFireStorm();
private:
	shared_ptr<AIController>			_aiController;
	shared_ptr<Transform>				_ownerTargetTransform;
	shared_ptr<FireStorm>			_fireStormParticle;
	shared_ptr<ParticleInstance>	_fireStormInstance;
	shared_ptr<MagicCircle>			_magicCircleParticle;
	shared_ptr<ParticleInstance>	_magicCircleInstance;
	uint16							_ownerAtk = 0;
	float							_abilityRange = 0.f;
	float							_abilityDamage = 0.f;
public:
	virtual void Enter(const shared_ptr<GameObject>& target) override;
	virtual void Execute() override;
	virtual void UpdateCoolTime() override;
};

