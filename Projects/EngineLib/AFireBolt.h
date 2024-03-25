#pragma once
#include "Ability.h"

#pragma region Forward Declaration
class CharacterInfo;
class CharacterController;
#pragma endregion

class AFireBolt :public Ability
{
public:
	AFireBolt();
	virtual ~AFireBolt();
public:
	virtual void Execute() override;
};

