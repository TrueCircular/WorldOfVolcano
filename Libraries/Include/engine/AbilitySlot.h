#pragma once
#include "MonoBehaviour.h"
#include "Ability.h"

#define MAX_ABILITY_SLOT (size_t)2

class AbilitySlot : public MonoBehaviour
{
public:
	AbilitySlot();
	virtual ~AbilitySlot();
public:
	array<shared_ptr<Ability>, MAX_ABILITY_SLOT> _abilitySlot;
public:
	void SetAbility(uint16 slotNum, shared_ptr<Ability> ability);
	const shared_ptr<Ability>& GetAbility(uint16 slotNum);
public:
	void ExecuteAbility(uint16 slotNum, shared_ptr<Transform> target);
};

