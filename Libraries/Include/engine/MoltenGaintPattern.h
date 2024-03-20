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
public:
	MoltenGaintStand();
	virtual ~MoltenGaintStand();
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
public:
	virtual void Enter(const shared_ptr<AIController>& controller) override;
	virtual void Update() override;
	virtual void Out(const wstring& transition) override;
};