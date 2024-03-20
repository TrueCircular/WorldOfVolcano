#include "pch.h"
#include "MoltenGaintPattern.h"

#include <stdlib.h>
#include <time.h>
#include "PlayableUnit.h"
#include "CharacterInfo.h"
#include "Sounds.h"
#include "AIController.h"

MoltenGaintStand::MoltenGaintStand()
{
	_name = L"MoltenGaintStand";
}

MoltenGaintStand::~MoltenGaintStand()
{
}

void MoltenGaintStand::Enter(const shared_ptr<AIController>& controller)
{
}

void MoltenGaintStand::Update()
{
}

void MoltenGaintStand::Out(const wstring& transition)
{
}

MoltenGaintDamaged::MoltenGaintDamaged()
{
	_name = L"MoltenGaintDamaged";
}

MoltenGaintDamaged::~MoltenGaintDamaged()
{
}

void MoltenGaintDamaged::Enter(const shared_ptr<AIController>& controller)
{
}

void MoltenGaintDamaged::Update()
{
}

void MoltenGaintDamaged::Out(const wstring& transition)
{
}

MoltenGaintStun::MoltenGaintStun()
{
	_name = L"MoltenGaintStun";
}

MoltenGaintStun::~MoltenGaintStun()
{
}

void MoltenGaintStun::Enter(const shared_ptr<AIController>& controller)
{
}

void MoltenGaintStun::Update()
{
}

void MoltenGaintStun::Out(const wstring& transition)
{
}

MoltenGaintDead::MoltenGaintDead()
{
	_name = L"MoltenGaintDead";
}

MoltenGaintDead::~MoltenGaintDead()
{
}

void MoltenGaintDead::Enter(const shared_ptr<AIController>& controller)
{
}

void MoltenGaintDead::Update()
{
}

void MoltenGaintDead::Out(const wstring& transition)
{
}

MoltenGaintTrace::MoltenGaintTrace()
{
	_name = L"MoltenGaintTrace";
}

MoltenGaintTrace::~MoltenGaintTrace()
{
}

void MoltenGaintTrace::Enter(const shared_ptr<AIController>& controller)
{
}

void MoltenGaintTrace::Update()
{
}

void MoltenGaintTrace::Out(const wstring& transition)
{
}

MoltenGaintMoveToSpwanPoint::MoltenGaintMoveToSpwanPoint()
{
	_name = L"MoltenGaintMoveToSpwanPoint";
}

MoltenGaintMoveToSpwanPoint::~MoltenGaintMoveToSpwanPoint()
{
}

void MoltenGaintMoveToSpwanPoint::Enter(const shared_ptr<AIController>& controller)
{
}

void MoltenGaintMoveToSpwanPoint::Update()
{
}

void MoltenGaintMoveToSpwanPoint::Out(const wstring& transition)
{
}

MoltenGaintBattle::MoltenGaintBattle()
{
	_name = L"MoltenGaintBattle";
}

MoltenGaintBattle::~MoltenGaintBattle()
{
}

void MoltenGaintBattle::Enter(const shared_ptr<AIController>& controller)
{
}

void MoltenGaintBattle::Update()
{
}

void MoltenGaintBattle::Out(const wstring& transition)
{
}

MoltenGaintAttack::MoltenGaintAttack()
{
	_name = L"MoltenGaintAttack";
}

MoltenGaintAttack::~MoltenGaintAttack()
{
}

void MoltenGaintAttack::Enter(const shared_ptr<AIController>& controller)
{
}

void MoltenGaintAttack::Update()
{
}

void MoltenGaintAttack::Out(const wstring& transition)
{
}
