#pragma once
#include "AIController.h"
#include "UnitStrategy.h"

class UnitFSM
{
public:
	UnitFSM() {}
	virtual ~UnitFSM() {}
protected:
	using TarceTargetList = unordered_set<shared_ptr<PlayableUnit>>;
protected:
	shared_ptr<UnitStrategy>	_strategy;
public:
	//Getter
	void SetStrategy(shared_ptr<UnitStrategy> strategy) { _strategy = strategy; }
	//Setter
	const shared_ptr<UnitStrategy>& GetStrategy() const { return _strategy; }
	const wstring& GetStrategyName() const { return _strategy->_name; }
public:
	virtual void Enter(const shared_ptr<AIController>& controller, const wstring& preTransition);
	virtual void Update();
};