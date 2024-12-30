#pragma once
#include "Ability.h"
#pragma region Forward Declaration
class CharacterInfo;
class PlayerController;
class AbilitySlot;

#pragma endregion

class AHealPotion : public Ability
{
public:
	AHealPotion();
	virtual ~AHealPotion();

private:

	shared_ptr<PlayerController>   _playerController;

	shared_ptr<GameObject>	 _ownerPicked;



	float						 _abilityDamage = 0.f;

private:
	void HealPotion();
public:
	virtual void Enter(const shared_ptr<GameObject>& target) override;
	virtual void Execute() override;
	virtual void UpdateCoolTime() override;
};

