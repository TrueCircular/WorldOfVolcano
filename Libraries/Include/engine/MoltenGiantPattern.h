#pragma once
#include "UnitStrategy.h"

#pragma region Forward Declaration
class CharacterInfo;
class AIController;
class Sounds;
struct TargetDesc;
#pragma endregion

class MoltenGiantStand : public StandStrategy
{
	using TargetList = unordered_set<shared_ptr<TargetDesc>>;
public:
	MoltenGiantStand();
	virtual ~MoltenGiantStand();
private:
	weak_ptr<AIController>	_controller;
	weak_ptr<Transform>		_transform;
	weak_ptr<ModelAnimator>	_animator;
	weak_ptr<CharacterInfo>	_characterInfo;
	weak_ptr<TargetList>	_targetList;
	float					_traceRadius = 0.f;
	float					_attackRange = 0.f;
public:
	virtual void Enter(const shared_ptr<AIController>& controller, const wstring& prevTransition) override;
	virtual void Update() override;
	virtual void Out(const wstring& nextTransition) override;
};

class MoltenGiantDamaged : public DamagedStrategy
{
public:
	MoltenGiantDamaged();
	virtual ~MoltenGiantDamaged();
private:
	weak_ptr<AIController>	_controller;
	weak_ptr<ModelAnimator>	_animator;
	shared_ptr<Sounds>		_damagedSound;
public:
	virtual void Enter(const shared_ptr<AIController>& controller, const wstring& prevTransition) override;
	virtual void Update() override;
	virtual void Out(const wstring& nextTransition) override;
};

class MoltenGiantStun : public StunStrategy
{
public:
	MoltenGiantStun();
	virtual ~MoltenGiantStun();
private:
	weak_ptr<AIController>	_controller;
	weak_ptr<ModelAnimator>	_animator;
	float _stunTime = 1.5f;
	float _stunTimer = 0.f;
	float _dt = 0.f;
public:
	virtual void Enter(const shared_ptr<AIController>& controller, const wstring& prevTransition) override;
	virtual void Update() override;
	virtual void Out(const wstring& nextTransition) override;
};

class MoltenGiantDead : public DeadStrategy
{
public:
	MoltenGiantDead();
	virtual ~MoltenGiantDead();
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

class MoltenGiantTrace : public TraceStrategy
{
	using TargetList = unordered_set<shared_ptr<TargetDesc>>;
public:
	MoltenGiantTrace();
	virtual ~MoltenGiantTrace();
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

class MoltenGiantMoveToSpwanPoint : public MoveToSpwanPointStrategy
{
public:
	MoltenGiantMoveToSpwanPoint();
	virtual ~MoltenGiantMoveToSpwanPoint();
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

class MoltenGiantBattle : public BattleStrategy
{
	using TargetList = unordered_set<shared_ptr<TargetDesc>>;
public:
	MoltenGiantBattle();
	virtual ~MoltenGiantBattle();
private:
	weak_ptr<AIController>	_controller;
	weak_ptr<Transform>		_transform;
	weak_ptr<TargetList>	_targetList;
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
	virtual void Enter(const shared_ptr<AIController>& controller, const wstring& prevTransition) override;
	virtual void Update() override;
	virtual void Out(const wstring& nextTransition) override;
};

class MoltenGiantAttack : public AttackStrategy
{
public:
	MoltenGiantAttack();
	virtual ~MoltenGiantAttack();
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