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
	virtual void Out(UnitFSMState transition) override;
};

class MoltenGaintDamaged : public DamagedStrategy
{
public:
	MoltenGaintDamaged();
	virtual ~MoltenGaintDamaged();
public:
	virtual void Enter(const shared_ptr<AIController>& controller) override;
	virtual void Update() override;
	virtual void Out(UnitFSMState transition) override;
};

class MoltenGaintStun : public StunStrategy
{
public:
	MoltenGaintStun();
	virtual ~MoltenGaintStun();
public:
	virtual void Enter(const shared_ptr<AIController>& controller) override;
	virtual void Update() override;
	virtual void Out(UnitFSMState transition) override;
};

class MoltenGaintDead : public DeadStrategy
{
public:
	MoltenGaintDead();
	virtual ~MoltenGaintDead();
public:
	virtual void Enter(const shared_ptr<AIController>& controller) override;
	virtual void Update() override;
	virtual void Out(UnitFSMState transition) override;
};