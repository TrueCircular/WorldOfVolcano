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

		if (_controller->GetTransform() != nullptr)
			_transform = _controller->GetTransform();

		if (_controller->GetAnimator() != nullptr)
			_animator = _controller->GetAnimator();

		if (_animator != nullptr)
		{
			_animator->SetFrameEnd(false);
			_animator->SetNextAnimation(L"Stand");
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
	if (_controller != nullptr)
	{
		_controller->SetCurrentFsmStrategy(_name, nextTransition);
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
	if (_controller != nullptr)
	{
		Out(_prevTransition);
	}
}

void RagnarosDamaged::UpdateFromServer()
{
	if (_controller != nullptr)
	{
		Out(_prevTransition);
	}
}

void RagnarosDamaged::Out(const wstring& nextTransition)
{
	if (_controller != nullptr)
	{
		_controller->SetCurrentFsmStrategy(_name, nextTransition);
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

		if (_controller->GetAnimator() != nullptr)
			_animator = _controller->GetAnimator();

		if (_animator != nullptr)
		{
			_animator->SetFrameEnd(false);
			_animator->SetNextAnimation(L"Stun");
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
	if (_controller != nullptr)
	{
		_controller->SetCurrentFsmStrategy(_name, nextTransition);
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

		if (_controller->GetAnimator() != nullptr)
			_animator = _controller->GetAnimator();

		if (_animator != nullptr)
		{
			_animator->SetFrameEnd(false);
			_animator->SetNextAnimation(L"Death");
		}
		_dt = 0.f;
		_controller->_isAlive = false;
		_deadSound->Play(false);
	}
}

void RagnarosDead::Update()
{
	if (_controller != nullptr)
	{
		_dt += MANAGER_TIME()->GetDeltaTime();

		if (_animator->GetFrameEnd() == true)
		{
			_controller->DeadEvent();
			MANAGER_IMGUI()->NotifyEnding();
		}
	}
}

void RagnarosDead::UpdateFromServer()
{
	if (_controller != nullptr)
	{
		_dt += MANAGER_TIME()->GetDeltaTime();

		if (_animator->GetFrameEnd() == true)
		{
			_controller->DeadEvent();
			MANAGER_IMGUI()->NotifyEnding();
		}
	}
}

void RagnarosDead::Out(const wstring& nextTransition)
{
	if (_controller != nullptr)
	{
		_controller->SetCurrentFsmStrategy(_name, nextTransition);
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

		if (_controller->GetTransform() != nullptr)
			_transform = _controller->GetTransform();

		if (_controller->GetTargetTransform() != nullptr)
			_targetTransform = _controller->GetTargetTransform();

		if (_controller->GetAnimator() != nullptr)
			_animator = _controller->GetAnimator();

		if (_animator != nullptr)
		{
			_animator->SetFrameEnd(false);
			_animator->SetNextAnimation(L"Battle");
		}

		_characterInfo = _controller->GetCharacterInfo();
		_abilitySlot = _controller->GetGameObject()->GetComponent<AbilitySlot>();
		if (_abilitySlot != nullptr)
		{
			if (_abilitySlot->GetAbility(0))
			{
				_abilityTime = _abilitySlot->GetAbility(0)->GetAbilityData().AbilityCoolTime;
			}

			if (_abilitySlot->GetAbility(1))
			{
				_abilityTime2 = _abilitySlot->GetAbility(1)->GetAbilityData().AbilityCoolTime;
			}
		}
		_targetList = _controller->GetTargetList();
		_traceRadius = _characterInfo->GetDefaultCharacterInfo()._traceRadius;
		_attackRange = _characterInfo->GetDefaultCharacterInfo()._attackRange;
		_attackTime = _characterInfo->GetDefaultCharacterInfo()._attackTime;
	}
}

void RagnarosBattle::Update()
{
	if (_controller != nullptr)
	{
		_dt = MANAGER_TIME()->GetDeltaTime();

		//Target update
		if (_targetList->size() <= 0)
		{
			Out(L"RagnarosStand");
		}
		else
		{
			if (_targetList->size() > 0)
			{
				float minAggro = 0.f;
				shared_ptr<Transform> _lastTarget;
				for (auto& target : *_targetList)
				{
					if (_targetTransform)
					{
						if (target->Target == _targetTransform->GetGameObject())
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
					_controller->SetTargetTransform(_targetTransform);
				}
			}
		}

		if (_targetTransform)
		{
			bool& isAlive = _targetTransform->GetGameObject()->GetComponent<CharacterController>()->_isAlive;

			if (isAlive)
			{
				Vec3 myPos = _transform->GetLocalPosition();
				Vec3 targetPos = _targetTransform->GetLocalPosition();
				targetPos.y = myPos.y;
				Vec3 toTargetDir = targetPos - myPos;

				{
					if (toTargetDir.Length() > 0)
					{
						toTargetDir.Normalize(toTargetDir);
						{
							Vec3 myForward = _transform->GetLookVector();
							Vec3 myRight = _transform->GetRightVector();
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

							Vec3 myRot = _transform->GetLocalRotation();
							myRot.y += angle;
							_transform->SetLocalRotation(myRot);
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
	if (_controller != nullptr)
	{
		_dt = MANAGER_TIME()->GetDeltaTime();

		//Target update
		if (_targetList->size() <= 0)
		{
			Out(L"RagnarosStand");
		}
		else
		{
			if (_targetList->size() > 0)
			{
				float minAggro = 0.f;
				shared_ptr<Transform> _lastTarget;
				for (auto& target : *_targetList)
				{
					if (_targetTransform)
					{
						if (target->Target == _targetTransform->GetGameObject())
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
					_controller->SetTargetTransform(_targetTransform);
				}
			}
		}

		if (_targetTransform)
		{
			bool& isAlive = _targetTransform->GetGameObject()->GetComponent<CharacterController>()->_isAlive;

			if (isAlive)
			{
				Vec3 myPos = _transform->GetLocalPosition();
				Vec3 targetPos = _targetTransform->GetLocalPosition();
				targetPos.y = myPos.y;
				Vec3 toTargetDir = targetPos - myPos;

				{
					if (toTargetDir.Length() > 0)
					{
						toTargetDir.Normalize(toTargetDir);
						{
							Vec3 myForward = _transform->GetLookVector();
							Vec3 myRight = _transform->GetRightVector();
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

							Vec3 myRot = _transform->GetLocalRotation();
							myRot.y += angle;
							_transform->SetLocalRotation(myRot);
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
	if (_controller != nullptr)
	{
		_controller->SetCurrentFsmStrategy(_name, nextTransition);
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

		if (_controller->GetTransform() != nullptr)
			_transform = _controller->GetTransform();

		if (_controller->GetTargetTransform() != nullptr)
			_targetTransform = _controller->GetTargetTransform();

		if (_controller->GetAnimator() != nullptr)
			_animator = _controller->GetAnimator();

		if (_animator != nullptr)
		{
			_characterInfo = _controller->GetCharacterInfo();
			_traceRadius = _characterInfo->GetDefaultCharacterInfo()._traceRadius;
			_attackRange = _characterInfo->GetDefaultCharacterInfo()._attackRange;

			if (_targetTransform)
			{
				auto targetCon = _targetTransform->GetGameObject()->GetComponent<CharacterController>();

				if (targetCon != nullptr)
				{
					float attackDamage = _characterInfo->GetCharacterInfo()._atk;
					targetCon->TakeDamage(_transform->GetGameObject(), attackDamage);
				}
			}

			_randAttack = rand() % 2;

			if (_randAttack == 0)
			{
				_animator->SetFrameEnd(false);
				_animator->SetNextAnimation(L"Attack1");
				_attack1Sound->Play(false);
			}
			else
			{
				_animator->SetFrameEnd(false);
				_animator->SetNextAnimation(L"Attack2");
				_attack2Sound->Play(false);
			}
		}
	}
}

void RagnarosAttack::Update()
{
	if (_controller != nullptr)
	{
		_dt = MANAGER_TIME()->GetDeltaTime();

		if (_animator->GetFrameEnd() == true)
		{
			Out(L"RagnarosBattle");
		}

		if (_targetTransform)
		{
			if (_targetTransform->GetGameObject()->GetComponent<CharacterController>()->_isAlive)
			{
				Vec3 myPos = _transform->GetLocalPosition();
				Vec3 targetPos = _targetTransform->GetLocalPosition();
				targetPos.y = myPos.y;
				Vec3 toTargetDir = targetPos - myPos;

				{
					if (toTargetDir.Length() > 0)
					{
						toTargetDir.Normalize(toTargetDir);
						{
							Vec3 myForward = _transform->GetLookVector();
							Vec3 myRight = _transform->GetRightVector();
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

							Vec3 myRot = _transform->GetLocalRotation();
							myRot.y += angle;
							_transform->SetLocalRotation(myRot);
						}
					}
				}
			}
		}
	}
}

void RagnarosAttack::UpdateFromServer()
{
	if (_controller != nullptr)
	{
		_dt = MANAGER_TIME()->GetDeltaTime();

		if (_animator->GetFrameEnd() == true)
		{
			Out(L"RagnarosBattle");
		}

		if (_targetTransform)
		{
			if (_targetTransform->GetGameObject()->GetComponent<CharacterController>()->_isAlive)
			{
				Vec3 myPos = _transform->GetLocalPosition();
				Vec3 targetPos = _targetTransform->GetLocalPosition();
				targetPos.y = myPos.y;
				Vec3 toTargetDir = targetPos - myPos;

				{
					if (toTargetDir.Length() > 0)
					{
						toTargetDir.Normalize(toTargetDir);
						{
							Vec3 myForward = _transform->GetLookVector();
							Vec3 myRight = _transform->GetRightVector();
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

							Vec3 myRot = _transform->GetLocalRotation();
							myRot.y += angle;
							_transform->SetLocalRotation(myRot);
						}
					}
				}
			}
		}
	}
}

void RagnarosAttack::Out(const wstring& nextTransition)
{
	if (_controller != nullptr)
	{
		_controller->SetCurrentFsmStrategy(_name, nextTransition);
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

		if (_controller->GetTransform() != nullptr)
			_transform = _controller->GetTransform();

		if (_controller->GetTargetTransform() != nullptr)
			_targetTransform = _controller->GetTargetTransform();

		if (_controller->GetAnimator() != nullptr)
			_animator = _controller->GetAnimator();

		if (_animator != nullptr)
		{
			_animator->SetFrameEnd(false);
			_animator->SetNextAnimation(L"Ability");
		}
		_characterInfo = _controller->GetCharacterInfo();
		_traceRadius = _characterInfo->GetDefaultCharacterInfo()._traceRadius;
		_attackRange = _characterInfo->GetDefaultCharacterInfo()._attackRange;
		_abilitySlot = _controller->GetGameObject()->GetComponent<AbilitySlot>();
		_abiltySound->Play(false);
	}
}

void RagnarosAbility1::Update()
{
	if (_controller != nullptr)
	{
		_dt = MANAGER_TIME()->GetDeltaTime();

		if (_animator->GetFrameEnd() == true)
		{
			if (_targetTransform)
			{
				_abilitySlot->ExecuteAbility(0, _targetTransform->GetGameObject());
			}
			Out(L"RagnarosBattle");
		}

		if (_targetTransform)
		{
			if (_targetTransform->GetGameObject()->GetComponent<CharacterController>()->_isAlive)
			{
				Vec3 myPos = _transform->GetLocalPosition();
				Vec3 targetPos = _targetTransform->GetLocalPosition();
				targetPos.y = myPos.y;
				Vec3 toTargetDir = targetPos - myPos;

				{
					if (toTargetDir.Length() > 0)
					{
						toTargetDir.Normalize(toTargetDir);
						{
							Vec3 myForward = _transform->GetLookVector();
							Vec3 myRight = _transform->GetRightVector();
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

							Vec3 myRot = _transform->GetLocalRotation();
							myRot.y += angle;
							_transform->SetLocalRotation(myRot);
						}
					}
				}
			}
		}
	}
}

void RagnarosAbility1::UpdateFromServer()
{
	if (_controller != nullptr)
	{
		_dt = MANAGER_TIME()->GetDeltaTime();

		if (_animator->GetFrameEnd() == true)
		{
			if (_targetTransform)
			{
				_abilitySlot->ExecuteAbility(0, _targetTransform->GetGameObject());
			}
			Out(L"RagnarosBattle");
		}

		if (_targetTransform)
		{
			if (_targetTransform->GetGameObject()->GetComponent<CharacterController>()->_isAlive)
			{
				Vec3 myPos = _transform->GetLocalPosition();
				Vec3 targetPos = _targetTransform->GetLocalPosition();
				targetPos.y = myPos.y;
				Vec3 toTargetDir = targetPos - myPos;

				{
					if (toTargetDir.Length() > 0)
					{
						toTargetDir.Normalize(toTargetDir);
						{
							Vec3 myForward = _transform->GetLookVector();
							Vec3 myRight = _transform->GetRightVector();
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

							Vec3 myRot = _transform->GetLocalRotation();
							myRot.y += angle;
							_transform->SetLocalRotation(myRot);
						}
					}
				}
			}
		}
	}
}

void RagnarosAbility1::Out(const wstring& nextTransition)
{
	if (_controller != nullptr)
	{
		_controller->SetCurrentFsmStrategy(_name, nextTransition);
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

		if (_controller->GetTransform() != nullptr)
			_transform = _controller->GetTransform();

		if (_controller->GetTargetTransform() != nullptr)
			_targetTransform = _controller->GetTargetTransform();

		if (_controller->GetAnimator() != nullptr)
			_animator = _controller->GetAnimator();

		if (_animator != nullptr)
		{
			_animator->SetFrameEnd(false);
			_animator->SetNextAnimation(L"Ability");
		}
		_characterInfo = _controller->GetCharacterInfo();
		_traceRadius = _characterInfo->GetDefaultCharacterInfo()._traceRadius;
		_attackRange = _characterInfo->GetDefaultCharacterInfo()._attackRange;
		_abilitySlot = _controller->GetGameObject()->GetComponent<AbilitySlot>();
		_abiltySound->Play(false);
	}
}

void RagnarosAbility2::Update()
{
	if (_controller != nullptr)
	{
		_dt = MANAGER_TIME()->GetDeltaTime();

		if (_animator->GetFrameEnd() == true)
		{
			if (_targetTransform)
			{
				_abilitySlot->ExecuteAbility(1, _targetTransform->GetGameObject());
			}
			Out(L"RagnarosBattle");
		}

		if (_targetTransform)
		{
			if (_targetTransform->GetGameObject()->GetComponent<CharacterController>()->_isAlive)
			{
				Vec3 myPos = _transform->GetLocalPosition();
				Vec3 targetPos = _targetTransform->GetLocalPosition();
				targetPos.y = myPos.y;
				Vec3 toTargetDir = targetPos - myPos;

				{
					if (toTargetDir.Length() > 0)
					{
						toTargetDir.Normalize(toTargetDir);
						{
							Vec3 myForward = _transform->GetLookVector();
							Vec3 myRight = _transform->GetRightVector();
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

							Vec3 myRot = _transform->GetLocalRotation();
							myRot.y += angle;
							_transform->SetLocalRotation(myRot);
						}
					}
				}
			}

		}
	}
}

void RagnarosAbility2::UpdateFromServer()
{
	if (_controller != nullptr)
	{
		_dt = MANAGER_TIME()->GetDeltaTime();

		if (_animator->GetFrameEnd() == true)
		{
			if (_targetTransform)
			{
				_abilitySlot->ExecuteAbility(1, _targetTransform->GetGameObject());
			}
			Out(L"RagnarosBattle");
		}

		if (_targetTransform)
		{
			if (_targetTransform->GetGameObject()->GetComponent<CharacterController>()->_isAlive)
			{
				Vec3 myPos = _transform->GetLocalPosition();
				Vec3 targetPos = _targetTransform->GetLocalPosition();
				targetPos.y = myPos.y;
				Vec3 toTargetDir = targetPos - myPos;

				{
					if (toTargetDir.Length() > 0)
					{
						toTargetDir.Normalize(toTargetDir);
						{
							Vec3 myForward = _transform->GetLookVector();
							Vec3 myRight = _transform->GetRightVector();
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

							Vec3 myRot = _transform->GetLocalRotation();
							myRot.y += angle;
							_transform->SetLocalRotation(myRot);
						}
					}
				}
			}

		}
	}
}

void RagnarosAbility2::Out(const wstring& nextTransition)
{
	if (_controller != nullptr)
	{
		_controller->SetCurrentFsmStrategy(_name, nextTransition);
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

		if (_controller->GetTransform() != nullptr)
			_transform = _controller->GetTransform();

		if (_controller->GetAnimator() != nullptr)
			_animator = _controller->GetAnimator();

		if (_animator != nullptr)
		{
			_animator->SetFrameEnd(false);
			_animator->SetNextAnimation(L"Submerged");
		}

		_targetList = _controller->GetTargetList();
	}
}

void RagnarosEncounterEvent1::Update()
{
	if (_controller != nullptr)
	{
		if (_targetList->size() > 0)
		{
			map<float, shared_ptr<TargetDesc>> ToTargetList;

			for (const auto& target : *_targetList)
			{
				Vec3 myPos = _transform->GetLocalPosition();
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
					_controller->SetTargetTransform(FinalTarget->GetTransform());
					Out(L"RagnarosEncounterEvent2");
				}
			}
		}
	}
}

void RagnarosEncounterEvent1::UpdateFromServer()
{
	if (_controller != nullptr)
	{
		if (_targetList->size() > 0)
		{
			map<float, shared_ptr<TargetDesc>> ToTargetList;

			for (const auto& target : *_targetList)
			{
				Vec3 myPos = _transform->GetLocalPosition();
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
					_controller->SetTargetTransform(FinalTarget->GetTransform());
					Out(L"RagnarosEncounterEvent2");
				}
			}
		}
	}
}

void RagnarosEncounterEvent1::Out(const wstring& nextTransition)
{
	if (_controller != nullptr)
	{
		_controller->SetCurrentFsmStrategy(_name, nextTransition);
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

		if (_controller->GetTransform() != nullptr)
			_transform = _controller->GetTransform();

		if (_controller->GetAnimator() != nullptr)
			_animator = _controller->GetAnimator();

		if (_animator != nullptr)
		{
			_animator->SetFrameEnd(false);
			_animator->SetNextAnimation(L"Birth");
		}

		_eventSound->Play(false);
	}
}

void RagnarosEncounterEvent2::Update()
{
	if (_controller != nullptr)
	{
		if (_animator->GetFrameEnd())
		{
			Out(L"RagnarosBattle");
		}
	}
}

void RagnarosEncounterEvent2::UpdateFromServer()
{
	if (_controller != nullptr)
	{
		if (_animator->GetFrameEnd())
		{
			Out(L"RagnarosBattle");
		}
	}
}

void RagnarosEncounterEvent2::Out(const wstring& nextTransition)
{
	if (_controller != nullptr)
	{
		_controller->SetCurrentFsmStrategy(_name, nextTransition);
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
	if (_controller != nullptr)
	{
		_controller->SetCurrentFsmStrategy(_name, nextTransition);
	}
}