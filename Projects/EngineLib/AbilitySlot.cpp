#include "pch.h"
#include "AbilitySlot.h"
#include "CharacterController.h"

AbilitySlot::AbilitySlot()
{
}

AbilitySlot::~AbilitySlot()
{
}

void AbilitySlot::SetAbility(uint16 slotNum, const shared_ptr<Ability>& ability)
{
	if (slotNum < MAX_ABILITY_SLOT)
	{
		if (ability != nullptr)
		{
			_abilitySlot[slotNum] = ability;
		}
	}
}

void AbilitySlot::SetController(const shared_ptr<CharacterController>& controller)
{
	if (controller)
	{
		_controller = controller;
	}

	if (_controller.lock())
	{
		for (auto& ability : _abilitySlot)
		{
			if (ability != nullptr)
			{
				ability->SetOwnerController(_controller.lock());
			}
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

void AbilitySlot::ExecuteAbility(uint16 slotNum, shared_ptr<GameObject> target)
{
	if (slotNum < MAX_ABILITY_SLOT)
	{
		if (_abilitySlot[slotNum] != nullptr && _abilitySlot[slotNum]->IsCoolTime() == false)
		{
			_selectNumber = slotNum;
			_abilitySlot[slotNum]->Enter(target);
		}
	}
}

void AbilitySlot::Update()
{
	if (_controller.lock() != nullptr)
	{
		for (const auto& ability : _abilitySlot)
		{
			if (ability != nullptr)
				ability->UpdateCoolTime();
		}

		if (_selectNumber >= 0 && _selectNumber < MAX_ABILITY_SLOT)
		{
			if (_abilitySlot[_selectNumber]->IsCoolTime() == false)
				_abilitySlot[_selectNumber]->Execute();
		}
	}
}
