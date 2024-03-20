#pragma once

#pragma region Forward Declaration
class AIController;
#pragma endregion

class UnitStrategy
{
public:
	UnitStrategy() {}
	virtual ~UnitStrategy() {}
public:
	wstring _name;
public:
	virtual void Enter(const shared_ptr<AIController>& controller) = 0;
	virtual void Update() = 0;
	virtual void Out(const wstring& transition) = 0;
};

class StandStrategy : public UnitStrategy
{
public:
	StandStrategy() {}
	virtual ~StandStrategy(){}
public:
	virtual void Enter(const shared_ptr<AIController>& controller) = 0;
	virtual void Update() = 0;
	virtual void Out(const wstring& transition) = 0;
};

class DamagedStrategy : public UnitStrategy
{
public:
	DamagedStrategy() {}
	virtual ~DamagedStrategy() {}
public:
	virtual void Enter(const shared_ptr<AIController>& controller) = 0;
	virtual void Update() = 0;
	virtual void Out(const wstring& transition) = 0;
};

class StunStrategy : public UnitStrategy
{
public:
	StunStrategy() {}
	virtual ~StunStrategy() {}
public:
	virtual void Enter(const shared_ptr<AIController>& controller) = 0;
	virtual void Update() = 0;
	virtual void Out(const wstring& transition) = 0;
};

class DeadStrategy : public UnitStrategy
{
public:
	DeadStrategy() {}
	virtual ~DeadStrategy() {}
public:
	virtual void Enter(const shared_ptr<AIController>& controller) = 0;
	virtual void Update() = 0;
	virtual void Out(const wstring& transition) = 0;
};

class TraceStrategy : public UnitStrategy
{
public:
	TraceStrategy() {}
	virtual ~TraceStrategy() {}
public:
	virtual void Enter(const shared_ptr<AIController>& controller) = 0;
	virtual void Update() = 0;
	virtual void Out(const wstring& transition) = 0;
};

class MoveToSpwanPointStrategy : public UnitStrategy
{
public:
	MoveToSpwanPointStrategy() {}
	virtual ~MoveToSpwanPointStrategy() {}
public:
	virtual void Enter(const shared_ptr<AIController>& controller) = 0;
	virtual void Update() = 0;
	virtual void Out(const wstring& transition) = 0;
};

class BattleStrategy : public UnitStrategy
{
public:
	BattleStrategy() {}
	virtual ~BattleStrategy() {}
public:
	virtual void Enter(const shared_ptr<AIController>& controller) = 0;
	virtual void Update() = 0;
	virtual void Out(const wstring& transition) = 0;
};

class AttackStrategy : public UnitStrategy
{
public:
	AttackStrategy() {}
	virtual ~AttackStrategy() {}
public:
	virtual void Enter(const shared_ptr<AIController>& controller) = 0;
	virtual void Update() = 0;
	virtual void Out(const wstring& transition) = 0;
};

class AbilityStrategy : public UnitStrategy
{
public:
	AbilityStrategy() {}
	virtual ~AbilityStrategy() {}
public:
	virtual void Enter(const shared_ptr<AIController>& controller) = 0;
	virtual void Update() = 0;
	virtual void Out(const wstring& transition) = 0;
};

class EventStrategy : public UnitStrategy
{
public:
	EventStrategy() {}
	virtual ~EventStrategy() {}
public:
	virtual void Enter(const shared_ptr<AIController>& controller) = 0;
	virtual void Update() = 0;
	virtual void Out(const wstring& transition) = 0;
};