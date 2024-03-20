#pragma once
#include "AIController.h"
#include "UnitStrategy.h"

#pragma region Forward Declaration
class PlayableUnit;
class CharacterInfo;
class Sounds;
#pragma endregion

class UnitFSM
{
public:
	UnitFSM() {}
	virtual ~UnitFSM() {}
protected:
	using TarceTargetList = unordered_set<shared_ptr<PlayableUnit>>;
protected:
	weak_ptr<Transform>		_transform;
	weak_ptr<AIController>	_controller;
	weak_ptr<ModelAnimator>	_animator;
	weak_ptr<CharacterInfo>	_characterInfo;
	TarceTargetList			_targetList;
public:
	void SetStrategy();
public:
	virtual void Enter(const shared_ptr<AIController>& controller){}
	virtual void Update(){}
	virtual void Out(UnitFSMState transition){}
};

class UnitFSMStand : public UnitFSM
{
public:
	UnitFSMStand();
	virtual ~UnitFSMStand();
private:
	float					_traceRadius = 0.f;
	float					_attackRange = 0.f;
private:
	void SearchTraceTarget();
public:
	virtual void Enter(const shared_ptr<AIController>& controller) override;
	virtual void Update() override;
	virtual void Out(UnitFSMState transition) override;
};

class UnitFSMTrace : public UnitFSM
{
	using TarceTargetList = unordered_set<shared_ptr<PlayableUnit>>;
public:
	UnitFSMTrace();
	virtual ~UnitFSMTrace();
private:
	weak_ptr<Transform>		_targetTransform;
	uint16					_moveSpeed = 0;
	float					_traceRadius = 0.f;
	float					_attackRange = 0.f;
	float					_dt = 0.f;
	float					_totargetRotationSpeed = 5.0f;
private:

public:
	virtual void Enter(const shared_ptr<AIController>& controller) override;
	virtual void Update() override;
	virtual void Out(UnitFSMState transition) override;
};

class UnitFSMMoveToSpwanPoint : public UnitFSM
{
public:
	UnitFSMMoveToSpwanPoint();
	virtual ~UnitFSMMoveToSpwanPoint();
private:
	float				_traceRadius = 0.f;
	float				_dt = 0.f;
	float				_totargetRotationSpeed = 5.0f;
	uint16				_moveSpeed = 0;
	Vec3				_spwanPos = Vec3(0.f);
private:
	void SearchTraceTarget();
public:
	virtual void Enter(const shared_ptr<AIController>& controller) override;
	virtual void Update() override;
	virtual void Out(UnitFSMState transition) override;
};

class UnitFSMBattle : public UnitFSM
{
public:
	UnitFSMBattle();
	virtual ~UnitFSMBattle();
private:
	weak_ptr<Transform>		_targetTransform;
	float					_traceRadius = 0.f;
	float					_traceTime = 0.f;
	float					_traceWaitingTime = 0.75f;
	float					_totargetRotationSpeed = 5.0f;
	float					_attackRange = 0.f;
	float					_attackTime = 0.f;
	float					_attackTimeCal = 0.f;
	float					_dt = 0.f;
public:
	virtual void Enter(const shared_ptr<AIController>& controller) override;
	virtual void Update() override;
	virtual void Out(UnitFSMState transition) override;
};

class UnitFSMAttack : public UnitFSM
{
public:
	UnitFSMAttack();
	virtual ~UnitFSMAttack();
private:
	float					_traceRadius = 0.f;
	float					_attackRange = 0.f;
	float					_dt = 0.f;
	float					_totargetRotationSpeed = 5.0f;
	weak_ptr<Transform>		_targetTransform;
private:
	shared_ptr<Sounds>		_attack1Sound;
	shared_ptr<Sounds>		_attack2Sound;
public:
	virtual void Enter(const shared_ptr<AIController>& controller) override;
	virtual void Update() override;
	virtual void Out(UnitFSMState transition) override;
};

class UnitFSMAbility : public UnitFSM
{
public:
	UnitFSMAbility();
	virtual ~UnitFSMAbility();
public:
	virtual void Enter(const shared_ptr<AIController>& controller) override;
	virtual void Update() override;
	virtual void Out(UnitFSMState transition) override;
};
