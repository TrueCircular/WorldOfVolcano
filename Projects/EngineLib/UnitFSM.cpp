#include "pch.h"
#include "UnitFSM.h"
#include <stdlib.h>
#include <time.h>
#include "PlayableUnit.h"
#include "CharacterInfo.h"
#include "Sounds.h"

UnitFSMStand::UnitFSMStand()
{
}

UnitFSMStand::~UnitFSMStand()
{
}

void UnitFSMStand::SearchTraceTarget()
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
			_animator.lock()->SetFrameEnd(false);
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
				Vec3 myPos = _transform.lock()->GetLocalPosition();
				Vec3 targetPos = target->GetTransform()->GetLocalPosition();
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
					else if (minDistance <= _traceRadius && minDistance > _attackRange)
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
		_controller.lock()->SetCurrentFsmState(transition);
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
			_animator.lock()->SetFrameEnd(false);
			_animator.lock()->SetNextAnimation(L"Run");

			_characterInfo = _controller.lock()->GetCharacterInfo();
			_traceRadius = _characterInfo.lock()->GetDefaultCharacterInfo()._traceRadius;
			_attackRange = _characterInfo.lock()->GetDefaultCharacterInfo()._attackRange;
			_moveSpeed = _characterInfo.lock()->GetDefaultCharacterInfo()._moveSpeed;
		}
	}
}

void UnitFSMTrace::Update()
{
	if (_controller.lock() != nullptr)
	{
		_dt = MANAGER_TIME()->GetDeltaTime();

		//자신의 위치에서 타겟방향으로 향하는 정규화 된 방향 벡터 계산(Normal Vector)
		Vec3 myPos = _transform.lock()->GetLocalPosition();
		Vec3 targetPos = _targetTransform.lock()->GetPosition();
		targetPos.y = myPos.y;
		Vec3 toTargetDir = targetPos - myPos;

		//타겟 방향으로 회전
		{
			if (toTargetDir.Length() > 0)
			{
				toTargetDir.Normalize(toTargetDir);
				{
					Vec3 myForward = _transform.lock()->GetLookVector();
					Vec3 myRight = _transform.lock()->GetRightVector();
					Vec3 myUp = Vec3(0,1,0);

					float dotAngle = max(-1.0f, min(1.0f, myForward.Dot(toTargetDir)));
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
			}
		}

		toTargetDir.Normalize(toTargetDir);

		//타겟 방향으로 이동 & Attack Range 체크 후 도달 시 Trasition
		{
			Vec3 toTargetTranslate = myPos + (toTargetDir * _moveSpeed * _dt);
			_transform.lock()->SetPosition(toTargetTranslate);

			Vec3 targetPos2 = _targetTransform.lock()->GetLocalPosition();

			float distance = Vec3::Distance(toTargetTranslate, targetPos2);

			if (distance <= _attackRange && distance <= _traceRadius)
			{
				Out(UnitFSMState::Battle);
			}
			else if (distance > _attackRange && distance > _traceRadius)
			{
				Out(UnitFSMState::MoveToSpwanPoint);
			}
		}
	}
}

void UnitFSMTrace::Out(UnitFSMState transition)
{
	if (_controller.lock() != nullptr)
	{
		_controller.lock()->SetCurrentFsmState(transition);
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
	if (controller != nullptr)
	{
		_controller = controller;

		if (_controller.lock()->GetTransform() != nullptr)
			_transform = _controller.lock()->GetTransform();

		if (_controller.lock()->GetAnimator() != nullptr)
			_animator = _controller.lock()->GetAnimator();

		if (_animator.lock() != nullptr)
		{
			_animator.lock()->SetFrameEnd(false);
			_animator.lock()->SetNextAnimation(L"Run");

			_characterInfo = _controller.lock()->GetCharacterInfo();
			_spwanPos = _controller.lock()->GetSpwanPosition();
			_moveSpeed = _characterInfo.lock()->GetDefaultCharacterInfo()._moveSpeed;
		}
	}
}

void UnitFSMMoveToSpwanPoint::SearchTraceTarget()
{
}

void UnitFSMMoveToSpwanPoint::Update()
{
	if (_controller.lock() != nullptr)
	{
		_dt = MANAGER_TIME()->GetDeltaTime();

		Vec3 myPos = _transform.lock()->GetLocalPosition();
		_spwanPos.y = myPos.y;
		Vec3 toTargetDir = _spwanPos - myPos;

		//타겟 방향으로 회전
		{
			if (toTargetDir.Length() > 0)
			{
				toTargetDir.Normalize(toTargetDir);
				{
					Vec3 myForward = _transform.lock()->GetLookVector();
					Vec3 myRight = _transform.lock()->GetRightVector();
					Vec3 myUp = Vec3(0, 1, 0);

					float dotAngle = max(-1.0f, min(1.0f, myForward.Dot(toTargetDir)));
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
			}
		}

		toTargetDir.Normalize(toTargetDir);

		float moveToLength = Vec3::Distance(myPos, _spwanPos);

		if (moveToLength >= 1.f)
		{
			Vec3 toSpwanPosTranslate = myPos + (toTargetDir * _moveSpeed * _dt);
			_transform.lock()->SetLocalPosition(toSpwanPosTranslate);
		}
		else
		{
			_transform.lock()->SetLocalPosition(_spwanPos);
			Out(UnitFSMState::Stand);
		}
	}

}

void UnitFSMMoveToSpwanPoint::Out(UnitFSMState transition)
{
	if (_controller.lock() != nullptr)
	{
		_controller.lock()->SetCurrentFsmState(transition);
	}
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
			_animator.lock()->SetFrameEnd(false);
			_animator.lock()->SetNextAnimation(L"Battle");

			_characterInfo = _controller.lock()->GetCharacterInfo();
			_traceRadius = _characterInfo.lock()->GetDefaultCharacterInfo()._traceRadius;
			_attackRange = _characterInfo.lock()->GetDefaultCharacterInfo()._attackRange;
			_attackTime = _characterInfo.lock()->GetDefaultCharacterInfo()._attackTime;
			_attackTimeCal = 0.f;
			_traceTime = 0.f;
		}
	}
}

void UnitFSMBattle::Update()
{
	if (_controller.lock() != nullptr)
	{
		_dt = MANAGER_TIME()->GetDeltaTime();
		_traceTime += _dt;

		Vec3 myPos = _transform.lock()->GetLocalPosition();
		Vec3 targetPos = _targetTransform.lock()->GetLocalPosition();
		targetPos.y = myPos.y;
		Vec3 toTargetDir = targetPos - myPos;

		//타겟 방향으로 회전
		{
			if (toTargetDir.Length() > 0)
			{
				toTargetDir.Normalize(toTargetDir);
				{
					Vec3 myForward = _transform.lock()->GetLookVector();
					Vec3 myRight = _transform.lock()->GetRightVector();
					Vec3 myUp = Vec3(0, 1, 0);

					myForward.Normalize();

					float dotAngle = max(-1.0f, min(1.0f, myForward.Dot(toTargetDir)));
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
			}
		}

		toTargetDir.Normalize(toTargetDir);
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
				{
					Out(UnitFSMState::Attack);
				}
			}
		}
		else if (distance > _attackRange && distance <= _traceRadius)
		{
			if (_traceTime + FLT_EPSILON > _traceWaitingTime)
			{
				Out(UnitFSMState::Trace);
			}
		}
		else if (distance > _attackRange && distance > _traceRadius)
		{
			Out(UnitFSMState::MoveToSpwanPoint);
		}

	}
}

void UnitFSMBattle::Out(UnitFSMState transition)
{
	if (_controller.lock() != nullptr)
	{
		_controller.lock()->SetCurrentFsmState(transition);
	}
}

UnitFSMAttack::UnitFSMAttack()
{
	//Attack1 Sound
	auto tempSound1 = MANAGER_RESOURCES()->GetResource<Sounds>(L"BaronGeddon_Attack1"); 
	if (tempSound1 == nullptr)
	{
		shared_ptr<Sounds> sound = make_shared<Sounds>();
		wstring soundPath = RESOURCES_ADDR_SOUND;
		soundPath += L"Character/Enemy/BaronGeddon/BaronGeddon_Attack1.mp3";
		sound->Load(soundPath);
		sound->SetVolume(500);
		MANAGER_RESOURCES()->AddResource<Sounds>(L"BaronGeddon_Attack1", sound);

		_attack1Sound = sound->Clone();
	}
	else
	{
		_attack1Sound = tempSound1->Clone();
		_attack1Sound->SetVolume(500);
	}

	//Attack2 Sound
	auto tmepSound2 = MANAGER_RESOURCES()->GetResource<Sounds>(L"BaronGeddon_Attack2");
	if (tmepSound2 == nullptr)
	{
		shared_ptr<Sounds> sound = make_shared<Sounds>();
		wstring soundPath = RESOURCES_ADDR_SOUND;
		soundPath += L"Character/Enemy/BaronGeddon/BaronGeddon_Attack2.mp3";
		sound->Load(soundPath);
		MANAGER_RESOURCES()->AddResource<Sounds>(L"BaronGeddon_Attack2", sound);

		_attack2Sound = sound->Clone();
	}
	else
	{
		_attack2Sound = tmepSound2->Clone();
	}
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
			_animator.lock()->SetFrameEnd(false);

			int randAttack = rand() % 2;

			if (randAttack == 0)
			{
				_animator.lock()->SetNextAnimation(L"Attack1");
				_attack1Sound->Play(false);
			}
			else
			{
				_animator.lock()->SetNextAnimation(L"Attack2");
				_attack2Sound->Play(false);
			}

			_characterInfo = _controller.lock()->GetCharacterInfo();
			_traceRadius = _characterInfo.lock()->GetDefaultCharacterInfo()._traceRadius;
			_attackRange = _characterInfo.lock()->GetDefaultCharacterInfo()._attackRange;
		}
	}
}

void UnitFSMAttack::Update()
{
	if (_controller.lock() != nullptr)
	{
		if (_animator.lock()->GetFrameEnd() == true)
		{
			Out(UnitFSMState::Battle);
		}

		_dt = MANAGER_TIME()->GetDeltaTime();

		Vec3 myPos = _transform.lock()->GetLocalPosition();
		Vec3 targetPos = _targetTransform.lock()->GetLocalPosition();
		targetPos.y = myPos.y;
		Vec3 toTargetDir = targetPos - myPos;

		//타겟 방향으로 회전
		{
			if (toTargetDir.Length() > 0)
			{
				toTargetDir.Normalize(toTargetDir);
				{
					Vec3 myForward = _transform.lock()->GetLookVector();
					Vec3 myRight = _transform.lock()->GetRightVector();
					Vec3 myUp = Vec3(0, 1, 0);

					myForward.Normalize();

					float dotAngle = max(-1.0f, min(1.0f, myForward.Dot(toTargetDir)));
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
			}
		}
	}
}

void UnitFSMAttack::Out(UnitFSMState transition)
{
	if (_controller.lock() != nullptr)
	{
		_controller.lock()->SetCurrentFsmState(transition);
	}
}

UnitFSMAbility::UnitFSMAbility()
{
}

UnitFSMAbility::~UnitFSMAbility()
{
}

void UnitFSMAbility::Enter(const shared_ptr<AIController>& controller)
{

}

void UnitFSMAbility::Update()
{

}

void UnitFSMAbility::Out(UnitFSMState transition)
{
}
