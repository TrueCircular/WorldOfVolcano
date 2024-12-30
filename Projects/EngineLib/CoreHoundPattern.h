#pragma once
#include "UnitStrategy.h"

#pragma region Forward Declaration
class CharacterInfo;
class AIController;
class Sounds;
struct TargetDesc;
#pragma endregion

class CoreHoundStand : public StandStrategy
{
	using TargetList = unordered_set<shared_ptr<TargetDesc>>;
public:
	CoreHoundStand();
	virtual ~CoreHoundStand();
private:
	shared_ptr<TargetList>	_targetList;
	float					_traceRadius = 0.f;
	float					_attackRange = 0.f;
public:
	virtual void Enter(const shared_ptr<AIController>& controller, const wstring& prevTransition) override;
	virtual void Update() override;
	virtual void UpdateFromServer() override;
	virtual void Out(const wstring& nextTransition) override;
};

class CoreHoundDamaged : public DamagedStrategy
{
public:
	CoreHoundDamaged();
	virtual ~CoreHoundDamaged();
private:
	shared_ptr<Sounds>		_damagedSound;
public:
	virtual void Enter(const shared_ptr<AIController>& controller, const wstring& prevTransition) override;
	virtual void Update() override;
	virtual void UpdateFromServer() override;
	virtual void Out(const wstring& nextTransition) override;
};

class CoreHoundDead : public DeadStrategy
{
public:
	CoreHoundDead();
	virtual ~CoreHoundDead();
private:
	shared_ptr<Sounds>		_deadSound;
	float					_dt = 0.f;
	float					_soundTimer = 1.0f;
	bool					_soundFlag = false;
public:
	virtual void Enter(const shared_ptr<AIController>& controller, const wstring& prevTransition) override;
	virtual void Update() override;
	virtual void UpdateFromServer() override;
	virtual void Out(const wstring& nextTransition) override;
};

class CoreHoundTrace : public TraceStrategy
{
	using TargetList = unordered_set<shared_ptr<TargetDesc>>;
public:
	CoreHoundTrace();
	virtual ~CoreHoundTrace();
private:
	shared_ptr<TargetList>	_targetList;
	shared_ptr<Transform>		_targetTransform;
	uint16					_moveSpeed = 0;
	float					_dt = 0.f;
	float					_traceRadius = 0.f;
	float					_attackRange = 0.f;
	float					_totargetRotationSpeed = 5.0f;
public:
	virtual void Enter(const shared_ptr<AIController>& controller, const wstring& prevTransition) override;
	virtual void Update() override;
	virtual void UpdateFromServer() override;
	virtual void Out(const wstring& nextTransition) override;
};

class CoreHoundMoveToSpwanPoint : public MoveToSpwanPointStrategy
{
public:
	CoreHoundMoveToSpwanPoint();
	virtual ~CoreHoundMoveToSpwanPoint();
private:
	Vec3					_spwanPos = Vec3(0.f);
	uint16					_moveSpeed = 0;
	float					_dt = 0.f;
	float					_traceRadius = 0.f;
	float					_totargetRotationSpeed = 5.0f;
public:
	virtual void Enter(const shared_ptr<AIController>& controller, const wstring& prevTransition) override;
	virtual void Update() override;
	virtual void UpdateFromServer() override;
	virtual void Out(const wstring& nextTransition) override;
};

class CoreHoundBattle : public BattleStrategy
{
	using TargetList = unordered_set<shared_ptr<TargetDesc>>;
public:
	CoreHoundBattle();
	virtual ~CoreHoundBattle();
private:
	shared_ptr<TargetList>	_targetList;
	shared_ptr<Transform>		_targetTransform;
	float					_dt = 0.f;
	float					_traceTime = 0.f;
	float					_traceWaitingTime = 0.75f;
	float					_traceRadius = 0.f;
	float					_attackTime = 0.f;
	float					_attackTimeCal = 0.f;
	float					_attackRange = 0.f;
	float					_totargetRotationSpeed = 5.0f;
public:
	virtual void Enter(const shared_ptr<AIController>& controller, const wstring& prevTransition) override;
	virtual void Update() override;
	virtual void UpdateFromServer() override;
	virtual void Out(const wstring& nextTransition) override;
};

class CoreHoundAttack : public AttackStrategy
{
public:
	CoreHoundAttack();
	virtual ~CoreHoundAttack();
private:
	shared_ptr<Transform>		_targetTransform;
	shared_ptr<Sounds>		_attack1Sound;
	shared_ptr<Sounds>		_attack2Sound;
	float					_dt = 0.f;
	float					_attackRange = 0.f;
	float					_traceRadius = 0.f;
	float					_totargetRotationSpeed = 5.0f;
public:
	virtual void Enter(const shared_ptr<AIController>& controller, const wstring& prevTransition) override;
	virtual void Update() override;
	virtual void UpdateFromServer() override;
	virtual void Out(const wstring& nextTransition) override;
};