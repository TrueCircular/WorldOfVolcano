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
public:
	BaronGeddonStand();
	virtual ~BaronGeddonStand();
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