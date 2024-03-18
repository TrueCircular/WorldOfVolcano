#pragma once
#include "MonoBehaviour.h"

class CharacterController : public MonoBehaviour
{
public:
	CharacterController(){}
	virtual ~CharacterController(){}
public:
	virtual void TakeDamage(const shared_ptr<GameObject>& sender, uint16 damage) {}
};

