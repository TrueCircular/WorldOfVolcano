#include "pch.h"
#include "CoreHoundPattern.h"

#include <stdlib.h>
#include <time.h>
#include "PlayableUnit.h"
#include "CharacterInfo.h"
#include "Sounds.h"
#include "AIController.h"

CoreHoundStand::CoreHoundStand()
{
	_name = L"CoreHoundStand";
}

CoreHoundStand::~CoreHoundStand()
{
}

void CoreHoundStand::Enter(const shared_ptr<AIController>& controller)
{
}

void CoreHoundStand::Update()
{
}

void CoreHoundStand::Out(const wstring& transition)
{
}

CoreHoundDamaged::CoreHoundDamaged()
{
	_name = L"CoreHoundDamaged";
}

CoreHoundDamaged::~CoreHoundDamaged()
{
}

void CoreHoundDamaged::Enter(const shared_ptr<AIController>& controller)
{
}

void CoreHoundDamaged::Update()
{
}

void CoreHoundDamaged::Out(const wstring& transition)
{
}

CoreHoundStun::CoreHoundStun()
{
	_name = L"CoreHoundStun";
}

CoreHoundStun::~CoreHoundStun()
{
}

void CoreHoundStun::Enter(const shared_ptr<AIController>& controller)
{
}

void CoreHoundStun::Update()
{
}

void CoreHoundStun::Out(const wstring& transition)
{
}

CoreHoundDead::CoreHoundDead()
{
	_name = L"CoreHoundDead";
}

CoreHoundDead::~CoreHoundDead()
{
}

void CoreHoundDead::Enter(const shared_ptr<AIController>& controller)
{
}

void CoreHoundDead::Update()
{
}

void CoreHoundDead::Out(const wstring& transition)
{
}

CoreHoundTrace::CoreHoundTrace()
{
	_name = L"CoreHoundTrace";
}

CoreHoundTrace::~CoreHoundTrace()
{
}

void CoreHoundTrace::Enter(const shared_ptr<AIController>& controller)
{
}

void CoreHoundTrace::Update()
{
}

void CoreHoundTrace::Out(const wstring& transition)
{
}

CoreHoundMoveToSpwanPoint::CoreHoundMoveToSpwanPoint()
{
	_name = L"CoreHoundMoveToSpwanPoint";
}

CoreHoundMoveToSpwanPoint::~CoreHoundMoveToSpwanPoint()
{
}

void CoreHoundMoveToSpwanPoint::Enter(const shared_ptr<AIController>& controller)
{
}

void CoreHoundMoveToSpwanPoint::Update()
{
}

void CoreHoundMoveToSpwanPoint::Out(const wstring& transition)
{
}

CoreHoundBattle::CoreHoundBattle()
{
	_name = L"CoreHoundBattle";
}

CoreHoundBattle::~CoreHoundBattle()
{
}

void CoreHoundBattle::Enter(const shared_ptr<AIController>& controller)
{
}

void CoreHoundBattle::Update()
{
}

void CoreHoundBattle::Out(const wstring& transition)
{
}

CoreHoundAttack::CoreHoundAttack()
{
	_name = L"CoreHoundAttack";
}

CoreHoundAttack::~CoreHoundAttack()
{
}

void CoreHoundAttack::Enter(const shared_ptr<AIController>& controller)
{
}

void CoreHoundAttack::Update()
{
}

void CoreHoundAttack::Out(const wstring& transition)
{
}
