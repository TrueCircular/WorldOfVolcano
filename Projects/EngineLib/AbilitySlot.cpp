#include "pch.h"
#include "AbilitySlot.h"

AbilitySlot::AbilitySlot()
{
}

AbilitySlot::~AbilitySlot()
{
}

void AbilitySlot::SetAbility(uint16 slotNum, shared_ptr<Ability> ability)
{
	if (slotNum < MAX_ABILITY_SLOT)
	{
		if (ability != nullptr)
		{
			_abilitySlot[slotNum] = ability;
		}
	}
}

const shared_ptr<Ability>& AbilitySlot::GetAbility(uint16 slotNum)
{
	shared_ptr<Ability> rAbility = nullptr;

	if (slotNum < MAX_ABILITY_SLOT)
	{
		rAbility = _abilitySlot[slotNum];
	}
	
	return rAbility;
}
