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

void RagnarosStand::Enter(const shared_ptr<AIController>& controller)
{
}

void RagnarosStand::Update()
{
}

void RagnarosStand::Out(const wstring& transition)
{
}

RagnarosDamaged::RagnarosDamaged()
{
	_name = L"RagnarosDamaged";
}

RagnarosDamaged::~RagnarosDamaged()
{
}

void RagnarosDamaged::Enter(const shared_ptr<AIController>& controller)
{
}

void RagnarosDamaged::Update()
{
}

void RagnarosDamaged::Out(const wstring& transition)
{
}

RagnarosStun::RagnarosStun()
{
	_name = L"RagnarosStun";
}

RagnarosStun::~RagnarosStun()
{
}

void RagnarosStun::Enter(const shared_ptr<AIController>& controller)
{
}

void RagnarosStun::Update()
{
}

void RagnarosStun::Out(const wstring& transition)
{
}

RagnarosDead::RagnarosDead()
{
	_name = L"RagnarosDead";
}

RagnarosDead::~RagnarosDead()
{
}

void RagnarosDead::Enter(const shared_ptr<AIController>& controller)
{
}

void RagnarosDead::Update()
{
}

void RagnarosDead::Out(const wstring& transition)
{
}

RagnarosTrace::RagnarosTrace()
{
	_name = L"RagnarosTrace";
}

RagnarosTrace::~RagnarosTrace()
{
}

void RagnarosTrace::Enter(const shared_ptr<AIController>& controller)
{
}

void RagnarosTrace::Update()
{
}

void RagnarosTrace::Out(const wstring& transition)
{
}

RagnarosMoveToSpwanPoint::RagnarosMoveToSpwanPoint()
{
	_name = L"RagnarosMoveToSpwanPoint";
}

RagnarosMoveToSpwanPoint::~RagnarosMoveToSpwanPoint()
{
}

void RagnarosMoveToSpwanPoint::Enter(const shared_ptr<AIController>& controller)
{
}

void RagnarosMoveToSpwanPoint::Update()
{
}

void RagnarosMoveToSpwanPoint::Out(const wstring& transition)
{
}

RagnarosBattle::RagnarosBattle()
{
	_name = L"RagnarosBattle";
}

RagnarosBattle::~RagnarosBattle()
{
}

void RagnarosBattle::Enter(const shared_ptr<AIController>& controller)
{
}

void RagnarosBattle::Update()
{
}

void RagnarosBattle::Out(const wstring& transition)
{
}

RagnarosAttack::RagnarosAttack()
{
	_name = L"RagnarosAttack";
}

RagnarosAttack::~RagnarosAttack()
{
}

void RagnarosAttack::Enter(const shared_ptr<AIController>& controller)
{
}

void RagnarosAttack::Update()
{
}

void RagnarosAttack::Out(const wstring& transition)
{
}

RagnarosAbility::RagnarosAbility()
{
	_name = L"RagnarosAbility";
}

RagnarosAbility::~RagnarosAbility()
{
}

void RagnarosAbility::Enter(const shared_ptr<AIController>& controller)
{
}

void RagnarosAbility::Update()
{
}

void RagnarosAbility::Out(const wstring& transition)
{
}
