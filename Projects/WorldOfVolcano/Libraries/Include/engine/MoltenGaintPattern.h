#pragma once
#include "UnitStrategy.h"

#pragma region Forward Declaration
class PlayableUnit;
class CharacterInfo;
class AIController;
class Sounds;
#pragma endregion

class MoltenGaintStand : public StandStrategy
{
	using TarceTargetList = unordered_set<shared_ptr<PlayableUnit>>;
public:
	MoltenGaintStand();
	virtual ~MoltenGaintStand();
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

class MoltenGaintDamaged : public DamagedStrategy
{
public:
	MoltenGaintDamaged();
	virtual ~MoltenGaintDamaged();
public:
	virtual void Enter(const shared_ptr<AIController>& controller) override;
	virtual void Update() override;
	virtual void Out(const wstring& transition) override;
};

class MoltenGaintStun : public StunStrategy
{
public:
	MoltenGaintStun();
	virtual ~MoltenGaintStun();
public:
	virtual void Enter(const shared_ptr<AIController>& controller) override;
	virtual void Update() override;
	virtual void Out(const wstring& transition) override;
};

class MoltenGaintDead : public DeadStrategy
{
public:
	MoltenGaintDead();
	virtual ~MoltenGaintDead();
public:
	virtual void Enter(const shared_ptr<AIController>& controller) override;
	virtual void Update() override;
	virtual void Out(const wstring& transition) override;
};

class MoltenGaintTrace : public TraceStrategy
{
public:
	MoltenGaintTrace();
	virtual ~MoltenGaintTrace();
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

class MoltenGaintMoveToSpwanPoint : public MoveToSpwanPointStrategy
{
public:
	MoltenGaintMoveToSpwanPoint();
	virtual ~MoltenGaintMoveToSpwanPoint();
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

class MoltenGaintBattle : public BattleStrategy
{
public:
	MoltenGaintBattle();
	virtual ~MoltenGaintBattle();
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

class MoltenGaintAttack : public AttackStrategy
{
public:
	MoltenGaintAttack();
	virtual ~MoltenGaintAttack();
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