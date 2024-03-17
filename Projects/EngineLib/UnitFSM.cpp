#include "pch.h"
#include "UnitFSM.h"
#include "PlayableUnit.h"
#include "CharacterInfo.h"


UnitFSMStand::UnitFSMStand()
{
}

UnitFSMStand::~UnitFSMStand()
{
}

void UnitFSMStand::Enter(const shared_ptr<AIController>& controller)
{
	if (controller != nullptr)
	{
		_controller = controller;

		if (_controller.lock()->GetTransform() != nullptr)
			_transform = _controller.lock()->GetTransform();

		if (_controller.lock()->GetAnimator() != nullptr)
			_animator = _controller.lock()->GetAnimator();

		if (_animator.lock() != nullptr)
		{
			if (_animator.lock()->GetCurrentAnimation() != nullptr)
			{
				_animator.lock()->GetTweenDesc()->ClearNextAnim();
				_animator.lock()->SetNextAnimation(L"Stand");
			}
			else
			{
				_animator.lock()->GetTweenDesc()->ClearCurrentAnim();
				_animator.lock()->SetCurrentAnimation(L"Stand");
			}

			_traceRadius = _controller.lock()->GetTraceRadius();
			_characterInfo = _controller.lock()->GetCharacterInfo();
		}
	}
}

void UnitFSMStand::Update()
{
	//현재 Scene에 존재하는 타깃 대상 캐싱
	_targetList = MANAGER_SCENE()->GetCurrentScene()->GetPlayableUnit();

	if (_targetList.size() > 0)
	{
		//Taget 후보 결정
		map<float, shared_ptr<PlayableUnit>> ToTargetList;

		for (const auto& target : _targetList)
		{
			Vec3 myPos = _transform.lock()->GetPosition();
			Vec3 targetPos = target->GetTransform()->GetPosition();
			float Length = Vec3::Distance(myPos, targetPos);

			//자신의 위치와 타겟 위치가 추적거리 안에 존재 할 경우 탐색
			if (Length <= _traceRadius)
			{
				ToTargetList.insert(make_pair(Length, target));
			}
		}

		if (ToTargetList.size() > 0)
		{
			uint16 maxAggroPow = 0;
			float minDistance = 0.f;
			shared_ptr<PlayableUnit> FinalTarget;

			//최종 타깃 계산
			for (const auto& target : ToTargetList)
			{
				uint16 aggroPow = target.second->GetComponent<CharacterInfo>()->GetCharacterInfo()._aggroLevel;

				if (aggroPow > maxAggroPow)
				{
					maxAggroPow = aggroPow;
					minDistance = target.first;
					FinalTarget = target.second;
				}
				else if (aggroPow == maxAggroPow && target.first < minDistance)
				{
					minDistance = target.first;
					FinalTarget = target.second;
				}
			}

			if (FinalTarget != nullptr)
			{
				if (minDistance <= _characterInfo.lock()->GetCharacterInfo()._attackRange)
				{
					_controller.lock()->SetTargetTransform(FinalTarget->GetTransform());
					Out(UnitFSMState::Attack);
				}
				else
				{
					_controller.lock()->SetTargetTransform(FinalTarget->GetTransform());
					Out(UnitFSMState::Trace);
				}
			}
		}
	}
}

void UnitFSMStand::Out(UnitFSMState transition)
{
	switch (transition)
	{
	case UnitFSMState::Trace:
	{
		_controller.lock()->SetCurrentFsmState(UnitFSMState::Trace);
	}break;
	case UnitFSMState::Battle:
	{

	}break;
	}
}

UnitFSMTrace::UnitFSMTrace()
{

}

UnitFSMTrace::~UnitFSMTrace()
{

}

void UnitFSMTrace::Enter(const shared_ptr<AIController>& controller)
{

}

void UnitFSMTrace::Update()
{

}

void UnitFSMTrace::Out(UnitFSMState transition)
{

}

UnitFSMMoveToSpwanPoint::UnitFSMMoveToSpwanPoint()
{
}

UnitFSMMoveToSpwanPoint::~UnitFSMMoveToSpwanPoint()
{
}

void UnitFSMMoveToSpwanPoint::Enter(const shared_ptr<AIController>& controller)
{
}

void UnitFSMMoveToSpwanPoint::Update()
{
}

void UnitFSMMoveToSpwanPoint::Out(UnitFSMState transition)
{
}

UnitFSMBattle::UnitFSMBattle()
{
}

UnitFSMBattle::~UnitFSMBattle()
{
}

void UnitFSMBattle::Enter(const shared_ptr<AIController>& controller)
{

}

void UnitFSMBattle::Update()
{

}

void UnitFSMBattle::Out(UnitFSMState transition)
{

}

UnitFSMAttack::UnitFSMAttack()
{

}

UnitFSMAttack::~UnitFSMAttack()
{

}

void UnitFSMAttack::Enter(const shared_ptr<AIController>& controller)
{

}

void UnitFSMAttack::Update()
{

}

void UnitFSMAttack::Out(UnitFSMState transition)
{

}

UnitFSMAbility1::UnitFSMAbility1()
{
}

UnitFSMAbility1::~UnitFSMAbility1()
{
}

void UnitFSMAbility1::Enter(const shared_ptr<AIController>& controller)
{

}

void UnitFSMAbility1::Update()
{

}

void UnitFSMAbility1::Out(UnitFSMState transition)
{

}

UnitFSMAbility2::UnitFSMAbility2()
{

}

UnitFSMAbility2::~UnitFSMAbility2()
{

}

void UnitFSMAbility2::Enter(const shared_ptr<AIController>& controller)
{

}

void UnitFSMAbility2::Update()
{

}

void UnitFSMAbility2::Out(UnitFSMState transition)
{

}