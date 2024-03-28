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

RagnarosDamaged::RagnarosDamaged()
{
	_name = L"RagnarosDamaged";
}

RagnarosDamaged::~RagnarosDamaged()
{
}

void RagnarosDamaged::Enter(const shared_ptr<AIController>& controller, const wstring& prevTransition)
{
	if (controller != nullptr)
	{
		_controller = controller;
		_prevTransition = prevTransition;
	}
}

void RagnarosDamaged::Update()
{
	if (_controller.lock() != nullptr)
	{
		Out(_prevTransition);
	}
}

void RagnarosDamaged::Out(const wstring& nextTransition)
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

		if (_controller.lock()->GetAnimator() != nullptr)
			_animator = _controller.lock()->GetAnimator();

		if (_animator.lock() != nullptr)
		{
			_animator.lock()->SetFrameEnd(false);
			_animator.lock()->SetNextAnimation(L"Stun");
		}

		_stunTimer = 0.f;
	}
}

void RagnarosStun::Update()
{
	_stunTimer += MANAGER_TIME()->GetDeltaTime();

	if (_stunTimer > _stunTime)
	{
		Out(L"RagnarosBattle");
	}
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

	//Death Sound
	auto tempSound1 = MANAGER_RESOURCES()->GetResource<Sounds>(L"Ragnaros_Death");
	if (tempSound1 == nullptr)
	{
		shared_ptr<Sounds> sound = make_shared<Sounds>();
		wstring soundPath = RESOURCES_ADDR_SOUND;
		soundPath += L"Character/Enemy/Ragnaros/Ragnaros_Death.mp3";
		sound->Load(soundPath);
		sound->SetVolume(50);
		MANAGER_RESOURCES()->AddResource<Sounds>(L"Ragnaros_Death", sound);

		_deadSound = sound->Clone();
	}
	else
	{
		_deadSound = tempSound1->Clone();
	}
}

RagnarosDead::~RagnarosDead()
{
}

void RagnarosDead::Enter(const shared_ptr<AIController>& controller, const wstring& prevTransition)
{
	if (controller != nullptr)
	{
		_controller = controller;
		_prevTransition = prevTransition;

		if (_controller.lock()->GetAnimator() != nullptr)
			_animator = _controller.lock()->GetAnimator();

		if (_animator.lock() != nullptr)
		{
			_animator.lock()->SetFrameEnd(false);
			_animator.lock()->SetNextAnimation(L"Death");
		}
		_dt = 0.f;
		_controller.lock()->_isAlive = false;
		_deadSound->Play(false);
	}
}

void RagnarosDead::Update()
{
	if (_controller.lock() != nullptr)
	{
		_dt += MANAGER_TIME()->GetDeltaTime();

		if (_animator.lock()->GetFrameEnd() == true)
		{
			_controller.lock()->DeadEvent();
		}
	}
}

void RagnarosDead::Out(const wstring& nextTransition)
{
	if (_controller.lock() != nullptr)
	{
		_controller.lock()->SetCurrentFsmStrategy(_name, nextTransition);
	}
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
	if (controller != nullptr)
	{
		_controller = controller;
		_prevTransition = prevTransition;

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
		}

		_characterInfo = _controller.lock()->GetCharacterInfo();
		_abilitySlot = _controller.lock()->GetGameObject()->GetComponent<AbilitySlot>();
		_abilityTime = _abilitySlot.lock()->GetAbility(0)->GetAbilityData().AbilityCoolTime;
		_targetList = _controller.lock()->GetTargetList();
		_traceRadius = _characterInfo.lock()->GetDefaultCharacterInfo()._traceRadius;
		_attackRange = _characterInfo.lock()->GetDefaultCharacterInfo()._attackRange;
		_attackTime = _characterInfo.lock()->GetDefaultCharacterInfo()._attackTime;
	}
}

void RagnarosBattle::Update()
{
	if (_controller.lock() != nullptr)
	{
		_dt = MANAGER_TIME()->GetDeltaTime();

		bool& tempisAlive = _targetTransform.lock()->GetGameObject()->GetComponent<CharacterController>()->_isAlive;

		//Target update
		if (_targetList.lock()->size() <= 0 || tempisAlive == false)
		{
			Out(L"RagnarosStand");
		}
		else
		{
			float minAggro = -1.f;

			shared_ptr<Transform> _lastTarget = nullptr;
			for (auto& target : *_targetList.lock())
			{
				if (target->AggroValue > minAggro)
				{
					minAggro = target->AggroValue;
					_lastTarget = target->Target->GetTransform();
				}
			}

			if (_lastTarget != nullptr)
			{
				_targetTransform = _lastTarget;
				_controller.lock()->SetTargetTransform(_targetTransform.lock());
			}
		}

		bool& isAlive = _targetTransform.lock()->GetGameObject()->GetComponent<CharacterController>()->_isAlive;

		if (isAlive)
		{
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

			//Ability Transition
			{
				_abilityTimer += _dt;

				if (_abilityTimer > _abilityTime)
				{
					_abilityTimer = 0.f;
					Out(L"RagnarosAbility");
				}
			}

			toTargetDir.Normalize(toTargetDir);
			float distance = Vec3::Distance(myPos, targetPos);

			if (distance <= _attackRange)
			{
				_attackTimeCal += _dt;

				//Attack Transition
				{
					if (_attackTimeCal >= _attackTime)
					{
						_attackTimeCal = 0.f;
						Out(L"RagnarosAttack");
					}
				}
			}
			else if (distance > _attackRange && distance > _traceRadius)
			{
				Out(L"RagnarosStand");
			}
		}
		else
		{
			Out(L"RagnarosStand");
		}
	}
}

void RagnarosBattle::Out(const wstring& nextTransition)
{
	if (_controller.lock() != nullptr)
	{
		_controller.lock()->SetCurrentFsmStrategy(_name, nextTransition);
	}
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

RagnarosEncounterEvent1::RagnarosEncounterEvent1()
{
	_name = L"RagnarosEncounterEvent1";
}

RagnarosEncounterEvent1::~RagnarosEncounterEvent1()
{
}

void RagnarosEncounterEvent1::Enter(const shared_ptr<AIController>& controller, const wstring& prevTransition)
{
}

void RagnarosEncounterEvent1::Update()
{
}

void RagnarosEncounterEvent1::Out(const wstring& nextTransition)
{
}

