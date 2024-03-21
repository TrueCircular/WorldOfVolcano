#pragma once
#include "UnitStrategy.h"

#pragma region Forward Declaration
class PlayableUnit;
class CharacterInfo;
class AIController;
class Sounds;
#pragma endregion

class MoltenGiantStand : public StandStrategy
{
	using TarceTargetList = unordered_set<shared_ptr<PlayableUnit>>;
public:
	MoltenGiantStand();
	virtual ~MoltenGiantStand();
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

class MoltenGiantDamaged : public DamagedStrategy
{
public:
	MoltenGiantDamaged();
	virtual ~MoltenGiantDamaged();
public:
	virtual void Enter(const shared_ptr<AIController>& controller) override;
	virtual void Update() override;
	virtual void Out(const wstring& transition) override;
};

class MoltenGiantStun : public StunStrategy
{
public:
	MoltenGiantStun();
	virtual ~MoltenGiantStun();
public:
	virtual void Enter(const shared_ptr<AIController>& controller) override;
	virtual void Update() override;
	virtual void Out(const wstring& transition) override;
};

class MoltenGiantDead : public DeadStrategy
{
public:
	MoltenGiantDead();
	virtual ~MoltenGiantDead();
public:
	virtual void Enter(const shared_ptr<AIController>& controller) override;
	virtual void Update() override;
	virtual void Out(const wstring& transition) override;
};

class MoltenGiantTrace : public TraceStrategy
{
public:
	MoltenGiantTrace();
	virtual ~MoltenGiantTrace();
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
	virtual void Enter(const shared_ptr<AIController>& controller) override;
	virtual void Update() override;
	virtual void Out(const wstring& transition) override;
};

class MoltenGiantBattle : public BattleStrategy
{
public:
	MoltenGiantBattle();
	virtual ~MoltenGiantBattle();
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
	virtual void Enter(const shared_ptr<AIController>& controller) override;
	virtual void Update() override;
	virtual void Out(const wstring& transition) override;
};