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

void RagnarosStand::UpdateFromServer()
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

void RagnarosDamaged::UpdateFromServer()
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

void RagnarosStun::UpdateFromServer()
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

void RagnarosDead::UpdateFromServer()
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
		if (_abilitySlot.lock() != nullptr)
		{
			if (_abilitySlot.lock()->GetAbility(0))
			{
				_abilityTime = _abilitySlot.lock()->GetAbility(0)->GetAbilityData().AbilityCoolTime;
			}

			if (_abilitySlot.lock()->GetAbility(1))
			{
				_abilityTime2 = _abilitySlot.lock()->GetAbility(1)->GetAbilityData().AbilityCoolTime;
			}
		}
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

		//Target update
		if (_targetList.lock()->size() <= 0)
		{
			Out(L"RagnarosStand");
		}
		else
		{
			if (_targetList.lock()->size() > 0)
			{
				float minAggro = 0.f;
				shared_ptr<Transform> _lastTarget;
				for (auto& target : *_targetList.lock())
				{
					if (_targetTransform.lock())
					{
						if (target->Target == _targetTransform.lock()->GetGameObject())
						{
							minAggro = target->AggroValue;
							_lastTarget = target->Target->GetTransform();
							continue;
						}
					}

					if (target->AggroValue > minAggro)
					{
						minAggro = target->AggroValue;
						_lastTarget = target->Target->GetTransform();
					}
				}

				if (_lastTarget)
				{
					_targetTransform = _lastTarget;
					_controller.lock()->SetTargetTransform(_targetTransform.lock());
				}
			}
		}

		if (_targetTransform.lock())
		{
			bool& isAlive = _targetTransform.lock()->GetGameObject()->GetComponent<CharacterController>()->_isAlive;

			if (isAlive)
			{
				Vec3 myPos = _transform.lock()->GetLocalPosition();
				Vec3 targetPos = _targetTransform.lock()->GetLocalPosition();
				targetPos.y = myPos.y;
				Vec3 toTargetDir = targetPos - myPos;

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
					_abilityTimer2 += _dt;

					if (_abilityTimer2 > _abilityTime2)
					{
						_abilityTimer2 = 0.f;
						Out(L"RagnarosAbility2");
						return;
					}
					else if (_abilityTimer > _abilityTime)
					{
						_abilityTimer = 0.f;
						Out(L"RagnarosAbility1");
						return;
					}
				}

				toTargetDir.Normalize(toTargetDir);
				float distance = Vec3::Distance(myPos, targetPos);

				if (distance <= _attackRange)
				{
					_attackTimeCal += _dt;

					//Attack Transition
					if (_attackTimeCal >= _attackTime)
					{
						_attackTimeCal = 0.f;
						Out(L"RagnarosAttack");
					}
				}
			}
			else
			{
				Out(L"RagnarosStand");
			}
		}
	}
}

void RagnarosBattle::UpdateFromServer()
{
	if (_controller.lock() != nullptr)
	{
		_dt = MANAGER_TIME()->GetDeltaTime();

		//Target update
		if (_targetList.lock()->size() <= 0)
		{
			Out(L"RagnarosStand");
		}
		else
		{
			if (_targetList.lock()->size() > 0)
			{
				float minAggro = 0.f;
				shared_ptr<Transform> _lastTarget;
				for (auto& target : *_targetList.lock())
				{
					if (_targetTransform.lock())
					{
						if (target->Target == _targetTransform.lock()->GetGameObject())
						{
							minAggro = target->AggroValue;
							_lastTarget = target->Target->GetTransform();
							continue;
						}
					}

					if (target->AggroValue > minAggro)
					{
						minAggro = target->AggroValue;
						_lastTarget = target->Target->GetTransform();
					}
				}

				if (_lastTarget)
				{
					_targetTransform = _lastTarget;
					_controller.lock()->SetTargetTransform(_targetTransform.lock());
				}
			}
		}

		if (_targetTransform.lock())
		{
			bool& isAlive = _targetTransform.lock()->GetGameObject()->GetComponent<CharacterController>()->_isAlive;

			if (isAlive)
			{
				Vec3 myPos = _transform.lock()->GetLocalPosition();
				Vec3 targetPos = _targetTransform.lock()->GetLocalPosition();
				targetPos.y = myPos.y;
				Vec3 toTargetDir = targetPos - myPos;

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
					_abilityTimer2 += _dt;

					if (_abilityTimer2 > _abilityTime2)
					{
						_abilityTimer2 = 0.f;
						Out(L"RagnarosAbility2");
						return;
					}
					else if (_abilityTimer > _abilityTime)
					{
						_abilityTimer = 0.f;
						Out(L"RagnarosAbility1");
						return;
					}
				}

				toTargetDir.Normalize(toTargetDir);
				float distance = Vec3::Distance(myPos, targetPos);

				if (distance <= _attackRange)
				{
					_attackTimeCal += _dt;

					//Attack Transition
					if (_attackTimeCal >= _attackTime)
					{
						_attackTimeCal = 0.f;
						Out(L"RagnarosAttack");
					}
				}
			}
			else
			{
				Out(L"RagnarosStand");
			}
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

	//Attack1 Sound
	auto tempSound1 = MANAGER_RESOURCES()->GetResource<Sounds>(L"Ragnaros_Attack1");
	if (tempSound1 == nullptr)
	{
		shared_ptr<Sounds> sound = make_shared<Sounds>();
		wstring soundPath = RESOURCES_ADDR_SOUND;
		soundPath += L"Character/Enemy/Ragnaros/Ragnaros_Attack1.mp3";
		sound->Load(soundPath);
		sound->SetVolume(50);
		MANAGER_RESOURCES()->AddResource<Sounds>(L"Ragnaros_Attack1", sound);

		_attack1Sound = sound->Clone();
	}
	else
	{
		_attack1Sound = tempSound1->Clone();
	}

	//Attack2 Sound
	auto tmepSound2 = MANAGER_RESOURCES()->GetResource<Sounds>(L"Ragnaros_Attack2");
	if (tmepSound2 == nullptr)
	{
		shared_ptr<Sounds> sound = make_shared<Sounds>();
		wstring soundPath = RESOURCES_ADDR_SOUND;
		soundPath += L"Character/Enemy/Ragnaros/Ragnaros_Attack2.mp3";
		sound->Load(soundPath);
		sound->SetVolume(50);
		MANAGER_RESOURCES()->AddResource<Sounds>(L"Ragnaros_Attack2", sound);

		_attack2Sound = sound->Clone();
	}
	else
	{
		_attack2Sound = tmepSound2->Clone();
	}
}

RagnarosAttack::~RagnarosAttack()
{
}

void RagnarosAttack::Enter(const shared_ptr<AIController>& controller, const wstring& prevTransition)
{
	if (controller != nullptr)
	{
		::srand(time(NULL));

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
			_characterInfo = _controller.lock()->GetCharacterInfo();
			_traceRadius = _characterInfo.lock()->GetDefaultCharacterInfo()._traceRadius;
			_attackRange = _characterInfo.lock()->GetDefaultCharacterInfo()._attackRange;

			if (_targetTransform.lock())
			{
				auto targetCon = _targetTransform.lock()->GetGameObject()->GetComponent<CharacterController>();

				if (targetCon != nullptr)
				{
					float attackDamage = _characterInfo.lock()->GetCharacterInfo()._atk;
					targetCon->TakeDamage(_transform.lock()->GetGameObject(), attackDamage);
				}
			}

			_randAttack = rand() % 2;

			if (_randAttack == 0)
			{
				_animator.lock()->SetFrameEnd(false);
				_animator.lock()->SetNextAnimation(L"Attack1");
				_attack1Sound->Play(false);
			}
			else
			{
				_animator.lock()->SetFrameEnd(false);
				_animator.lock()->SetNextAnimation(L"Attack2");
				_attack2Sound->Play(false);
			}
		}
	}
}

void RagnarosAttack::Update()
{
	if (_controller.lock() != nullptr)
	{
		_dt = MANAGER_TIME()->GetDeltaTime();

		if (_animator.lock()->GetFrameEnd() == true)
		{
			Out(L"RagnarosBattle");
		}

		if (_targetTransform.lock())
		{
			if (_targetTransform.lock()->GetGameObject()->GetComponent<CharacterController>()->_isAlive)
			{
				Vec3 myPos = _transform.lock()->GetLocalPosition();
				Vec3 targetPos = _targetTransform.lock()->GetLocalPosition();
				targetPos.y = myPos.y;
				Vec3 toTargetDir = targetPos - myPos;

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
	}
}

void RagnarosAttack::UpdateFromServer()
{
	if (_controller.lock() != nullptr)
	{
		_dt = MANAGER_TIME()->GetDeltaTime();

		if (_animator.lock()->GetFrameEnd() == true)
		{
			Out(L"RagnarosBattle");
		}

		if (_targetTransform.lock())
		{
			if (_targetTransform.lock()->GetGameObject()->GetComponent<CharacterController>()->_isAlive)
			{
				Vec3 myPos = _transform.lock()->GetLocalPosition();
				Vec3 targetPos = _targetTransform.lock()->GetLocalPosition();
				targetPos.y = myPos.y;
				Vec3 toTargetDir = targetPos - myPos;

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
	}
}

void RagnarosAttack::Out(const wstring& nextTransition)
{
	if (_controller.lock() != nullptr)
	{
		_controller.lock()->SetCurrentFsmStrategy(_name, nextTransition);
	}
}

RagnarosAbility1::RagnarosAbility1()
{
	_name = L"RagnarosAbility1";

	//Ability Sound
	auto tmepSound = MANAGER_RESOURCES()->GetResource<Sounds>(L"Ragnaros_Ability1");
	if (tmepSound == nullptr)
	{
		shared_ptr<Sounds> sound = make_shared<Sounds>();
		wstring soundPath = RESOURCES_ADDR_SOUND;
		soundPath += L"Character/Enemy/Ragnaros/Ragnaros_Ability1.mp3";
		sound->Load(soundPath);
		sound->SetVolume(50);
		MANAGER_RESOURCES()->AddResource<Sounds>(L"Ragnaros_Ability1", sound);

		_abiltySound = sound->Clone();
	}
	else
	{
		_abiltySound = tmepSound->Clone();
	}
}

RagnarosAbility1::~RagnarosAbility1()
{
}

void RagnarosAbility1::Enter(const shared_ptr<AIController>& controller, const wstring& prevTransition)
{
	if (controller != nullptr)
	{
		::srand(time(NULL));

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
			_animator.lock()->SetNextAnimation(L"Ability");
		}
		_characterInfo = _controller.lock()->GetCharacterInfo();
		_traceRadius = _characterInfo.lock()->GetDefaultCharacterInfo()._traceRadius;
		_attackRange = _characterInfo.lock()->GetDefaultCharacterInfo()._attackRange;
		_abilitySlot = _controller.lock()->GetGameObject()->GetComponent<AbilitySlot>();
		_abiltySound->Play(false);
	}
}

void RagnarosAbility1::Update()
{
	if (_controller.lock() != nullptr)
	{
		_dt = MANAGER_TIME()->GetDeltaTime();

		if (_animator.lock()->GetFrameEnd() == true)
		{
			if (_targetTransform.lock())
			{
				_abilitySlot.lock()->ExecuteAbility(0, _targetTransform.lock()->GetGameObject());
			}
			Out(L"RagnarosBattle");
		}

		if (_targetTransform.lock())
		{
			if (_targetTransform.lock()->GetGameObject()->GetComponent<CharacterController>()->_isAlive)
			{
				Vec3 myPos = _transform.lock()->GetLocalPosition();
				Vec3 targetPos = _targetTransform.lock()->GetLocalPosition();
				targetPos.y = myPos.y;
				Vec3 toTargetDir = targetPos - myPos;

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
	}
}

void RagnarosAbility1::UpdateFromServer()
{
	if (_controller.lock() != nullptr)
	{
		_dt = MANAGER_TIME()->GetDeltaTime();

		if (_animator.lock()->GetFrameEnd() == true)
		{
			if (_targetTransform.lock())
			{
				_abilitySlot.lock()->ExecuteAbility(0, _targetTransform.lock()->GetGameObject());
			}
			Out(L"RagnarosBattle");
		}

		if (_targetTransform.lock())
		{
			if (_targetTransform.lock()->GetGameObject()->GetComponent<CharacterController>()->_isAlive)
			{
				Vec3 myPos = _transform.lock()->GetLocalPosition();
				Vec3 targetPos = _targetTransform.lock()->GetLocalPosition();
				targetPos.y = myPos.y;
				Vec3 toTargetDir = targetPos - myPos;

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
	}
}

void RagnarosAbility1::Out(const wstring& nextTransition)
{
	if (_controller.lock() != nullptr)
	{
		_controller.lock()->SetCurrentFsmStrategy(_name, nextTransition);
	}
}

RagnarosAbility2::RagnarosAbility2()
{
	_name = L"RagnarosAbility2";

	//Ability Sound
	auto tmepSound = MANAGER_RESOURCES()->GetResource<Sounds>(L"Ragnaros_Ability2");
	if (tmepSound == nullptr)
	{
		shared_ptr<Sounds> sound = make_shared<Sounds>();
		wstring soundPath = RESOURCES_ADDR_SOUND;
		soundPath += L"Character/Enemy/Ragnaros/Ragnaros_Ability2.mp3";
		sound->Load(soundPath);
		sound->SetVolume(50);
		MANAGER_RESOURCES()->AddResource<Sounds>(L"Ragnaros_Ability2", sound);

		_abiltySound = sound->Clone();
	}
	else
	{
		_abiltySound = tmepSound->Clone();
	}
}

RagnarosAbility2::~RagnarosAbility2()
{
}

void RagnarosAbility2::Enter(const shared_ptr<AIController>& controller, const wstring& prevTransition)
{
	if (controller != nullptr)
	{
		::srand(time(NULL));

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
			_animator.lock()->SetNextAnimation(L"Ability");
		}
		_characterInfo = _controller.lock()->GetCharacterInfo();
		_traceRadius = _characterInfo.lock()->GetDefaultCharacterInfo()._traceRadius;
		_attackRange = _characterInfo.lock()->GetDefaultCharacterInfo()._attackRange;
		_abilitySlot = _controller.lock()->GetGameObject()->GetComponent<AbilitySlot>();
		_abiltySound->Play(false);
	}
}

void RagnarosAbility2::Update()
{
	if (_controller.lock() != nullptr)
	{
		_dt = MANAGER_TIME()->GetDeltaTime();

		if (_animator.lock()->GetFrameEnd() == true)
		{
			if (_targetTransform.lock())
			{
				_abilitySlot.lock()->ExecuteAbility(1, _targetTransform.lock()->GetGameObject());
			}
			Out(L"RagnarosBattle");
		}

		if (_targetTransform.lock())
		{
			if (_targetTransform.lock()->GetGameObject()->GetComponent<CharacterController>()->_isAlive)
			{
				Vec3 myPos = _transform.lock()->GetLocalPosition();
				Vec3 targetPos = _targetTransform.lock()->GetLocalPosition();
				targetPos.y = myPos.y;
				Vec3 toTargetDir = targetPos - myPos;

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
	}
}

void RagnarosAbility2::UpdateFromServer()
{
	if (_controller.lock() != nullptr)
	{
		_dt = MANAGER_TIME()->GetDeltaTime();

		if (_animator.lock()->GetFrameEnd() == true)
		{
			if (_targetTransform.lock())
			{
				_abilitySlot.lock()->ExecuteAbility(1, _targetTransform.lock()->GetGameObject());
			}
			Out(L"RagnarosBattle");
		}

		if (_targetTransform.lock())
		{
			if (_targetTransform.lock()->GetGameObject()->GetComponent<CharacterController>()->_isAlive)
			{
				Vec3 myPos = _transform.lock()->GetLocalPosition();
				Vec3 targetPos = _targetTransform.lock()->GetLocalPosition();
				targetPos.y = myPos.y;
				Vec3 toTargetDir = targetPos - myPos;

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
	}
}

void RagnarosAbility2::Out(const wstring& nextTransition)
{
	if (_controller.lock() != nullptr)
	{
		_controller.lock()->SetCurrentFsmStrategy(_name, nextTransition);
	}
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
			_animator.lock()->SetNextAnimation(L"Submerged");
		}

		_targetList = _controller.lock()->GetTargetList();
	}
}

void RagnarosEncounterEvent1::Update()
{
	if (_controller.lock() != nullptr)
	{
		if (_targetList.lock()->size() > 0)
		{
			map<float, shared_ptr<TargetDesc>> ToTargetList;

			for (const auto& target : *_targetList.lock())
			{
				Vec3 myPos = _transform.lock()->GetLocalPosition();
				Vec3 targetPos = target->Target->GetTransform()->GetLocalPosition();
				targetPos.y = myPos.y;
				bool& isAlive = target->Target->GetComponent<CharacterController>()->_isAlive;
				float Length = Vec3::Distance(myPos, targetPos);

				if (Length <= _encounterDistance && isAlive)
				{
					ToTargetList.insert(make_pair(Length, target));
				}
			}

			if (ToTargetList.size() > 0)
			{
				float minDistance = ToTargetList.begin()->first;
				shared_ptr<GameObject> FinalTarget;
				FinalTarget = ToTargetList.begin()->second->Target;

				if (FinalTarget != nullptr)
				{
					_controller.lock()->SetTargetTransform(FinalTarget->GetTransform());
					Out(L"RagnarosEncounterEvent2");
				}
			}
		}
	}
}

void RagnarosEncounterEvent1::UpdateFromServer()
{
	if (_controller.lock() != nullptr)
	{
		if (_targetList.lock()->size() > 0)
		{
			map<float, shared_ptr<TargetDesc>> ToTargetList;

			for (const auto& target : *_targetList.lock())
			{
				Vec3 myPos = _transform.lock()->GetLocalPosition();
				Vec3 targetPos = target->Target->GetTransform()->GetLocalPosition();
				targetPos.y = myPos.y;
				bool& isAlive = target->Target->GetComponent<CharacterController>()->_isAlive;
				float Length = Vec3::Distance(myPos, targetPos);

				if (Length <= _encounterDistance && isAlive)
				{
					ToTargetList.insert(make_pair(Length, target));
				}
			}

			if (ToTargetList.size() > 0)
			{
				float minDistance = ToTargetList.begin()->first;
				shared_ptr<GameObject> FinalTarget;
				FinalTarget = ToTargetList.begin()->second->Target;

				if (FinalTarget != nullptr)
				{
					_controller.lock()->SetTargetTransform(FinalTarget->GetTransform());
					Out(L"RagnarosEncounterEvent2");
				}
			}
		}
	}
}

void RagnarosEncounterEvent1::Out(const wstring& nextTransition)
{
	if (_controller.lock() != nullptr)
	{
		_controller.lock()->SetCurrentFsmStrategy(_name, nextTransition);
	}
}

RagnarosEncounterEvent2::RagnarosEncounterEvent2()
{
	_name = L"RagnarosEncounterEvent2";

	//Event Sound
	auto tempSound1 = MANAGER_RESOURCES()->GetResource<Sounds>(L"Ragnaros_Event");
	if (tempSound1 == nullptr)
	{
		shared_ptr<Sounds> sound = make_shared<Sounds>();
		wstring soundPath = RESOURCES_ADDR_SOUND;
		soundPath += L"Character/Enemy/Ragnaros/Ragnaros_Event.mp3";
		sound->Load(soundPath);
		sound->SetVolume(50);
		MANAGER_RESOURCES()->AddResource<Sounds>(L"Ragnaros_Event", sound);

		_eventSound = sound->Clone();
	}
	else
	{
		_eventSound = tempSound1->Clone();
	}
}

RagnarosEncounterEvent2::~RagnarosEncounterEvent2()
{
}

void RagnarosEncounterEvent2::Enter(const shared_ptr<AIController>& controller, const wstring& prevTransition)
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
			_animator.lock()->SetNextAnimation(L"Birth");
		}

		_eventSound->Play(false);
	}
}

void RagnarosEncounterEvent2::Update()
{
	if (_controller.lock() != nullptr)
	{
		if (_animator.lock()->GetFrameEnd())
		{
			Out(L"RagnarosBattle");
		}
	}
}

void RagnarosEncounterEvent2::UpdateFromServer()
{
	if (_controller.lock() != nullptr)
	{
		if (_animator.lock()->GetFrameEnd())
		{
			Out(L"RagnarosBattle");
		}
	}
}

void RagnarosEncounterEvent2::Out(const wstring& nextTransition)
{
	if (_controller.lock() != nullptr)
	{
		_controller.lock()->SetCurrentFsmStrategy(_name, nextTransition);
	}
}

RagnarosAllDeadEvent::RagnarosAllDeadEvent()
{
	_name = L"RagnarosAllDeadEvent";

}

RagnarosAllDeadEvent::~RagnarosAllDeadEvent()
{
}

void RagnarosAllDeadEvent::Enter(const shared_ptr<AIController>& controller, const wstring& prevTransition)
{
}

void RagnarosAllDeadEvent::Update()
{
}

void RagnarosAllDeadEvent::UpdateFromServer()
{
}

void RagnarosAllDeadEvent::Out(const wstring& nextTransition)
{
	if (_controller.lock() != nullptr)
	{
		_controller.lock()->SetCurrentFsmStrategy(_name, nextTransition);
	}
}