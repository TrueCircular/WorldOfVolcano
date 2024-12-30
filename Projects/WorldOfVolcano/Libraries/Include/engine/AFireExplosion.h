#pragma once
#include "Ability.h"

#pragma region Forward Declaration
class AIController;
class CharacterInfo;
class RagExplode;
#pragma endregion

class AFireExplosion : public Ability
{
public:
	AFireExplosion();
	virtual ~AFireExplosion();
private:
	shared_ptr<AIController>			_aiController;
	shared_ptr<Transform>				_ownerTargetTransform;
	shared_ptr<RagExplode>			_fireExplosionParticle;
	shared_ptr<ParticleInstance>	_fireExplosionInstance;
	uint16							_ownerAtk = 0;
	float							_abilityRange = 0.f;
	float							_abilityDamage = 0.f;
public:
	virtual void Enter(const shared_ptr<GameObject>& target) override;
	virtual void Execute() override;
	virtual void UpdateCoolTime() override;
};

