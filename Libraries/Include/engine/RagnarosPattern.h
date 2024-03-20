#pragma once
#include "UnitStrategy.h"

#pragma region Forward Declaration
class PlayableUnit;
class CharacterInfo;
class AIController;
class Sounds;
#pragma endregion

class RagnarosStand : public StandStrategy
{
public:
	RagnarosStand();
	virtual ~RagnarosStand();
public:
	virtual void Enter(const shared_ptr<AIController>& controller) override;
	virtual void Update() override;
	virtual void Out(const wstring& transition) override;
};

class RagnarosDamaged : public DamagedStrategy
{
public:
	RagnarosDamaged();
	virtual ~RagnarosDamaged();
public:
	virtual void Enter(const shared_ptr<AIController>& controller) override;
	virtual void Update() override;
	virtual void Out(const wstring& transition) override;
};

class RagnarosStun : public StunStrategy
{
public:
	RagnarosStun();
	virtual ~RagnarosStun();
public:
	virtual void Enter(const shared_ptr<AIController>& controller) override;
	virtual void Update() override;
	virtual void Out(const wstring& transition) override;
};

class RagnarosDead : public DeadStrategy
{
public:
	RagnarosDead();
	virtual ~RagnarosDead();
public:
	virtual void Enter(const shared_ptr<AIController>& controller) override;
	virtual void Update() override;
	virtual void Out(const wstring& transition) override;
};

class RagnarosTrace : public TraceStrategy
{
public:
	RagnarosTrace();
	virtual ~RagnarosTrace();
public:
	virtual void Enter(const shared_ptr<AIController>& controller) override;
	virtual void Update() override;
	virtual void Out(const wstring& transition) override;
};

class RagnarosMoveToSpwanPoint : public MoveToSpwanPointStrategy
{
public:
	RagnarosMoveToSpwanPoint();
	virtual ~RagnarosMoveToSpwanPoint();
public:
	virtual void Enter(const shared_ptr<AIController>& controller) override;
	virtual void Update() override;
	virtual void Out(const wstring& transition) override;
};

class RagnarosBattle : public BattleStrategy
{
public:
	RagnarosBattle();
	virtual ~RagnarosBattle();
public:
	virtual void Enter(const shared_ptr<AIController>& controller) override;
	virtual void Update() override;
	virtual void Out(const wstring& transition) override;
};

class RagnarosAttack : public AttackStrategy
{
public:
	RagnarosAttack();
	virtual ~RagnarosAttack();
public:
	virtual void Enter(const shared_ptr<AIController>& controller) override;
	virtual void Update() override;
	virtual void Out(const wstring& transition) override;
};

class RagnarosAbility : public AbilityStrategy
{
public:
	RagnarosAbility();
	virtual ~RagnarosAbility();
public:
	virtual void Enter(const shared_ptr<AIController>& controller) override;
	virtual void Update() override;
	virtual void Out(const wstring& transition) override;
};