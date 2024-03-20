#pragma once
#include "Ability.h"

class WarriorCharge : public Ability
{
public:
	WarriorCharge();
	virtual ~WarriorCharge();
public:
	virtual void Execute() override;
};

