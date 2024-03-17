#pragma once
#include "AbilityData.h"

#pragma region Forward Declaration
class CharacterInfo;
class CharacterController;
#pragma endregion

class Ability
{
public:
	Ability();
	virtual ~Ability();
protected:
	weak_ptr<CharacterController>	_ownerController;
	weak_ptr<CharacterController>	_targetController;
	weak_ptr<Transform>				_ownerTransform;
	weak_ptr<Transform>				_targetTransform;
	weak_ptr<CharacterInfo>			_ownerInfo;
	weak_ptr<CharacterInfo>			_targetInfo;
	shared_ptr<AbilityData>			_abilityData;
public:
	//Setter
	void SetOwnerController(const shared_ptr<CharacterController>& controller);
	void SetTargetController(const shared_ptr<CharacterController>& controller);
	void SetAbilityData(const shared_ptr<AbilityData>& data) { _abilityData = data; }
public:
	//Getter
	const shared_ptr<CharacterController>& GetOwnerController() const { return _ownerController.lock(); }
	const shared_ptr<CharacterController>& GetTargetController() const { return _targetController.lock(); }
	const shared_ptr<AbilityData>& GetAbilityData() const { return _abilityData; }
	const shared_ptr<Transform>& GetOwnerTransform() const { return _ownerTransform.lock(); }
	const shared_ptr<Transform>& GetTargetTransform() const { return _targetTransform.lock(); }
	const shared_ptr<CharacterInfo>& GetOwnerCharacterInfo() const { return _ownerInfo.lock(); }
	const shared_ptr<CharacterInfo>& GetTargetCharacterInfo() const { return _targetInfo.lock(); }
public:
	virtual void Execute(){}
};

