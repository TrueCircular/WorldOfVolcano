#pragma once
#include "UnitStrategy.h"

#pragma region Forward Declaration
class PlayableUnit;
class CharacterInfo;
class AIController;
class Sounds;
#pragma endregion

class CoreHoundStand : public StandStrategy
{
public:
	CoreHoundStand();
	virtual ~CoreHoundStand();
public:
	virtual void Enter(const shared_ptr<AIController>& controller) override;
	virtual void Update() override;
	virtual void Out(const wstring& transition) override;
};

class CoreHoundDamaged : public DamagedStrategy
{
public:
	CoreHoundDamaged();
	virtual ~CoreHoundDamaged();
public:
	virtual void Enter(const shared_ptr<AIController>& controller) override;
	virtual void Update() override;
	virtual void Out(const wstring& transition) override;
};

class CoreHoundStun : public StunStrategy
{
public:
	CoreHoundStun();
	virtual ~CoreHoundStun();
public:
	virtual void Enter(const shared_ptr<AIController>& controller) override;
	virtual void Update() override;
	virtual void Out(const wstring& transition) override;
};

class CoreHoundDead : public DeadStrategy
{
public:
	CoreHoundDead();
	virtual ~CoreHoundDead();
public:
	virtual void Enter(const shared_ptr<AIController>& controller) override;
	virtual void Update() override;
	virtual void Out(const wstring& transition) override;
};

class CoreHoundTrace : public TraceStrategy
{
public:
	CoreHoundTrace();
	virtual ~CoreHoundTrace();
public:
	virtual void Enter(const shared_ptr<AIController>& controller) override;
	virtual void Update() override;
	virtual void Out(const wstring& transition) override;
};

class CoreHoundMoveToSpwanPoint : public MoveToSpwanPointStrategy
{
public:
	CoreHoundMoveToSpwanPoint();
	virtual ~CoreHoundMoveToSpwanPoint();
public:
	virtual void Enter(const shared_ptr<AIController>& controller) override;
	virtual void Update() override;
	virtual void Out(const wstring& transition) override;
};

class CoreHoundBattle : public BattleStrategy
{
public:
	CoreHoundBattle();
	virtual ~CoreHoundBattle();
public:
	virtual void Enter(const shared_ptr<AIController>& controller) override;
	virtual void Update() override;
	virtual void Out(const wstring& transition) override;
};

class CoreHoundAttack : public AttackStrategy
{
public:
	CoreHoundAttack();
	virtual ~CoreHoundAttack();
public:
	virtual void Enter(const shared_ptr<AIController>& controller) override;
	virtual void Update() override;
	virtual void Out(const wstring& transition) override;
};