#pragma once
#include "AbilityData.h"

#pragma region Forward Declaration
class CharacterInfo;
class CharacterController;
class AbilitySlot;
#pragma endregion

class Ability
{
public:
	Ability();
	virtual ~Ability();
protected:
	weak_ptr<CharacterController>	_ownerController;
	weak_ptr<Transform>				_ownerTransform;
	weak_ptr<CharacterInfo>			_ownerInfo;
	weak_ptr<AbilitySlot>			_ownerAbilitySlot;
	shared_ptr<AbilityData>			_abilityData;
	bool							_isCoolTime = false;
	float							_coolTime = 0.f;
	float							_coolTimer = 0.f;
public:
	//Setter
	void SetOwnerController(const shared_ptr<CharacterController>& controller);
	void SetAbilityData(const shared_ptr<AbilityData>& data) { _abilityData = data; }
	//Getter
	const bool& IsCoolTime() const { return _isCoolTime; }
public:
	virtual void Enter(const shared_ptr<GameObject>& target) {}
	virtual void Execute(){}
	virtual void UpdateCoolTime() {}
};

