#pragma once
#include "MonoBehaviour.h"
#include "Ability.h"

#define MAX_ABILITY_SLOT (size_t)4

class CharacterController;

class AbilitySlot : public MonoBehaviour
{
public:
	AbilitySlot();
	virtual ~AbilitySlot();
public:
	array<shared_ptr<Ability>, MAX_ABILITY_SLOT> _abilitySlot;
	weak_ptr<CharacterController> _controller;
	int _selectNumber = -1;
public:
	void SetAbility(uint16 slotNum, const shared_ptr<Ability>& ability);
	void SetController(const shared_ptr<CharacterController>& controller);
	const shared_ptr<Ability>& GetAbility(uint16 slotNum);
public:
	void ExecuteAbility(uint16 slotNum, shared_ptr<GameObject> target);
public:
	virtual void Update() override;
};

