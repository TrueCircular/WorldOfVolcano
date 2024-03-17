#pragma once
#include "Ability.h"

class WarriorRoar : public Ability
{
public:
	WarriorRoar();
	virtual ~WarriorRoar();
public:
	virtual void Execute() override;
};

