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
	virtual void Out(const wstring& nextTransition) override;
};

class RagnarosStun : public StunStrategy
{
public:
	RagnarosStun();
	virtual ~RagnarosStun();
private:
	weak_ptr<AIController>	_controller;
	weak_ptr<Transform>		_transform;
	weak_ptr<ModelAnimator>	_animator;
	weak_ptr<CharacterInfo>	_characterInfo;
	float					_traceRadius = 0.f;
	float					_attackRange = 0.f;
public:
	virtual void Enter(const shared_ptr<AIController>& controller, const wstring& prevTransition) override;
	virtual void Update() override;
	virtual void Out(const wstring& nextTransition) override;
};

class RagnarosDead : public DeadStrategy
{
public:
	RagnarosDead();
	virtual ~RagnarosDead();
public:
	virtual void Enter(const shared_ptr<AIController>& controller, const wstring& prevTransition) override;
	virtual void Update() override;
	virtual void Out(const wstring& nextTransition) override;
};

class RagnarosBattle : public BattleStrategy
{
public:
	RagnarosBattle();
	virtual ~RagnarosBattle();
public:
	virtual void Enter(const shared_ptr<AIController>& controller, const wstring& prevTransition) override;
	virtual void Update() override;
	virtual void Out(const wstring& nextTransition) override;
};

class RagnarosAttack : public AttackStrategy
{
public:
	RagnarosAttack();
	virtual ~RagnarosAttack();
public:
	virtual void Enter(const shared_ptr<AIController>& controller, const wstring& prevTransition) override;
	virtual void Update() override;
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
	virtual void Out(const wstring& nextTransition) override;
};

class RagnarosEncounterEvent : public EventStrategy
{
public:
	RagnarosEncounterEvent();
	virtual ~RagnarosEncounterEvent();
public:
	virtual void Enter(const shared_ptr<AIController>& controller, const wstring& prevTransition) override;
	virtual void Update() override;
	virtual void Out(const wstring& nextTransition) override;
};

class RagnarosSubMergeEvent : public EventStrategy
{
public:
	RagnarosSubMergeEvent();
	virtual ~RagnarosSubMergeEvent();
public:
	virtual void Enter(const shared_ptr<AIController>& controller, const wstring& prevTransition) override;
	virtual void Update() override;
	virtual void Out(const wstring& nextTransition) override;
};

class RagnarosSubMergedEvent : public EventStrategy
{
public:
	RagnarosSubMergedEvent();
	virtual ~RagnarosSubMergedEvent();
public:
	virtual void Enter(const shared_ptr<AIController>& controller, const wstring& prevTransition) override;
	virtual void Update() override;
	virtual void Out(const wstring& nextTransition) override;
};