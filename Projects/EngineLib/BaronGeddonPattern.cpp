#include "pch.h"
#include "BaronGeddonPattern.h"

#include "PlayerController.h"
#include "AIController.h"
#include "CharacterInfo.h"
#include "Sounds.h"
#include "AbilitySlot.h"

BaronGeddonStand::BaronGeddonStand()
{
	_name = L"BaronGeddonStand";
}

BaronGeddonStand::~BaronGeddonStand()
{
}

void BaronGeddonStand::Enter(const shared_ptr<AIController>& controller, const wstring& prevTransition)
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

		_characterInfo = _controller->GetCharacterInfo();
		_traceRadius = _characterInfo->GetDefaultCharacterInfo()._traceRadius;
		_attackRange = _characterInfo->GetDefaultCharacterInfo()._attackRange;
		_targetList = _controller->GetTargetList();
		_spwanPos = _controller->GetSpawnPosition();
	}
}

void BaronGeddonStand::Update()
{
	if (_controller != nullptr)
	{
		Vec3 myPos = _transform->GetLocalPosition();
		_spwanPos.y = myPos.y;

		/*float dist = Vec3::Distance(myPos, _spwanPos);

		if (dist > 5.f + FLT_EPSILON)
		{
			Out(L"BaronGeddonMoveToSpwanPoint");
		}*/

		if (_targetList->size() > 0)
		{
			//Taget 후보 결정
			map<float, shared_ptr<TargetDesc>> ToTargetList;

			for (const auto& target : *_targetList)
			{
				Vec3 myPos = _transform->GetLocalPosition();
				Vec3 targetPos = target->Target->GetTransform()->GetLocalPosition();
				bool& isAlive = target->Target->GetComponent<CharacterController>()->_isAlive;
				float Length = Vec3::Distance(myPos, targetPos);

				//자신의 위치와 타겟 위치가 추적거리 안에 존재 할 경우 탐색
				if (Length <= _traceRadius && isAlive)
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
					if (minDistance <= _attackRange && minDistance <= _traceRadius)
					{
						_controller->SetTargetTransform(FinalTarget->GetTransform());
						Out(L"BaronGeddonBattle");
					}
					else if (minDistance > _attackRange && minDistance <= _traceRadius)
					{
						_controller->SetTargetTransform(FinalTarget->GetTransform());
						Out(L"BaronGeddonTrace");
					}
				}
			}
		}
	}
}

void BaronGeddonStand::UpdateFromServer()
{
	if (_controller != nullptr)
	{
		if (_transform)
		{
			Vec3 myPos = _transform->GetLocalPosition();
			_spwanPos.y = myPos.y;

			/*float dist = Vec3::Distance(myPos, _spwanPos);

			if (dist > 5.f + FLT_EPSILON)
			{
				Out(L"BaronGeddonMoveToSpwanPoint");
			}*/

			if (_targetList->size() > 0)
			{
				//Taget 후보 결정
				map<float, shared_ptr<TargetDesc>> ToTargetList;

				for (const auto& target : *_targetList)
				{
					Vec3 myPos = _transform->GetLocalPosition();
					Vec3 targetPos = target->Target->GetTransform()->GetLocalPosition();
					bool& isAlive = target->Target->GetComponent<CharacterController>()->_isAlive;
					float Length = Vec3::Distance(myPos, targetPos);

					//자신의 위치와 타겟 위치가 추적거리 안에 존재 할 경우 탐색
					if (Length <= _traceRadius && isAlive)
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
						if (minDistance <= _attackRange && minDistance <= _traceRadius)
						{
							_controller->SetTargetTransform(FinalTarget->GetTransform());
							Out(L"BaronGeddonBattle");
						}
						else if (minDistance > _attackRange && minDistance <= _traceRadius)
						{
							_controller->SetTargetTransform(FinalTarget->GetTransform());
							Out(L"BaronGeddonTrace");
						}
					}
				}
			}
		}
	}
}

void BaronGeddonStand::Out(const wstring& nextTransition)
{
	if (_controller != nullptr)
	{
		_controller->SetCurrentFsmStrategy(_name, nextTransition);
	}
}

BaronGeddonDamaged::BaronGeddonDamaged()
{
	_name = L"BaronGeddonDamaged";

	//Damaged Sound
	auto tempSound = MANAGER_RESOURCES()->GetResource<Sounds>(L"BaronGeddon_Damaged");
	if (tempSound == nullptr)
	{
		shared_ptr<Sounds> sound = make_shared<Sounds>();
		wstring soundPath = RESOURCES_ADDR_SOUND;
		soundPath += L"Character/Enemy/BaronGeddon/BaronGeddon_Damaged.mp3";
		sound->Load(soundPath);
		sound->SetVolume(50);
		MANAGER_RESOURCES()->AddResource<Sounds>(L"BaronGeddon_Damaged", sound);

		_damagedSound = sound->Clone();
	}
	else
	{
		_damagedSound = tempSound->Clone();
	}
}

BaronGeddonDamaged::~BaronGeddonDamaged()
{
}

void BaronGeddonDamaged::Enter(const shared_ptr<AIController>& controller, const wstring& prevTransition)
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
			_animator->SetNextAnimation(L"Damaged");
		}
		_damagedSound->Play(false);
	}
}

void BaronGeddonDamaged::Update()
{
	if (_controller != nullptr)
	{
		if (_animator->GetFrameEnd() == true)
		{
			Out(L"BaronGeddonBattle");
		}
	}
}

void BaronGeddonDamaged::UpdateFromServer()
{
	if (_controller != nullptr)
	{
		if (_animator->GetFrameEnd() == true)
		{
			Out(L"BaronGeddonBattle");
		}
	}
}

void BaronGeddonDamaged::Out(const wstring& nextTransition)
{
	if (_controller != nullptr)
	{
		_controller->SetCurrentFsmStrategy(_name, nextTransition);
	}
}

BaronGeddonDead::BaronGeddonDead()
{
	_name = L"BaronGeddonDead";

	//Death Sound
	auto tempSound1 = MANAGER_RESOURCES()->GetResource<Sounds>(L"BaronGeddon_Death");
	if (tempSound1 == nullptr)
	{
		shared_ptr<Sounds> sound = make_shared<Sounds>();
		wstring soundPath = RESOURCES_ADDR_SOUND;
		soundPath += L"Character/Enemy/BaronGeddon/BaronGeddon_Death.mp3";
		sound->Load(soundPath);
		sound->SetVolume(50);
		MANAGER_RESOURCES()->AddResource<Sounds>(L"BaronGeddon_Death", sound);

		_deadSound = sound->Clone();
	}
	else
	{
		_deadSound = tempSound1->Clone();
	}
}

BaronGeddonDead::~BaronGeddonDead()
{
}

void BaronGeddonDead::Enter(const shared_ptr<AIController>& controller, const wstring& prevTransition)
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
		_soundFlag = false;
		_controller->_isAlive = false;
	}
}

void BaronGeddonDead::Update()
{
	if (_controller != nullptr)
	{
		_dt += MANAGER_TIME()->GetDeltaTime();

		if (_animator->GetFrameEnd() == true)
		{
			_controller->DeadEvent();
		}

		if (_dt > _soundTimer && _soundFlag == false)
		{
			_deadSound->Play(false);
			_soundFlag = true;
		}
	}
}

void BaronGeddonDead::UpdateFromServer()
{
	if (_controller != nullptr)
	{
		_dt += MANAGER_TIME()->GetDeltaTime();

		if (_animator->GetFrameEnd() == true)
		{
			_controller->DeadEvent();
		}

		if (_dt > _soundTimer && _soundFlag == false)
		{
			_deadSound->Play(false);
			_soundFlag = true;
		}
	}
}

void BaronGeddonDead::Out(const wstring& nextTransition)
{
	if (_controller != nullptr)
	{
		_controller->SetCurrentFsmStrategy(_name, nextTransition);
	}
}

BaronGeddonTrace::BaronGeddonTrace()
{
	_name = L"BaronGeddonTrace";
}

BaronGeddonTrace::~BaronGeddonTrace()
{
}

void BaronGeddonTrace::Enter(const shared_ptr<AIController>& controller, const wstring& prevTransition)
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
			_animator->SetNextAnimation(L"Run");
		}

		_characterInfo = _controller->GetCharacterInfo();
		_targetList = _controller->GetTargetList();
		_traceRadius = _characterInfo->GetDefaultCharacterInfo()._traceRadius;
		_attackRange = _characterInfo->GetDefaultCharacterInfo()._attackRange;
		_moveSpeed = _characterInfo->GetDefaultCharacterInfo()._moveSpeed;
	}
}

void BaronGeddonTrace::Update()
{
	if (_controller != nullptr)
	{
		_dt = MANAGER_TIME()->GetDeltaTime();

		//Target Update
		{
			if (_targetList->size() > 0)
			{
				float minAggro = 0.f;
				shared_ptr<Transform> _lastTarget;
				for (auto& target : *_targetList)
				{
					if (target->Target == _targetTransform->GetGameObject())
					{
						minAggro = target->AggroValue;
						_lastTarget = target->Target->GetTransform();
						continue;
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

		bool& isAlive = _targetTransform->GetGameObject()->GetComponent<CharacterController>()->_isAlive;
		if (isAlive == false)
		{
			Out(L"BaronGeddonMoveToSpwanPoint");
		}
		else
		{
			//자신의 위치에서 타겟방향으로 향하는 정규화 된 방향 벡터 계산(Normal Vector)
			Vec3 myPos = _transform->GetLocalPosition();
			Vec3 targetPos = _targetTransform->GetPosition();
			targetPos.y = myPos.y;
			Vec3 toTargetDir = targetPos - myPos;

			//타겟 방향으로 회전
			{
				if (toTargetDir.Length() > 0)
				{
					toTargetDir.Normalize(toTargetDir);
					{
						Vec3 myForward = _transform->GetLookVector();
						Vec3 myRight = _transform->GetRightVector();
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

						Vec3 myRot = _transform->GetLocalRotation();
						myRot.y += angle;
						_transform->SetLocalRotation(myRot);
					}
				}
			}

			toTargetDir.Normalize(toTargetDir);

			//타겟 방향으로 이동 & Attack Range 체크 후 도달 시 Trasition
			{
				Vec3 toTargetTranslate = myPos + (toTargetDir * _moveSpeed * _dt);
				_transform->SetPosition(toTargetTranslate);

				Vec3 targetPos2 = _targetTransform->GetLocalPosition();

				float distance = Vec3::Distance(toTargetTranslate, targetPos2);

				if (distance <= _attackRange && distance <= _traceRadius)
				{
					Out(L"BaronGeddonBattle");
				}
				else if (distance > _attackRange && distance > _traceRadius)
				{
					Out(L"BaronGeddonMoveToSpwanPoint");
				}
			}
		}
	}
}

void BaronGeddonTrace::UpdateFromServer()
{
	if (_controller != nullptr)
	{
		_dt = MANAGER_TIME()->GetDeltaTime();

		//Target Update
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
			if (isAlive == false)
			{
				Out(L"BaronGeddonMoveToSpwanPoint");
			}
			else
			{
				//자신의 위치에서 타겟방향으로 향하는 정규화 된 방향 벡터 계산(Normal Vector)
				Vec3 myPos = _transform->GetLocalPosition();
				Vec3 targetPos = _targetTransform->GetPosition();
				targetPos.y = myPos.y;
				Vec3 toTargetDir = targetPos - myPos;

				//타겟 방향으로 회전
				{
					if (toTargetDir.Length() > 0)
					{
						toTargetDir.Normalize(toTargetDir);
						{
							Vec3 myForward = _transform->GetLookVector();
							Vec3 myRight = _transform->GetRightVector();
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

							Vec3 myRot = _transform->GetLocalRotation();
							myRot.y += angle;
							_transform->SetLocalRotation(myRot);
						}
					}
				}

				toTargetDir.Normalize(toTargetDir);

				//타겟 방향으로 이동 & Attack Range 체크 후 도달 시 Trasition
				{
					Vec3 toTargetTranslate = myPos + (toTargetDir * _moveSpeed * _dt);
					_transform->SetPosition(toTargetTranslate);

					Vec3 targetPos2 = _targetTransform->GetLocalPosition();

					float distance = Vec3::Distance(toTargetTranslate, targetPos2);

					if (distance <= _attackRange && distance <= _traceRadius)
					{
						Out(L"BaronGeddonBattle");
					}
					else if (distance > _attackRange && distance > _traceRadius)
					{
						Out(L"BaronGeddonMoveToSpwanPoint");
					}
				}
			}
		}
	}
}

void BaronGeddonTrace::Out(const wstring& nextTransition)
{
	if (_controller != nullptr)
	{
		_controller->SetCurrentFsmStrategy(_name, nextTransition);
	}
}

BaronGeddonMoveToSpwanPoint::BaronGeddonMoveToSpwanPoint()
{
	_name = L"BaronGeddonMoveToSpwanPoint";
}

BaronGeddonMoveToSpwanPoint::~BaronGeddonMoveToSpwanPoint()
{
}

void BaronGeddonMoveToSpwanPoint::Enter(const shared_ptr<AIController>& controller, const wstring& prevTransition)
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
			_animator->SetNextAnimation(L"Run");
		}

		_characterInfo = _controller->GetCharacterInfo();
		_spwanPos = _controller->GetSpawnPosition();
		_moveSpeed = _characterInfo->GetDefaultCharacterInfo()._moveSpeed;
	}
}

void BaronGeddonMoveToSpwanPoint::Update()
{
	if (_controller != nullptr)
	{
		_dt = MANAGER_TIME()->GetDeltaTime();

		Vec3 myPos = _transform->GetLocalPosition();
		_spwanPos.y = myPos.y;
		Vec3 toTargetDir = _spwanPos - myPos;

		//타겟 방향으로 회전
		{
			if (toTargetDir.Length() > 0)
			{
				toTargetDir.Normalize(toTargetDir);
				{
					Vec3 myForward = _transform->GetLookVector();
					Vec3 myRight = _transform->GetRightVector();
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

					Vec3 myRot = _transform->GetLocalRotation();
					myRot.y += angle;
					_transform->SetLocalRotation(myRot);
				}
			}
		}

		toTargetDir.Normalize(toTargetDir);

		float moveToLength = Vec3::Distance(myPos, _spwanPos);

		if (moveToLength > 2.f + FLT_EPSILON)
		{
			Vec3 toSpwanPosTranslate = myPos + (toTargetDir * _moveSpeed * _dt);
			_transform->SetLocalPosition(toSpwanPosTranslate);
		}
		else
		{
			_transform->SetLocalPosition(_spwanPos);
			Out(L"BaronGeddonStand");
		}
	}
}

void BaronGeddonMoveToSpwanPoint::UpdateFromServer()
{
	if (_controller != nullptr)
	{
		_dt = MANAGER_TIME()->GetDeltaTime();

		Vec3 myPos = _transform->GetLocalPosition();
		_spwanPos.y = myPos.y;
		Vec3 toTargetDir = _spwanPos - myPos;

		//타겟 방향으로 회전
		{
			if (toTargetDir.Length() > 0)
			{
				toTargetDir.Normalize(toTargetDir);
				{
					Vec3 myForward = _transform->GetLookVector();
					Vec3 myRight = _transform->GetRightVector();
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

					Vec3 myRot = _transform->GetLocalRotation();
					myRot.y += angle;
					_transform->SetLocalRotation(myRot);
				}
			}
		}

		toTargetDir.Normalize(toTargetDir);

		float moveToLength = Vec3::Distance(myPos, _spwanPos);

		if (moveToLength > 2.f + FLT_EPSILON)
		{
		}
		else
		{
			Out(L"BaronGeddonStand");
		}
	}
}

void BaronGeddonMoveToSpwanPoint::Out(const wstring& nextTransition)
{
	if (_controller != nullptr)
	{
		_controller->SetCurrentFsmStrategy(_name, nextTransition);
	}
}

BaronGeddonBattle::BaronGeddonBattle()
{
	_name = L"BaronGeddonBattle";
}

BaronGeddonBattle::~BaronGeddonBattle()
{
}

void BaronGeddonBattle::Enter(const shared_ptr<AIController>& controller, const wstring& prevTransition)
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
			_animator->SetNextAnimation(L"Stand");
		}

		_characterInfo = _controller->GetCharacterInfo();
		_abilitySlot = _controller->GetGameObject()->GetComponent<AbilitySlot>();
		_abilityTime = _abilitySlot->GetAbility(0)->GetAbilityData().AbilityCoolTime;
		_targetList = _controller->GetTargetList();
		_traceRadius = _characterInfo->GetDefaultCharacterInfo()._traceRadius;
		_attackRange = _characterInfo->GetDefaultCharacterInfo()._attackRange;
		_attackTime = _characterInfo->GetDefaultCharacterInfo()._attackTime;
		_traceTime = 0.f;
	}
}

void BaronGeddonBattle::Update()
{
	if (_controller != nullptr)
	{
		_dt = MANAGER_TIME()->GetDeltaTime();
		_traceTime += _dt;

		bool& tempisAlive = _targetTransform->GetGameObject()->GetComponent<CharacterController>()->_isAlive;

		//Target update
		if (_targetList->size() <= 0 || tempisAlive == false)
		{
			Out(L"BaronGeddonMoveToSpwanPoint");
		}
		else
		{
			if (_targetList->size() > 0)
			{
				float minAggro = 0.f;
				shared_ptr<Transform> _lastTarget;
				for (auto& target : *_targetList)
				{
					if (target->Target == _targetTransform->GetGameObject())
					{
						minAggro = target->AggroValue;
						_lastTarget = target->Target->GetTransform();
						continue;
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

		bool& isAlive = _targetTransform->GetGameObject()->GetComponent<CharacterController>()->_isAlive;

		if (isAlive)
		{
			Vec3 myPos = _transform->GetLocalPosition();
			Vec3 targetPos = _targetTransform->GetLocalPosition();
			targetPos.y = myPos.y;
			Vec3 toTargetDir = targetPos - myPos;

			//타겟 방향으로 회전
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

			_abilityTimer += _dt;

			//Ability Transition
			{
				if (_abilityTimer >= _abilityTime)
				{
					_abilityTimer = 0.f;
					Out(L"BaronGeddonAbility");
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
						Out(L"BaronGeddonAttack");
					}
				}
			}
			else if (distance > _attackRange && distance <= _traceRadius)
			{
				if (_traceTime + FLT_EPSILON > _traceWaitingTime)
				{
					Out(L"BaronGeddonTrace");
				}
			}
			else if (distance > _attackRange && distance > _traceRadius)
			{
				Out(L"BaronGeddonMoveToSpwanPoint");
			}
		}
		else
		{
			Out(L"BaronGeddonMoveToSpwanPoint");
		}
	}
}

void BaronGeddonBattle::UpdateFromServer()
{
	if (_controller != nullptr)
	{
		_dt = MANAGER_TIME()->GetDeltaTime();
		_traceTime += _dt;
		
		if (_targetTransform)
		{
			bool& tempisAlive = _targetTransform->GetGameObject()->GetComponent<CharacterController>()->_isAlive;

			//Target update
			if (_targetList->size() <= 0 || tempisAlive == false)
			{
				Out(L"BaronGeddonMoveToSpwanPoint");
			}
			else
			{
				if (_targetList->size() > 0)
				{
					float minAggro = 0.f;
					shared_ptr<Transform> _lastTarget;
					for (auto& target : *_targetList)
					{
						if (target->Target == _targetTransform->GetGameObject())
						{
							minAggro = target->AggroValue;
							_lastTarget = target->Target->GetTransform();
							continue;
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

			bool& isAlive = _targetTransform->GetGameObject()->GetComponent<CharacterController>()->_isAlive;

			if (isAlive)
			{
				Vec3 myPos = _transform->GetLocalPosition();
				Vec3 targetPos = _targetTransform->GetLocalPosition();
				targetPos.y = myPos.y;
				Vec3 toTargetDir = targetPos - myPos;

				//타겟 방향으로 회전
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

				_abilityTimer += _dt;

				//Ability Transition
				{
					if (_abilityTimer >= _abilityTime)
					{
						_abilityTimer = 0.f;
						Out(L"BaronGeddonAbility");
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
							Out(L"BaronGeddonAttack");
						}
					}
				}
				else if (distance > _attackRange && distance <= _traceRadius)
				{
					if (_traceTime + FLT_EPSILON > _traceWaitingTime)
					{
						Out(L"BaronGeddonTrace");
					}
				}
				else if (distance > _attackRange && distance > _traceRadius)
				{
					Out(L"BaronGeddonMoveToSpwanPoint");
				}
			}
			else
			{
				Out(L"BaronGeddonMoveToSpwanPoint");
			}
		}
	}
}

void BaronGeddonBattle::Out(const wstring& nextTransition)
{
	if (_controller != nullptr)
	{
		_controller->SetCurrentFsmStrategy(_name, nextTransition);
	}
}

BaronGeddonAttack::BaronGeddonAttack()
{
	_name = L"BaronGeddonAttack";

	//Attack1 Sound
	auto tempSound1 = MANAGER_RESOURCES()->GetResource<Sounds>(L"BaronGeddon_Attack1");
	if (tempSound1 == nullptr)
	{
		shared_ptr<Sounds> sound = make_shared<Sounds>();
		wstring soundPath = RESOURCES_ADDR_SOUND;
		soundPath += L"Character/Enemy/BaronGeddon/BaronGeddon_Attack1.mp3";
		sound->Load(soundPath);
		sound->SetVolume(50);
		MANAGER_RESOURCES()->AddResource<Sounds>(L"BaronGeddon_Attack1", sound);

		_attack1Sound = sound->Clone();
	}
	else
	{
		_attack1Sound = tempSound1->Clone();
	}

	//Attack2 Sound
	auto tmepSound2 = MANAGER_RESOURCES()->GetResource<Sounds>(L"BaronGeddon_Attack2");
	if (tmepSound2 == nullptr)
	{
		shared_ptr<Sounds> sound = make_shared<Sounds>();
		wstring soundPath = RESOURCES_ADDR_SOUND;
		soundPath += L"Character/Enemy/BaronGeddon/BaronGeddon_Attack2.mp3";
		sound->Load(soundPath);
		sound->SetVolume(50);
		MANAGER_RESOURCES()->AddResource<Sounds>(L"BaronGeddon_Attack2", sound);

		_attack2Sound = sound->Clone();
	}
	else
	{
		_attack2Sound = tmepSound2->Clone();
	}
}

BaronGeddonAttack::~BaronGeddonAttack()
{
}

void BaronGeddonAttack::Enter(const shared_ptr<AIController>& controller, const wstring& prevTransition)
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
					float distance = Vec3::Distance(_transform->GetLocalPosition(), _targetTransform->GetLocalPosition());
					if (distance < 60)
					{
						float attackDamage = _characterInfo->GetCharacterInfo()._atk;
						targetCon->TakeDamage(_transform->GetGameObject(), attackDamage);
					}
				}
			}

			int randAttack = rand() % 2;

			if (randAttack == 0)
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

void BaronGeddonAttack::Update()
{
	if (_controller != nullptr)
	{
		_dt = MANAGER_TIME()->GetDeltaTime();

		if (_animator->GetFrameEnd() == true)
		{
			Out(L"BaronGeddonBattle");
		}

		if (_targetTransform->GetGameObject()->GetComponent<CharacterController>()->_isAlive)
		{
			Vec3 myPos = _transform->GetLocalPosition();
			Vec3 targetPos = _targetTransform->GetLocalPosition();
			targetPos.y = myPos.y;
			Vec3 toTargetDir = targetPos - myPos;

			//타겟 방향으로 회전
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

void BaronGeddonAttack::UpdateFromServer()
{
	if (_controller != nullptr)
	{
		_dt = MANAGER_TIME()->GetDeltaTime();

		if (_animator->GetFrameEnd() == true)
		{
			Out(L"BaronGeddonBattle");
		}

		if (_targetTransform)
		{
			if (_targetTransform->GetGameObject()->GetComponent<CharacterController>()->_isAlive)
			{
				Vec3 myPos = _transform->GetLocalPosition();
				Vec3 targetPos = _targetTransform->GetLocalPosition();
				targetPos.y = myPos.y;
				Vec3 toTargetDir = targetPos - myPos;

				//타겟 방향으로 회전
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

void BaronGeddonAttack::Out(const wstring& nextTransition)
{
	if (_controller != nullptr)
	{
		_controller->SetCurrentFsmStrategy(_name, nextTransition);
	}
}

BaronGeddonAbility::BaronGeddonAbility()
{
	_name = L"BaronGeddonAbility";

	//Ability Sound
	auto tmepSound = MANAGER_RESOURCES()->GetResource<Sounds>(L"BaronGeddon_Ability1");
	if (tmepSound == nullptr)
	{
		shared_ptr<Sounds> sound = make_shared<Sounds>();
		wstring soundPath = RESOURCES_ADDR_SOUND;
		soundPath += L"Character/Enemy/BaronGeddon/BaronGeddon_Ability1.mp3";
		sound->Load(soundPath);
		sound->SetVolume(50);
		MANAGER_RESOURCES()->AddResource<Sounds>(L"BaronGeddon_Ability1", sound);

		_abiltySound = sound->Clone();
	}
	else
	{
		_abiltySound = tmepSound->Clone();
	}
}

BaronGeddonAbility::~BaronGeddonAbility()
{
}

void BaronGeddonAbility::Enter(const shared_ptr<AIController>& controller, const wstring& prevTransition)
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
		_abFlag = false;
		_abTimer = 0.f;
	}
}

void BaronGeddonAbility::Update()
{
	if (_controller != nullptr)
	{
		_dt = MANAGER_TIME()->GetDeltaTime();
		_abTimer += _dt;

		if (_animator->GetFrameEnd() == true)
		{
			Out(L"BaronGeddonBattle");
		}

		if (_abTimer > _abTime && _abFlag == false)
		{
			_abilitySlot->ExecuteAbility(0, _targetTransform->GetGameObject());
			_abiltySound->Play(false);
			_abFlag = true;
		}

		if (_targetTransform->GetGameObject()->GetComponent<CharacterController>()->_isAlive)
		{
			Vec3 myPos = _transform->GetLocalPosition();
			Vec3 targetPos = _targetTransform->GetLocalPosition();
			targetPos.y = myPos.y;
			Vec3 toTargetDir = targetPos - myPos;

			//타겟 방향으로 회전
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

void BaronGeddonAbility::UpdateFromServer()
{
	if (_controller != nullptr)
	{
		_dt = MANAGER_TIME()->GetDeltaTime();
		_abTimer += _dt;

		if (_animator->GetFrameEnd() == true)
		{
			Out(L"BaronGeddonBattle");
		}

		if (_targetTransform)
		{
			if (_abTimer > _abTime && _abFlag == false)
			{
				_abilitySlot->ExecuteAbility(0, _targetTransform->GetGameObject());
				_abiltySound->Play(false);
				_abFlag = true;
			}

			if (_targetTransform->GetGameObject()->GetComponent<CharacterController>()->_isAlive)
			{
				Vec3 myPos = _transform->GetLocalPosition();
				Vec3 targetPos = _targetTransform->GetLocalPosition();
				targetPos.y = myPos.y;
				Vec3 toTargetDir = targetPos - myPos;

				//타겟 방향으로 회전
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

void BaronGeddonAbility::Out(const wstring& nextTransition)
{
	if (_controller != nullptr)
	{
		_controller->SetCurrentFsmStrategy(_name, nextTransition);
	}
}
