#pragma once
#include "UnitStrategy.h"

#pragma region Forward Declaration
class CharacterInfo;
class AIController;
class Sounds;
class AbilitySlot;
struct TargetDesc;
#pragma endregion

class RagnarosStand : public StandStrategy
{
public:
	RagnarosStand();
	virtual ~RagnarosStand();
private:
	weak_ptr<AIController>	_controller;
	weak_ptr<Transform>		_transform;
	weak_ptr<ModelAnimator>	_animator;
public:
	virtual void Enter(const shared_ptr<AIController>& controller, const wstring& prevTransition) override;
	virtual void Update() override;
	virtual void UpdateFromServer() override {}
	virtual void Out(const wstring& nextTransition) override;
};

class RagnarosDamaged : public DamagedStrategy
{
public:
	RagnarosDamaged();
	virtual ~RagnarosDamaged();
private:
	weak_ptr<AIController>	_controller;
	weak_ptr<ModelAnimator>	_animator;
public:
	virtual void Enter(const shared_ptr<AIController>& controller, const wstring& prevTransition) override;
	virtual void Update() override;
	virtual void UpdateFromServer() override {}
	virtual void Out(const wstring& nextTransition) override;
};

class RagnarosStun : public StunStrategy
{
public:
	RagnarosStun();
	virtual ~RagnarosStun();
private:
	weak_ptr<AIController>	_controller;
	weak_ptr<ModelAnimator>	_animator;
	float					_stunTime = 10.f;
	float					_stunTimer = 0.f;
public:
	virtual void Enter(const shared_ptr<AIController>& controller, const wstring& prevTransition) override;
	virtual void Update() override;
	virtual void UpdateFromServer() override {}
	virtual void Out(const wstring& nextTransition) override;
};

class RagnarosDead : public DeadStrategy
{
public:
	RagnarosDead();
	virtual ~RagnarosDead();
private:
	weak_ptr<AIController>	_controller;
	weak_ptr<ModelAnimator>	_animator;
	shared_ptr<Sounds>		_deadSound;
	float					_dt = 0.f;
public:
	virtual void Enter(const shared_ptr<AIController>& controller, const wstring& prevTransition) override;
	virtual void Update() override;
	virtual void UpdateFromServer() override {}
	virtual void Out(const wstring& nextTransition) override;
};

class RagnarosBattle : public BattleStrategy
{
	using TargetList = unordered_set<shared_ptr<TargetDesc>>;
public:
	RagnarosBattle();
	virtual ~RagnarosBattle();
private:
	weak_ptr<AIController>	_controller;
	weak_ptr<Transform>		_transform;
	weak_ptr<TargetList>	_targetList;
	weak_ptr<Transform>		_targetTransform;
	weak_ptr<ModelAnimator>	_animator;
	weak_ptr<CharacterInfo>	_characterInfo;
	weak_ptr<AbilitySlot>	_abilitySlot;
	float					_dt = 0.f;
	float					_traceRadius = 0.f;
	float					_attackTime = 0.f;
	float					_attackTimeCal = 0.f;
	float					_attackRange = 0.f;
	float					_abilityTime = 0.f;
	float					_abilityTimer = 0.f;
	float					_totargetRotationSpeed = 5.0f;
public:
	virtual void Enter(const shared_ptr<AIController>& controller, const wstring& prevTransition) override;
	virtual void Update() override;
	virtual void UpdateFromServer() override {}
	virtual void Out(const wstring& nextTransition) override;
};

class RagnarosAttack : public AttackStrategy
{
public:
	RagnarosAttack();
	virtual ~RagnarosAttack();
private:
	weak_ptr<AIController>	_controller;
	weak_ptr<Transform>		_transform;
	weak_ptr<Transform>		_targetTransform;
	weak_ptr<ModelAnimator>	_animator;
	weak_ptr<CharacterInfo>	_characterInfo;
	shared_ptr<Sounds>		_attack1Sound;
	shared_ptr<Sounds>		_attack2Sound;
	float					_dt = 0.f;
	float					_attackRange = 0.f;
	float					_traceRadius = 0.f;
	float					_totargetRotationSpeed = 5.0f;
public:
	virtual void Enter(const shared_ptr<AIController>& controller, const wstring& prevTransition) override;
	virtual void Update() override;
	virtual void UpdateFromServer() override {}
	virtual void Out(const wstring& nextTransition) override;
};

class RagnarosAbility : public AbilityStrategy
{
public:
	RagnarosAbility();
	virtual ~RagnarosAbility();
public:
	virtual void Enter(const shared_ptr<AIController>& controller, const wstring& prevTransition) override;
	virtual void Update() override;
	virtual void UpdateFromServer() override {}
	virtual void Out(const wstring& nextTransition) override;
};

class RagnarosEncounterEvent1 : public EventStrategy
{
public:
	RagnarosEncounterEvent1();
	virtual ~RagnarosEncounterEvent1();
public:
	virtual void Enter(const shared_ptr<AIController>& controller, const wstring& prevTransition) override;
	virtual void Update() override;
	virtual void Out(const wstring& nextTransition) override;
};