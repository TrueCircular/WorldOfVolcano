#include "pch.h"
#include "Ability.h"
#include "CharacterInfo.h"
#include "CharacterController.h"

Ability::Ability()
{
}

Ability::~Ability()
{
}

void Ability::SetOwnerController(const shared_ptr<CharacterController>& controller)
{
	if (controller == nullptr)
		return;

	_ownerController = controller;
}

void Ability::SetTargetController(const shared_ptr<CharacterController>& controller)
{
	if (controller == nullptr)
		return;

	_targetController = controller;
}
