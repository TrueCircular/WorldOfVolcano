#include "pch.h"
#include "BaronGeddonPattern.h"

#include <stdlib.h>
#include <time.h>
#include "PlayableUnit.h"
#include "CharacterInfo.h"
#include "Sounds.h"
#include "AIController.h"

BaronGeddonStand::BaronGeddonStand()
{
	_name = L"BaronGeddonStand";
}

BaronGeddonStand::~BaronGeddonStand()
{
}

void BaronGeddonStand::Enter(const shared_ptr<AIController>& controller)
{
}

void BaronGeddonStand::Update()
{
}

void BaronGeddonStand::Out(const wstring& transition)
{
}

BaronGeddonDamaged::BaronGeddonDamaged()
{
	_name = L"BaronGeddonDamaged";
}

BaronGeddonDamaged::~BaronGeddonDamaged()
{
}

void BaronGeddonDamaged::Enter(const shared_ptr<AIController>& controller)
{
}

void BaronGeddonDamaged::Update()
{
}

void BaronGeddonDamaged::Out(const wstring& transition)
{
}

BaronGeddonStun::BaronGeddonStun()
{
	_name = L"BaronGeddonStun";
}

BaronGeddonStun::~BaronGeddonStun()
{
}

void BaronGeddonStun::Enter(const shared_ptr<AIController>& controller)
{
}

void BaronGeddonStun::Update()
{
}

void BaronGeddonStun::Out(const wstring& transition)
{
}

BaronGeddonDead::BaronGeddonDead()
{
	_name = L"BaronGeddonDead";
}

BaronGeddonDead::~BaronGeddonDead()
{
}

void BaronGeddonDead::Enter(const shared_ptr<AIController>& controller)
{
}

void BaronGeddonDead::Update()
{
}

void BaronGeddonDead::Out(const wstring& transition)
{
}

BaronGeddonTrace::BaronGeddonTrace()
{
	_name = L"BaronGeddonTrace";
}

BaronGeddonTrace::~BaronGeddonTrace()
{
}

void BaronGeddonTrace::Enter(const shared_ptr<AIController>& controller)
{
}

void BaronGeddonTrace::Update()
{
}

void BaronGeddonTrace::Out(const wstring& transition)
{
}

BaronGeddonMoveToSpwanPoint::BaronGeddonMoveToSpwanPoint()
{
	_name = L"BaronGeddonMoveToSpwanPoint";
}

BaronGeddonMoveToSpwanPoint::~BaronGeddonMoveToSpwanPoint()
{
}

void BaronGeddonMoveToSpwanPoint::Enter(const shared_ptr<AIController>& controller)
{
}

void BaronGeddonMoveToSpwanPoint::Update()
{
}

void BaronGeddonMoveToSpwanPoint::Out(const wstring& transition)
{
}

BaronGeddonBattle::BaronGeddonBattle()
{
	_name = L"BaronGeddonBattle";
}

BaronGeddonBattle::~BaronGeddonBattle()
{
}

void BaronGeddonBattle::Enter(const shared_ptr<AIController>& controller)
{
}

void BaronGeddonBattle::Update()
{
}

void BaronGeddonBattle::Out(const wstring& transition)
{
}

BaronGeddonAttack::BaronGeddonAttack()
{
	_name = L"BaronGeddonAttack";
}

BaronGeddonAttack::~BaronGeddonAttack()
{
}

void BaronGeddonAttack::Enter(const shared_ptr<AIController>& controller)
{
}

void BaronGeddonAttack::Update()
{
}

void BaronGeddonAttack::Out(const wstring& transition)
{
}

BaronGeddonAbility::BaronGeddonAbility()
{
	_name = L"BaronGeddonAbility";
}

BaronGeddonAbility::~BaronGeddonAbility()
{
}

void BaronGeddonAbility::Enter(const shared_ptr<AIController>& controller)
{
}

void BaronGeddonAbility::Update()
{
}

void BaronGeddonAbility::Out(const wstring& transition)
{
}
