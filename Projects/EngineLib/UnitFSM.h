#pragma once
#include "AIController.h"

class PlayableUnit;
class CharacterInfo;
class HeightGetter;

class UnitFSM
{
public:
	UnitFSM() {}
	virtual ~UnitFSM() {}
protected:
	weak_ptr<Transform>		_transform;
	weak_ptr<AIController>	_controller;
	weak_ptr<ModelAnimator>	_animator;
	weak_ptr<CharacterInfo>	_characterInfo;
public:
	virtual void Enter(const shared_ptr<AIController>& controller){}
	virtual void Update(){}
	virtual void Out(UnitFSMState transition){}
};

class UnitFSMStand : public UnitFSM
{
	using TarceTargetList = unordered_set<shared_ptr<PlayableUnit>>;
public:
	UnitFSMStand();
	virtual ~UnitFSMStand();
private:
	float					_traceRadius = 0.f;
	float					_attackRange = 0.f;
	TarceTargetList			_targetList;
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
	uint16					_moveSpeed = 0;
	float					_traceRadius = 0.f;
	float					_attackRange = 0.f;
	float					_dt = 0.f;
	float					_totargetRotationSpeed = 5.0f;
	TarceTargetList			_targetList;
	weak_ptr<Transform>		_targetTransform;
private:

public:
	virtual void Enter(const shared_ptr<AIController>& controller) override;
	virtual void Update() override;
	virtual void Out(UnitFSMState transition) override;
};

class UnitFSMMoveToSpwanPoint : public UnitFSM
{
	using TarceTargetList = unordered_set<shared_ptr<PlayableUnit>>;
public:
	UnitFSMMoveToSpwanPoint();
	virtual ~UnitFSMMoveToSpwanPoint();
private:
	float				_traceRadius = 0.f;
	float				_dt = 0.f;
	float				_totargetRotationSpeed = 5.0f;
	uint16				_moveSpeed = 0;
	Vec3				_spwanPos = Vec3(0.f);
	TarceTargetList		_targetList;
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
	float					_traceRadius = 0.f;
	float					_attackRange = 0.f;
	float					_attackTime = 0.f;
	float					_attackTimeCal = 0.f;
	float					_dt = 0.f;
	float					_totargetRotationSpeed = 5.0f;
	weak_ptr<Transform>		_targetTransform;
public:
	virtual void Enter(const shared_ptr<AIController>& controller) override;
	virtual void Update() override;
	virtual void Out(UnitFSMState transition) override;
};

class UnitFSMAttack : public UnitFSM
{
public:
	UnitFSMAttack();;
	virtual ~UnitFSMAttack();
private:
	float					_traceRadius = 0.f;
	float					_attackRange = 0.f;
	float					_dt = 0.f;
	float					_totargetRotationSpeed = 5.0f;
	weak_ptr<Transform>		_targetTransform;
public:
	virtual void Enter(const shared_ptr<AIController>& controller) override;
	virtual void Update() override;
	virtual void Out(UnitFSMState transition) override;
};

class UnitFSMAbility1 : public UnitFSM
{
public:
	UnitFSMAbility1();
	virtual ~UnitFSMAbility1();
public:
	virtual void Enter(const shared_ptr<AIController>& controller) override;
	virtual void Update() override;
	virtual void Out(UnitFSMState transition) override;
};

class UnitFSMAbility2 : public UnitFSM
{
public:
	UnitFSMAbility2();
	virtual ~UnitFSMAbility2();
public:
	virtual void Enter(const shared_ptr<AIController>& controller) override;
	virtual void Update() override;
	virtual void Out(UnitFSMState transition) override;
};