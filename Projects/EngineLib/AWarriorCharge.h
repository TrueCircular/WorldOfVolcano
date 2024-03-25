#pragma once
#include "Ability.h"

#pragma region Forward Declaration
class CharacterInfo;
class CharacterController;
#pragma endregion

class AWarriorCharge : public Ability
{
public:
	AWarriorCharge();
	virtual ~AWarriorCharge();
public:
	virtual void Execute() override;
};

