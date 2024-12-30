#pragma once
#include "Ability.h"

#pragma region Forward Declaration
class AIController;
class CharacterInfo;
class FireBall;
#pragma endregion

class AFireBolt :public Ability
{
public:
	AFireBolt();
	virtual ~AFireBolt();
private:
	shared_ptr<AIController>			_aiController;
	shared_ptr<Transform>				_ownerTargetTransform;
	shared_ptr<FireBall>			_fireBoltParticle;
	shared_ptr<ParticleInstance>	_fireBoltInstance;
	uint16							_ownerAtk = 0;
	float							_abilityRange = 0.f;
	float							_abilityDamage = 0.f;
public:
	virtual void Enter(const shared_ptr<GameObject>& target) override;
	virtual void Execute() override;
	virtual void UpdateCoolTime() override;
};

