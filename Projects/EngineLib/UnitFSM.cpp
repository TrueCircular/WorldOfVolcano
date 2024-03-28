#include "pch.h"
#include "UnitFSM.h"

void UnitFSM::Enter(const shared_ptr<AIController>& controller, const wstring& preTransition)
{
	_strategy->Enter(controller, preTransition);
}

void UnitFSM::Update()
{
	_strategy->Update();
}

void UnitFSM::UpdateFromServer()
{
	_strategy->UpdateFromServer();
}
