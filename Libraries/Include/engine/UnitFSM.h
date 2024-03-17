#pragma once
#include "AIController.h"

class PlayableUnit;
class CharacterInfo;



class UnitFSM
{
public:
	UnitFSM() {}
	virtual ~UnitFSM() {}
protected:
	weak_ptr<Transform>		_transform;
	weak_ptr<AIController>	_controller;
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
	float					_attackDistance = 0.f;
	TarceTargetList			_targetList;
	weak_ptr<ModelAnimator> _animator;
	weak_ptr<CharacterInfo> _characterInfo;
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
	float					_traceRadius = 0.f;
	TarceTargetList			_targetList;
	weak_ptr<ModelAnimator> _animator;
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
	uint32 _toAttackDamage = 0;
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