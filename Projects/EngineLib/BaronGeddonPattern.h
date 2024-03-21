#pragma once
#include "UnitStrategy.h"

#pragma region Forward Declaration
class PlayableUnit;
class CharacterInfo;
class AIController;
class Sounds;
#pragma endregion

class BaronGeddonStand : public StandStrategy
{
	using TarceTargetList = unordered_set<shared_ptr<PlayableUnit>>;
public:
	BaronGeddonStand();
	virtual ~BaronGeddonStand();
private:
	weak_ptr<AIController>	_controller;
	weak_ptr<Transform>		_transform;
	weak_ptr<ModelAnimator>	_animator;
	weak_ptr<CharacterInfo>	_characterInfo;
	TarceTargetList			_targetList;
	float					_traceRadius = 0.f;
	float					_attackRange = 0.f;
public:
	virtual void Enter(const shared_ptr<AIController>& controller) override;
	virtual void Update() override;
	virtual void Out(const wstring& transition) override;
};

class BaronGeddonDamaged : public DamagedStrategy
{
public:
	BaronGeddonDamaged();
	virtual ~BaronGeddonDamaged();
public:
	virtual void Enter(const shared_ptr<AIController>& controller) override;
	virtual void Update() override;
	virtual void Out(const wstring& transition) override;
};

class BaronGeddonStun : public StunStrategy
{
public:
	BaronGeddonStun();
	virtual ~BaronGeddonStun();
public:
	virtual void Enter(const shared_ptr<AIController>& controller) override;
	virtual void Update() override;
	virtual void Out(const wstring& transition) override;
};

class BaronGeddonDead : public DeadStrategy
{
public:
	BaronGeddonDead();
	virtual ~BaronGeddonDead();
public:
	virtual void Enter(const shared_ptr<AIController>& controller) override;
	virtual void Update() override;
	virtual void Out(const wstring& transition) override;
};

class BaronGeddonTrace : public TraceStrategy
{
public:
	BaronGeddonTrace();
	virtual ~BaronGeddonTrace();
private:
	weak_ptr<AIController>	_controller;
	weak_ptr<Transform>		_transform;
	weak_ptr<Transform>		_targetTransform;
	weak_ptr<ModelAnimator>	_animator;
	weak_ptr<CharacterInfo>	_characterInfo;
	uint16					_moveSpeed = 0;
	float					_dt = 0.f;
	float					_traceRadius = 0.f;
	float					_attackRange = 0.f;
	float					_totargetRotationSpeed = 5.0f;
public:
	virtual void Enter(const shared_ptr<AIController>& controller) override;
	virtual void Update() override;
	virtual void Out(const wstring& transition) override;
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
	virtual void Enter(const shared_ptr<AIController>& controller) override;
	virtual void Update() override;
	virtual void Out(const wstring& transition) override;
};

class BaronGeddonBattle : public BattleStrategy
{
public:
	BaronGeddonBattle();
	virtual ~BaronGeddonBattle();
private:
	weak_ptr<AIController>	_controller;
	weak_ptr<Transform>		_transform;
	weak_ptr<Transform>		_targetTransform;
	weak_ptr<ModelAnimator>	_animator;
	weak_ptr<CharacterInfo>	_characterInfo;
	float					_dt = 0.f;
	float					_traceTime = 0.f;
	float					_traceWaitingTime = 0.75f;
	float					_traceRadius = 0.f;
	float					_attackTime = 0.f;
	float					_attackTimeCal = 0.f;
	float					_attackRange = 0.f;
	float					_totargetRotationSpeed = 5.0f;
public:
	virtual void Enter(const shared_ptr<AIController>& controller) override;
	virtual void Update() override;
	virtual void Out(const wstring& transition) override;
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
	virtual void Enter(const shared_ptr<AIController>& controller) override;
	virtual void Update() override;
	virtual void Out(const wstring& transition) override;
};

class BaronGeddonAbility : public AbilityStrategy
{
public:
	BaronGeddonAbility();
	virtual ~BaronGeddonAbility();
public:
	virtual void Enter(const shared_ptr<AIController>& controller) override;
	virtual void Update() override;
	virtual void Out(const wstring& transition) override;
};