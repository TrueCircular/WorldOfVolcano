#pragma once
#include "Ability.h"
#pragma region Forward Declaration
class CharacterInfo;
class PlayerController;
class AbilitySlot;

#pragma endregion

class AManaPotion : public Ability
{
public:
	AManaPotion();
	virtual ~AManaPotion();

private:

	shared_ptr<PlayerController>   _playerController;

	shared_ptr<GameObject>	 _ownerPicked;



	float						 _abilityDamage = 0.f;

private:
	void ManaPotion();
public:
	virtual void Enter(const shared_ptr<GameObject>& target) override;
	virtual void Execute() override;
	virtual void UpdateCoolTime() override;
};

