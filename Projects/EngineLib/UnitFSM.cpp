#include "pch.h"
#include "UnitFSM.h"
#include "PlayableUnit.h"
#include "CharacterInfo.h"
#include <stdlib.h>
#include <time.h>

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
			_animator.lock()->SetNextAnimation(L"Stand");

			_characterInfo = _controller.lock()->GetCharacterInfo();
			_traceRadius = _characterInfo.lock()->GetDefaultCharacterInfo()._traceRadius;
			_attackRange = _characterInfo.lock()->GetDefaultCharacterInfo()._attackRange;
		}
	}
}

void UnitFSMStand::Update()
{
	//현재 Scene에 존재하는 타깃 대상 캐싱
	_targetList = MANAGER_SCENE()->GetCurrentScene()->GetPlayableUnit();

	if (_controller.lock() != nullptr)
	{
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
					uint16 aggroPow = target.second->GetComponent<CharacterInfo>()->GetDefaultCharacterInfo()._aggroLevel;

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
					if (minDistance <= _attackRange)
					{
						_controller.lock()->SetTargetTransform(FinalTarget->GetTransform());
						Out(UnitFSMState::Battle);
					}
					else if(minDistance <= _traceRadius)
					{
						_controller.lock()->SetTargetTransform(FinalTarget->GetTransform());
						Out(UnitFSMState::Trace);
					}
				}
			}
		}
	}
}

void UnitFSMStand::Out(UnitFSMState transition)
{
	if (_controller.lock() != nullptr)
	{
		switch (transition)
		{
		case UnitFSMState::Dead:
		{
			_controller.lock()->SetCurrentFsmState(UnitFSMState::Dead);
		}break;
		case UnitFSMState::Damaged:
		{
			_controller.lock()->SetCurrentFsmState(UnitFSMState::Damaged);
		}break;
		case UnitFSMState::Trace:
		{
			_controller.lock()->SetCurrentFsmState(UnitFSMState::Trace);
		}break;
		case UnitFSMState::Battle:
		{
			_controller.lock()->SetCurrentFsmState(UnitFSMState::Battle);
		}break;
		}
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
	if (controller != nullptr)
	{
		_controller = controller;

		if (_controller.lock()->GetTransform() != nullptr)
			_transform = _controller.lock()->GetTransform();

		if (_controller.lock()->GetTargetTransform() != nullptr)
			_targetTransform = _controller.lock()->GetTargetTransform();

		if (_controller.lock()->GetAnimator() != nullptr)
			_animator = _controller.lock()->GetAnimator();

		if (_animator.lock() != nullptr)
		{
			_animator.lock()->SetNextAnimation(L"Run");

			_characterInfo = _controller.lock()->GetCharacterInfo();
			_traceRadius = _characterInfo.lock()->GetDefaultCharacterInfo()._traceRadius;
			_attackRange = _characterInfo.lock()->GetDefaultCharacterInfo()._attackRange;
		}
	}
}

void UnitFSMTrace::Update()
{
	if (_controller.lock() != nullptr)
	{
		_dt = MANAGER_TIME()->GetDeltaTime();

		if (_transform.lock() != nullptr && _targetTransform.lock() != nullptr)
		{
			//자신의 위치에서 타겟방향으로 향하는 정규화 된 방향 벡터 계산(Normal Vector)
			Vec3 myPos = _transform.lock()->GetPosition();
			Vec3 targetPos = _targetTransform.lock()->GetPosition();
			Vec3 toTargetDir = targetPos - myPos;
			toTargetDir.Normalize(toTargetDir);

			//타겟 방향으로 회전
			{
				Vec3 myForward = _transform.lock()->GetLookVector();
				Vec3 myRight = _transform.lock()->GetRightVector();
				Vec3 myUp = _transform.lock()->GetUpVector();

				myRight.Normalize(myRight);
				myForward.Normalize(myForward);

				float dotAngle = myForward.Dot(toTargetDir);
				float angle = acosf(dotAngle);

				Vec3 cross = ::XMVector3Cross(myForward, toTargetDir);
				float LeftRight = cross.Dot(myUp);

				if (LeftRight < 0)
				{
					angle = -angle;
				}

				angle = angle * _totargetRotationSpeed * _dt;

				Vec3 myRot = _transform.lock()->GetLocalRotation();
				myRot.y += angle;
				_transform.lock()->SetLocalRotation(myRot);
			}

			//타겟 방향으로 이동 & Attack Range 체크 후 도달 시 Trasition
			{
				Vec3 toTargetTranslate = myPos + (toTargetDir * 30.f * _dt);
				_transform.lock()->SetPosition(toTargetTranslate);

				Vec3 targetPos2 = _targetTransform.lock()->GetPosition();

				float distance = Vec3::Distance(toTargetTranslate, targetPos2);

				if (distance <= _attackRange)
				{
					Out(UnitFSMState::Battle);
				}
			}
		}
	}
}

void UnitFSMTrace::Out(UnitFSMState transition)
{
	if (_controller.lock() != nullptr)
	{
		switch (transition)
		{
		case UnitFSMState::Dead:
		{
			_controller.lock()->SetCurrentFsmState(UnitFSMState::Dead);
		}break;
		case UnitFSMState::MoveToSpwanPoint:
		{
			_controller.lock()->SetCurrentFsmState(UnitFSMState::MoveToSpwanPoint);
		}break;
		case UnitFSMState::Battle:
		{
			_controller.lock()->SetCurrentFsmState(UnitFSMState::Battle);
		}break;
		}
	}
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
	if (controller != nullptr)
	{
		_controller = controller;

		if (_controller.lock()->GetTransform() != nullptr)
			_transform = _controller.lock()->GetTransform();

		if (_controller.lock()->GetTargetTransform() != nullptr)
			_targetTransform = _controller.lock()->GetTargetTransform();

		if (_controller.lock()->GetAnimator() != nullptr)
			_animator = _controller.lock()->GetAnimator();

		if (_animator.lock() != nullptr)
		{
			_animator.lock()->SetNextAnimation(L"Battle");

			_characterInfo = _controller.lock()->GetCharacterInfo();
			_traceRadius = _characterInfo.lock()->GetDefaultCharacterInfo()._traceRadius;
			_attackRange = _characterInfo.lock()->GetDefaultCharacterInfo()._attackRange;
			_attackTime = _characterInfo.lock()->GetDefaultCharacterInfo()._attackTime;
			_attackTimeCal = 0.f;
		}
	}
}

void UnitFSMBattle::Update()
{
	if (_controller.lock() != nullptr)
	{
		_dt = MANAGER_TIME()->GetDeltaTime();

		//타겟 방향으로 회전
		{
			Vec3 myPos = _transform.lock()->GetPosition();
			Vec3 targetPos = _targetTransform.lock()->GetPosition();
			Vec3 toTargetDir = targetPos - myPos;
			toTargetDir.Normalize(toTargetDir);

			Vec3 myForward = _transform.lock()->GetLookVector();
			Vec3 myRight = _transform.lock()->GetRightVector();
			Vec3 myUp = _transform.lock()->GetUpVector();

			myRight.Normalize(myRight);
			myForward.Normalize(myForward);

			float dotAngle = myForward.Dot(toTargetDir);
			float angle = acosf(dotAngle);

			Vec3 cross = ::XMVector3Cross(myForward, toTargetDir);
			float LeftRight = cross.Dot(myUp);

			if (LeftRight < 0)
			{
				angle = -angle;
			}

			angle = angle * _totargetRotationSpeed * _dt;

			Vec3 myRot = _transform.lock()->GetLocalRotation();
			myRot.y += angle;
			_transform.lock()->SetLocalRotation(myRot);
		}

		Vec3 myPos = _transform.lock()->GetPosition();
		Vec3 targetPos = _targetTransform.lock()->GetPosition();
		float distance = Vec3::Distance(myPos, targetPos);

		if (distance <= _attackRange)
		{
			_attackTimeCal += _dt;

			//if 분기 제어 필요


			//Ability Transition
			{

			}

			//Attack Transition
			{
				if (_attackTimeCal >= _attackTime)
					Out(UnitFSMState::Attack);
			}
		}
		else if (distance > _attackRange && distance <= _traceRadius)
		{
			Out(UnitFSMState::Trace);
		}
		else if (distance > _attackRange && distance > _traceRadius)
		{
			Out(UnitFSMState::MoveToSpwanPoint);
		}

	}
}

void UnitFSMBattle::Out(UnitFSMState transition)
{
	switch (transition)
	{
	case UnitFSMState::Stand:
	{
		_controller.lock()->SetCurrentFsmState(UnitFSMState::Stand);
	}break;	
	case UnitFSMState::Damaged:
	{
		_controller.lock()->SetCurrentFsmState(UnitFSMState::Damaged);
	}break;	
	case UnitFSMState::Dead:
	{
		_controller.lock()->SetCurrentFsmState(UnitFSMState::Dead);
	}break;	
	case UnitFSMState::Trace:
	{
		_controller.lock()->SetCurrentFsmState(UnitFSMState::Trace);
	}break;	
	case UnitFSMState::MoveToSpwanPoint:
	{
		_controller.lock()->SetCurrentFsmState(UnitFSMState::MoveToSpwanPoint);
	}break;	
	case UnitFSMState::Attack:
	{
		_controller.lock()->SetCurrentFsmState(UnitFSMState::Attack);
	}break;
	case UnitFSMState::Ability1:
	{
		_controller.lock()->SetCurrentFsmState(UnitFSMState::Ability1);
	}break;
	case UnitFSMState::Ability2:
	{
		_controller.lock()->SetCurrentFsmState(UnitFSMState::Ability2);
	}break;
	case UnitFSMState::Event:
	{
		_controller.lock()->SetCurrentFsmState(UnitFSMState::Event);
	}break;
	}
}

UnitFSMAttack::UnitFSMAttack()
{
}

UnitFSMAttack::~UnitFSMAttack()
{

}

void UnitFSMAttack::Enter(const shared_ptr<AIController>& controller)
{
	if (controller != nullptr)
	{
		::srand(time(NULL));

		_controller = controller;

		if (_controller.lock()->GetTransform() != nullptr)
			_transform = _controller.lock()->GetTransform();

		if (_controller.lock()->GetTargetTransform() != nullptr)
			_targetTransform = _controller.lock()->GetTargetTransform();

		if (_controller.lock()->GetAnimator() != nullptr)
			_animator = _controller.lock()->GetAnimator();

		if (_animator.lock() != nullptr)
		{
			int randAttack = rand() % 2;

			if (randAttack == 0)
			{
				_animator.lock()->SetNextAnimation(L"Attack1");
			}
			else
			{
				_animator.lock()->SetNextAnimation(L"Attack2");
			}

			_characterInfo = _controller.lock()->GetCharacterInfo();
			_traceRadius = _characterInfo.lock()->GetDefaultCharacterInfo()._traceRadius;
			_attackRange = _characterInfo.lock()->GetDefaultCharacterInfo()._attackRange;
			_animator.lock()->SetFrameEnd(false);
		}
	}
}

void UnitFSMAttack::Update()
{
	if (_controller.lock() != nullptr)
	{
		_dt = MANAGER_TIME()->GetDeltaTime();

		//타겟 방향으로 회전
		{
			Vec3 myPos = _transform.lock()->GetPosition();
			Vec3 targetPos = _targetTransform.lock()->GetPosition();
			Vec3 toTargetDir = targetPos - myPos;
			toTargetDir.Normalize(toTargetDir);

			Vec3 myForward = _transform.lock()->GetLookVector();
			Vec3 myRight = _transform.lock()->GetRightVector();
			Vec3 myUp = _transform.lock()->GetUpVector();

			myRight.Normalize(myRight);
			myForward.Normalize(myForward);

			float dotAngle = myForward.Dot(toTargetDir);
			float angle = acosf(dotAngle);

			Vec3 cross = ::XMVector3Cross(myForward, toTargetDir);
			float LeftRight = cross.Dot(myUp);

			if (LeftRight < 0)
			{
				angle = -angle;
			}

			angle = angle * _totargetRotationSpeed * _dt;

			Vec3 myRot = _transform.lock()->GetLocalRotation();
			myRot.y += angle;
			_transform.lock()->SetLocalRotation(myRot);
		}

		if (_animator.lock()->GetFrameEnd() == true)
		{
			Out(UnitFSMState::Battle);
		}
	}
}

void UnitFSMAttack::Out(UnitFSMState transition)
{
	switch (transition)
	{
	case UnitFSMState::Stand:
	{
		_controller.lock()->SetCurrentFsmState(transition);
	}break;
	case UnitFSMState::Damaged:
	{
		_controller.lock()->SetCurrentFsmState(transition);
	}break;
	case UnitFSMState::Dead:
	{
		_controller.lock()->SetCurrentFsmState(transition);
	}break;
	case UnitFSMState::Battle:
	{
		_controller.lock()->SetCurrentFsmState(transition);
	}break;
	}
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