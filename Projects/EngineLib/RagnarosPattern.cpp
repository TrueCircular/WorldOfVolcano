#include "pch.h"
#include "RagnarosPattern.h"

#include "PlayerController.h"
#include "AIController.h"
#include "CharacterInfo.h"
#include "Sounds.h"
#include "AbilitySlot.h"

RagnarosStand::RagnarosStand()
{
	_name = L"RagnarosStand";
}

RagnarosStand::~RagnarosStand()
{
}

void RagnarosStand::Enter(const shared_ptr<AIController>& controller, const wstring& prevTransition)
{
	if (controller != nullptr)
	{
		_controller = controller;
		_prevTransition = prevTransition;

		if (_controller.lock()->GetTransform() != nullptr)
			_transform = _controller.lock()->GetTransform();

		if (_controller.lock()->GetAnimator() != nullptr)
			_animator = _controller.lock()->GetAnimator();

		if (_animator.lock() != nullptr)
		{
			_animator.lock()->SetFrameEnd(false);
			_animator.lock()->SetNextAnimation(L"Stand");
		}
	}
}

void RagnarosStand::Update()
{
}

void RagnarosStand::Out(const wstring& nextTransition)
{
	if (_controller.lock() != nullptr)
	{
		_controller.lock()->SetCurrentFsmStrategy(_name, nextTransition);
	}
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
	if (controller != nullptr)
	{
		_controller = controller;
		_prevTransition = prevTransition;

		if (_controller.lock()->GetTransform() != nullptr)
			_transform = _controller.lock()->GetTransform();

		if (_controller.lock()->GetAnimator() != nullptr)
			_animator = _controller.lock()->GetAnimator();

		if (_animator.lock() != nullptr)
		{
			_animator.lock()->SetFrameEnd(false);
			_animator.lock()->SetNextAnimation(L"Stun");
		}
	}
}

void RagnarosStun::Update()
{
}

void RagnarosStun::Out(const wstring& nextTransition)
{
	if (_controller.lock() != nullptr)
	{
		_controller.lock()->SetCurrentFsmStrategy(_name, nextTransition);
	}
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

RagnarosEncounterEvent::RagnarosEncounterEvent()
{
	_name = L"RagnarosEncounterEvent";
}

RagnarosEncounterEvent::~RagnarosEncounterEvent()
{
}

void RagnarosEncounterEvent::Enter(const shared_ptr<AIController>& controller, const wstring& prevTransition)
{
}

void RagnarosEncounterEvent::Update()
{
}

void RagnarosEncounterEvent::Out(const wstring& nextTransition)
{
}
