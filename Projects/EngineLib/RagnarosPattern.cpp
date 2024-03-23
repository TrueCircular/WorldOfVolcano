#include "pch.h"
#include "RagnarosPattern.h"

#include <stdlib.h>
#include <time.h>
#include "PlayableUnit.h"
#include "CharacterInfo.h"
#include "Sounds.h"
#include "AIController.h"

RagnarosStand::RagnarosStand()
{
	_name = L"RagnarosStand";
}

RagnarosStand::~RagnarosStand()
{
}

void RagnarosStand::Enter(const shared_ptr<AIController>& controller, const wstring& prevTransition)
{
}

void RagnarosStand::Update()
{
}

void RagnarosStand::Out(const wstring& nextTransition)
{
}

RagnarosStun::RagnarosStun()
{
	_name = L"RagnarosStun";
}

RagnarosStun::~RagnarosStun()
{
}

void RagnarosStun::Enter(const shared_ptr<AIController>& controller, const wstring& prevTransition)
{
}

void RagnarosStun::Update()
{
}

void RagnarosStun::Out(const wstring& nextTransition)
{
}

RagnarosDead::RagnarosDead()
{
	_name = L"RagnarosDead";
}

RagnarosDead::~RagnarosDead()
{
}

void RagnarosDead::Enter(const shared_ptr<AIController>& controller, const wstring& prevTransition)
{
}

void RagnarosDead::Update()
{
}

void RagnarosDead::Out(const wstring& nextTransition)
{
}

RagnarosTrace::RagnarosTrace()
{
	_name = L"RagnarosTrace";
}

RagnarosTrace::~RagnarosTrace()
{
}

void RagnarosTrace::Enter(const shared_ptr<AIController>& controller, const wstring& prevTransition)
{
}

void RagnarosTrace::Update()
{
}

void RagnarosTrace::Out(const wstring& nextTransition)
{
}

RagnarosBattle::RagnarosBattle()
{
	_name = L"RagnarosBattle";
}

RagnarosBattle::~RagnarosBattle()
{
}

void RagnarosBattle::Enter(const shared_ptr<AIController>& controller, const wstring& prevTransition)
{
}

void RagnarosBattle::Update()
{
}

void RagnarosBattle::Out(const wstring& nextTransition)
{
}

RagnarosAttack::RagnarosAttack()
{
	_name = L"RagnarosAttack";
}

RagnarosAttack::~RagnarosAttack()
{
}

void RagnarosAttack::Enter(const shared_ptr<AIController>& controller, const wstring& prevTransition)
{
}

void RagnarosAttack::Update()
{
}

void RagnarosAttack::Out(const wstring& nextTransition)
{
}

RagnarosAbility::RagnarosAbility()
{
	_name = L"RagnarosAbility";
}

RagnarosAbility::~RagnarosAbility()
{
}

void RagnarosAbility::Enter(const shared_ptr<AIController>& controller, const wstring& prevTransition)
{
}

void RagnarosAbility::Update()
{
}

void RagnarosAbility::Out(const wstring& nextTransition)
{
}
