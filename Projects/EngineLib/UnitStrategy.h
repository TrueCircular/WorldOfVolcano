#pragma once

#pragma region Forward Declaration
class AIController;
#pragma endregion

class UnitStrategy
{
public:
	UnitStrategy() { _type = UnitStrategyType::None; }
	virtual ~UnitStrategy() {}
public:
	wstring			 _name;
	wstring			 _prevTransition;
	UnitStrategyType _type;
protected:
	shared_ptr<AIController>	_controller;
	shared_ptr<Transform>		_transform;
	shared_ptr<ModelAnimator>	_animator;
	shared_ptr<CharacterInfo>	_characterInfo;
	shared_ptr<Transform> _hostTargetTransform;
	uint32 _targetId = 0;
public:
	uint32 GetTargetId() { return _targetId; }
	void SetHostTarget(shared_ptr<Transform> hostTarget);
	wstring GetStrategyName() { return _name; }
	void UpdateInfo(MONSTER_INFO info);
	shared_ptr<Transform> GetWeakTransform() { return _transform; }
public:
	virtual void Enter(const shared_ptr<AIController>& controller, const wstring& prevTransition) = 0;
	virtual void Update() = 0;
	virtual void UpdateFromServer() = 0;
	virtual void Out(const wstring& nextTransition) = 0;
};

class StandStrategy : public UnitStrategy
{
public:
	StandStrategy() { _type = UnitStrategyType::Stand; }
	virtual ~StandStrategy(){}
public:
	virtual void Enter(const shared_ptr<AIController>& controller, const wstring& prevTransition) override {}
	virtual void Update() override {}
	virtual void UpdateFromServer() override {}
	virtual void Out(const wstring& nextTransition) override {}
};

class DamagedStrategy : public UnitStrategy
{
public:
	DamagedStrategy() { _type = UnitStrategyType::Damaged; }
	virtual ~DamagedStrategy() {}
public:
	virtual void Enter(const shared_ptr<AIController>& controller, const wstring& prevTransition) override {}
	virtual void Update() override {}
	virtual void UpdateFromServer() override {}
	virtual void Out(const wstring& nextTransition) override {}
};

class StunStrategy : public UnitStrategy
{
public:
	StunStrategy() { _type = UnitStrategyType::Stun; }
	virtual ~StunStrategy() {}
public:
	virtual void Enter(const shared_ptr<AIController>& controller, const wstring& prevTransition) override {}
	virtual void Update() override {}
	virtual void UpdateFromServer() override {}
	virtual void Out(const wstring& nextTransition) override {}
};

class DeadStrategy : public UnitStrategy
{
public:
	DeadStrategy() { _type = UnitStrategyType::Dead; }
	virtual ~DeadStrategy() {}
public:
	virtual void Enter(const shared_ptr<AIController>& controller, const wstring& prevTransition) override {}
	virtual void Update() override {}
	virtual void UpdateFromServer() override {}
	virtual void Out(const wstring& nextTransition) override {}
};

class TraceStrategy : public UnitStrategy
{
public:
	TraceStrategy() { _type = UnitStrategyType::Trace; }
	virtual ~TraceStrategy() {}
public:
	virtual void Enter(const shared_ptr<AIController>& controller, const wstring& prevTransition) override {}
	virtual void Update() override {}
	virtual void Out(const wstring& nextTransition) override {}
};

class MoveToSpwanPointStrategy : public UnitStrategy
{
public:
	MoveToSpwanPointStrategy() { _type = UnitStrategyType::MoveToSpwanPoint; }
	virtual ~MoveToSpwanPointStrategy() {}
public:
	virtual void Enter(const shared_ptr<AIController>& controller, const wstring& prevTransition) override {}
	virtual void Update() override {}
	virtual void UpdateFromServer() override {}
	virtual void Out(const wstring& nextTransition) override {}
};

class BattleStrategy : public UnitStrategy
{
public:
	BattleStrategy() { _type = UnitStrategyType::Battle; }
	virtual ~BattleStrategy() {}
public:
	virtual void Enter(const shared_ptr<AIController>& controller, const wstring& prevTransition) override {}
	virtual void Update() override {}
	virtual void UpdateFromServer() override {}
	virtual void Out(const wstring& nextTransition) override {}
};

class AttackStrategy : public UnitStrategy
{
public:
	AttackStrategy() { _type = UnitStrategyType::Attack; }
	virtual ~AttackStrategy() {}
public:
	virtual void Enter(const shared_ptr<AIController>& controller, const wstring& prevTransition) override {}
	virtual void Update() override {}
	virtual void UpdateFromServer() override {}
	virtual void Out(const wstring& nextTransition) override {}
};

class AbilityStrategy : public UnitStrategy
{
public:
	AbilityStrategy() { _type = UnitStrategyType::Ability; }
	virtual ~AbilityStrategy() {}
public:
	virtual void Enter(const shared_ptr<AIController>& controller, const wstring& prevTransition) override {}
	virtual void Update() override {}
	virtual void UpdateFromServer() override {}
	virtual void Out(const wstring& nextTransition) override {}
};

class EventStrategy : public UnitStrategy
{
public:
	EventStrategy() { _type = UnitStrategyType::Event; }
	virtual ~EventStrategy() {}
public:
	virtual void Enter(const shared_ptr<AIController>& controller, const wstring& prevTransition) override{}
	virtual void Update() override{}
	virtual void UpdateFromServer() override {}
	virtual void Out(const wstring& nextTransition) override{}
};