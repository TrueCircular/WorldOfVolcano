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
	shared_ptr<CharacterController>	_ownerController;
	shared_ptr<Transform>				_ownerTransform;
	shared_ptr<CharacterInfo>			_ownerInfo;
	shared_ptr<AbilitySlot>			_ownerAbilitySlot;
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
	const float& GetCoolTime() const { return _coolTime; }
	const float& GetCoolTimer() const { return _coolTimer; }
	const AbilityDataDesc& GetAbilityData() const { return _abilityData->GetAbilityData(); }
public:
	virtual void Enter(const shared_ptr<GameObject>& target) {}
	virtual void Execute(){}
	virtual void UpdateCoolTime() {}
};

