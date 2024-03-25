#pragma once
#include "UnitStrategy.h"

#pragma region Forward Declaration
class CharacterInfo;
class AIController;
class Sounds;
class AbilitySlot;
struct TargetDesc;
#pragma endregion

class BaronGeddonStand : public StandStrategy
{
	using TargetList = unordered_set<shared_ptr<TargetDesc>>;
public:
	BaronGeddonStand();
	virtual ~BaronGeddonStand();
private:
	weak_ptr<AIController>	_controller;
	weak_ptr<Transform>		_transform;
	weak_ptr<ModelAnimator>	_animator;
	weak_ptr<CharacterInfo>	_characterInfo;
	weak_ptr<TargetList>	_targetList;
	Vec3					_spwanPos = Vec3(0.f);
	float					_traceRadius = 0.f;
	float					_attackRange = 0.f;
public:
	virtual void Enter(const shared_ptr<AIController>& controller, const wstring& prevTransition) override;
	virtual void Update() override;
	virtual void Out(const wstring& nextTransition) override;
};

class BaronGeddonDamaged : public DamagedStrategy
{
public:
	BaronGeddonDamaged();
	virtual ~BaronGeddonDamaged();
private:
	weak_ptr<AIController>	_controller;
	weak_ptr<ModelAnimator>	_animator;
	shared_ptr<Sounds>		_damagedSound;
public:
	virtual void Enter(const shared_ptr<AIController>& controller, const wstring& prevTransition) override;
	virtual void Update() override;
	virtual void Out(const wstring& nextTransition) override;
};

class BaronGeddonDead : public DeadStrategy
{
public:
	BaronGeddonDead();
	virtual ~BaronGeddonDead();
private:
	weak_ptr<AIController>	_controller;
	weak_ptr<ModelAnimator>	_animator;
	shared_ptr<Sounds>		_deadSound;
	float					_dt = 0.f;
	float					_soundTimer = 1.0f;
	bool					_soundFlag = false;
public:
	virtual void Enter(const shared_ptr<AIController>& controller, const wstring& prevTransition) override;
	virtual void Update() override;
	virtual void Out(const wstring& nextTransition) override;
};

class BaronGeddonTrace : public TraceStrategy
{
	using TargetList = unordered_set<shared_ptr<TargetDesc>>;
public:
	BaronGeddonTrace();
	virtual ~BaronGeddonTrace();
private:
	weak_ptr<AIController>	_controller;
	weak_ptr<Transform>		_transform;
	weak_ptr<TargetList>	_targetList;
	weak_ptr<Transform>		_targetTransform;
	weak_ptr<ModelAnimator>	_animator;
	weak_ptr<CharacterInfo>	_characterInfo;
	uint16					_moveSpeed = 0;
	float					_dt = 0.f;
	float					_traceRadius = 0.f;
	float					_attackRange = 0.f;
	float					_totargetRotationSpeed = 5.0f;
public:
	virtual void Enter(const shared_ptr<AIController>& controller, const wstring& prevTransition) override;
	virtual void Update() override;
	virtual void Out(const wstring& nextTransition) override;
};

class BaronGeddonMoveToSpwanPoint : public MoveToSpwanPointStrategy
{
public:
	BaronGeddonMoveToSpwanPoint();
	virtual ~BaronGeddonMoveToSpwanPoint();
private:
	weak_ptr<AIController>	_controller;
	weak_ptr<Transform>		_transform;
	weak_ptr<ModelAnimator>	_animator;
	weak_ptr<CharacterInfo>	_characterInfo;
	Vec3					_spwanPos = Vec3(0.f);
	uint16					_moveSpeed = 0;
	float					_dt = 0.f;
	float					_traceRadius = 0.f;
	float					_totargetRotationSpeed = 5.0f;
public:
	virtual void Enter(const shared_ptr<AIController>& controller, const wstring& prevTransition) override;
	virtual void Update() override;
	virtual void Out(const wstring& nextTransition) override;
};

class BaronGeddonBattle : public BattleStrategy
{
	using TargetList = unordered_set<shared_ptr<TargetDesc>>;
public:
	BaronGeddonBattle();
	virtual ~BaronGeddonBattle();
private:
	weak_ptr<AIController>	_controller;
	weak_ptr<Transform>		_transform;
	weak_ptr<TargetList>	_targetList;
	weak_ptr<Transform>		_targetTransform;
	weak_ptr<ModelAnimator>	_animator;
	weak_ptr<CharacterInfo>	_characterInfo;
	weak_ptr<AbilitySlot>	_abilitySlot;
	float					_dt = 0.f;
	float					_traceTime = 0.f;
	float					_traceWaitingTime = 0.75f;
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
	virtual void Out(const wstring& nextTransition) override;
};

class BaronGeddonAttack : public AttackStrategy
{
public:
	BaronGeddonAttack();
	virtual ~BaronGeddonAttack();
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
	virtual void Out(const wstring& nextTransition) override;
};

class BaronGeddonAbility : public AbilityStrategy
{
public:
	BaronGeddonAbility();
	virtual ~BaronGeddonAbility();
private:
	weak_ptr<AIController>	_controller;
	weak_ptr<Transform>		_transform;
	weak_ptr<Transform>		_targetTransform;
	weak_ptr<ModelAnimator>	_animator;
	weak_ptr<CharacterInfo>	_characterInfo;
	shared_ptr<Sounds>		_abiltySound;
	weak_ptr<AbilitySlot>	_abilitySlot;
	float					_dt = 0.f;
	float					_attackRange = 0.f;
	float					_traceRadius = 0.f;
	float					_totargetRotationSpeed = 5.0f;

	float					_abTime = 0.3f;
	float					_abTimer = 0.f;
	bool					_abFlag = false;
public:
	virtual void Enter(const shared_ptr<AIController>& controller, const wstring& prevTransition) override;
	virtual void Update() override;
	virtual void Out(const wstring& nextTransition) override;
};