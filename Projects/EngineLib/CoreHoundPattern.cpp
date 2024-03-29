#include "pch.h"
#include "CoreHoundPattern.h"

#include "PlayerController.h"
#include "AIController.h"
#include "CharacterInfo.h"
#include "Sounds.h"

CoreHoundStand::CoreHoundStand()
{
	_name = L"CoreHoundStand";
}

CoreHoundStand::~CoreHoundStand()
{
}

void CoreHoundStand::Enter(const shared_ptr<AIController>& controller, const wstring& prevTransition)
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
		_characterInfo = _controller.lock()->GetCharacterInfo();
		_traceRadius = _characterInfo.lock()->GetDefaultCharacterInfo()._traceRadius;
		_attackRange = _characterInfo.lock()->GetDefaultCharacterInfo()._attackRange;
		_targetList = _controller.lock()->GetTargetList();
	}
}

void CoreHoundStand::Update()
{
	if (_controller.lock() != nullptr)
	{
		if (_targetList.lock()->size() > 0)
		{
			//Taget 후보 결정
			map<float, shared_ptr<TargetDesc>> ToTargetList;

			for (const auto& target : *_targetList.lock())
			{
				Vec3 myPos = _transform.lock()->GetLocalPosition();
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
						_controller.lock()->SetTargetTransform(FinalTarget->GetTransform());
						Out(L"CoreHoundBattle");
					}
					else if (minDistance > _attackRange && minDistance <= _traceRadius)
					{
						_controller.lock()->SetTargetTransform(FinalTarget->GetTransform());
						Out(L"CoreHoundTrace");
					}
				}
			}
		}
	}
}

void CoreHoundStand::UpdateFromServer()
{
	if (_controller.lock() != nullptr)
	{
		if (_targetList.lock()->size() > 0)
		{
			//Taget 후보 결정
			map<float, shared_ptr<TargetDesc>> ToTargetList;

			for (const auto& target : *_targetList.lock())
			{
				Vec3 myPos = _transform.lock()->GetLocalPosition();
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
						_controller.lock()->SetTargetTransform(FinalTarget->GetTransform());
					}
					else if (minDistance > _attackRange && minDistance <= _traceRadius)
					{
						_controller.lock()->SetTargetTransform(FinalTarget->GetTransform());
					}
				}
			}
		}
	}
}

void CoreHoundStand::Out(const wstring& nextTransition)
{
	if (_controller.lock() != nullptr)
	{
		_controller.lock()->SetCurrentFsmStrategy(_name, nextTransition);
	}
}

CoreHoundDamaged::CoreHoundDamaged()
{
	_name = L"CoreHoundDamaged";

	//Damaged Sound
	auto tempSound = MANAGER_RESOURCES()->GetResource<Sounds>(L"CoreHound_Damaged");
	if (tempSound == nullptr)
	{
		shared_ptr<Sounds> sound = make_shared<Sounds>();
		wstring soundPath = RESOURCES_ADDR_SOUND;
		soundPath += L"Character/Enemy/CoreHound/CoreHound_Damaged.mp3";
		sound->Load(soundPath);
		sound->SetVolume(50);
		MANAGER_RESOURCES()->AddResource<Sounds>(L"CoreHound_Damaged", sound);

		_damagedSound = sound->Clone();
	}
	else
	{
		_damagedSound = tempSound->Clone();
	}
}

CoreHoundDamaged::~CoreHoundDamaged()
{
}

void CoreHoundDamaged::Enter(const shared_ptr<AIController>& controller, const wstring& prevTransition)
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
			_animator.lock()->SetNextAnimation(L"Damaged");
		}
		_damagedSound->Play(false);
	}
}

void CoreHoundDamaged::Update()
{
	if (_controller.lock() != nullptr)
	{
		if (_animator.lock()->GetFrameEnd() == true)
		{
			Out(L"CoreHoundBattle");
		}
	}
}

void CoreHoundDamaged::UpdateFromServer()
{
	cout << "server";
}

void CoreHoundDamaged::Out(const wstring& nextTransition)
{
	if (_controller.lock() != nullptr)
	{
		_controller.lock()->SetCurrentFsmStrategy(_name, nextTransition);
	}
}

CoreHoundDead::CoreHoundDead()
{
	_name = L"CoreHoundDead";

	//Death Sound
	auto tempSound1 = MANAGER_RESOURCES()->GetResource<Sounds>(L"CoreHound_Death");
	if (tempSound1 == nullptr)
	{
		shared_ptr<Sounds> sound = make_shared<Sounds>();
		wstring soundPath = RESOURCES_ADDR_SOUND;
		soundPath += L"Character/Enemy/CoreHound/CoreHound_Death.mp3";
		sound->Load(soundPath);
		sound->SetVolume(50);
		MANAGER_RESOURCES()->AddResource<Sounds>(L"CoreHound_Death", sound);

		_deadSound = sound->Clone();
	}
	else
	{
		_deadSound = tempSound1->Clone();
	}
}

CoreHoundDead::~CoreHoundDead()
{
}

void CoreHoundDead::Enter(const shared_ptr<AIController>& controller, const wstring& prevTransition)
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
		_soundFlag = false;
		_controller.lock()->_isAlive = false;
	}
}

void CoreHoundDead::Update()
{
	if (_controller.lock() != nullptr)
	{
		_dt += MANAGER_TIME()->GetDeltaTime();

		if (_animator.lock()->GetFrameEnd() == true)
		{
			_controller.lock()->DeadEvent();
		}

		if (_dt > _soundTimer && _soundFlag == false)
		{
			_deadSound->Play(false);
			_soundFlag = true;
		}
	}
}

void CoreHoundDead::UpdateFromServer()
{
	cout << "server";
}

void CoreHoundDead::Out(const wstring& nextTransition)
{
	if (_controller.lock() != nullptr)
	{
		_controller.lock()->SetCurrentFsmStrategy(_name, nextTransition);
	}
}

CoreHoundTrace::CoreHoundTrace()
{
	_name = L"CoreHoundTrace";
}

CoreHoundTrace::~CoreHoundTrace()
{
}

void CoreHoundTrace::Enter(const shared_ptr<AIController>& controller, const wstring& prevTransition)
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
			_animator.lock()->SetNextAnimation(L"Run");
		}

		_characterInfo = _controller.lock()->GetCharacterInfo();
		_targetList = _controller.lock()->GetTargetList();
		_traceRadius = _characterInfo.lock()->GetDefaultCharacterInfo()._traceRadius;
		_attackRange = _characterInfo.lock()->GetDefaultCharacterInfo()._attackRange;
		_moveSpeed = _characterInfo.lock()->GetDefaultCharacterInfo()._moveSpeed;
	}
}

void CoreHoundTrace::Update()
{
	if (_controller.lock() != nullptr)
	{
		_dt = MANAGER_TIME()->GetDeltaTime();

		//Target Update
		{
			if (_targetList.lock()->size() > 0)
			{
				float minAggro = 0.f;
				shared_ptr<Transform> _lastTarget;
				for (auto& target : *_targetList.lock())
				{
					if (target->Target == _targetTransform.lock()->GetGameObject())
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
					_controller.lock()->SetTargetTransform(_targetTransform.lock());
				}
			}
		}

		bool& isAlive = _targetTransform.lock()->GetGameObject()->GetComponent<CharacterController>()->_isAlive;
		if (isAlive == false)
		{
			Out(L"CoreHoundMoveToSpwanPoint");
		}
		else
		{
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

			//타겟 방향으로 이동 & Attack Range 체크 후 도달 시 Trasition
			{
				Vec3 toTargetTranslate = myPos + (toTargetDir * _moveSpeed * _dt);
				_transform.lock()->SetPosition(toTargetTranslate);

				Vec3 targetPos2 = _targetTransform.lock()->GetLocalPosition();

				float distance = Vec3::Distance(toTargetTranslate, targetPos2);

				if (distance <= _attackRange && distance <= _traceRadius)
				{
					Out(L"CoreHoundBattle");
				}
				else if (distance > _attackRange && distance > _traceRadius)
				{
					Out(L"CoreHoundMoveToSpwanPoint");
				}
			}
		}
	}
}

void CoreHoundTrace::UpdateFromServer()
{
	if (_controller.lock() != nullptr)
	{
		_dt = MANAGER_TIME()->GetDeltaTime();

		if (_targetList.lock()->size() > 0)
		{
			float minAggro = 0.f;
			shared_ptr<Transform> _lastTarget;
			for (auto& target : *_targetList.lock())
			{
				if (target->Target == _targetTransform.lock()->GetGameObject())
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
				_controller.lock()->SetTargetTransform(_targetTransform.lock());
			}
		}
	}
}

void CoreHoundTrace::Out(const wstring& nextTransition)
{
	if (_controller.lock() != nullptr)
	{
		_controller.lock()->SetCurrentFsmStrategy(_name, nextTransition);
	}
}

CoreHoundMoveToSpwanPoint::CoreHoundMoveToSpwanPoint()
{
	_name = L"CoreHoundMoveToSpwanPoint";
}

CoreHoundMoveToSpwanPoint::~CoreHoundMoveToSpwanPoint()
{
}

void CoreHoundMoveToSpwanPoint::Enter(const shared_ptr<AIController>& controller, const wstring& prevTransition)
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
			_animator.lock()->SetNextAnimation(L"Run");
		}

		_characterInfo = _controller.lock()->GetCharacterInfo();
		_spwanPos = _controller.lock()->GetSpawnPosition();
		_moveSpeed = _characterInfo.lock()->GetDefaultCharacterInfo()._moveSpeed;
	}
}

void CoreHoundMoveToSpwanPoint::Update()
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

		if (moveToLength > 2.f + FLT_EPSILON)
		{
			Vec3 toSpwanPosTranslate = myPos + (toTargetDir * _moveSpeed * _dt);
			_transform.lock()->SetLocalPosition(toSpwanPosTranslate);
		}
		else
		{
			_transform.lock()->SetLocalPosition(_spwanPos);
			Out(L"CoreHoundStand");
		}
	}
}

void CoreHoundMoveToSpwanPoint::UpdateFromServer()
{

}

void CoreHoundMoveToSpwanPoint::Out(const wstring& nextTransition)
{
	if (_controller.lock() != nullptr)
	{
		_controller.lock()->SetCurrentFsmStrategy(_name, nextTransition);
	}
}

CoreHoundBattle::CoreHoundBattle()
{
	_name = L"CoreHoundBattle";
}

CoreHoundBattle::~CoreHoundBattle()
{
}

void CoreHoundBattle::Enter(const shared_ptr<AIController>& controller, const wstring& prevTransition)
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
		_targetList = _controller.lock()->GetTargetList();
		_traceRadius = _characterInfo.lock()->GetDefaultCharacterInfo()._traceRadius;
		_attackRange = _characterInfo.lock()->GetDefaultCharacterInfo()._attackRange;
		_attackTime = _characterInfo.lock()->GetDefaultCharacterInfo()._attackTime;
		_traceTime = 0.f;
	}
}

void CoreHoundBattle::Update()
{
	if (_controller.lock() != nullptr)
	{
		_dt = MANAGER_TIME()->GetDeltaTime();
		_traceTime += _dt;

		bool& tempisAlive = _targetTransform.lock()->GetGameObject()->GetComponent<CharacterController>()->_isAlive;

		//Target update
		if (_targetList.lock()->size() <= 0 || tempisAlive == false)
		{
			Out(L"CoreHoundMoveToSpwanPoint");
		}
		else
		{
			float minAggro = 0.f;
			shared_ptr<Transform> _lastTarget;
			for (auto& target : *_targetList.lock())
			{
				if (target->Target == _targetTransform.lock()->GetGameObject())
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
						Out(L"CoreHoundAttack");
					}
				}
			}
			else if (distance > _attackRange && distance <= _traceRadius)
			{
				if (_traceTime + FLT_EPSILON > _traceWaitingTime)
				{
					Out(L"CoreHoundTrace");
				}
			}
			else if (distance > _attackRange && distance > _traceRadius)
			{
				Out(L"CoreHoundMoveToSpwanPoint");
			}
		}
		else
		{
			Out(L"CoreHoundMoveToSpwanPoint");
		}
	}
}

void CoreHoundBattle::UpdateFromServer()
{
	if (_controller.lock() != nullptr)
	{
		_dt = MANAGER_TIME()->GetDeltaTime();
		_traceTime += _dt;

		bool& tempisAlive = _targetTransform.lock()->GetGameObject()->GetComponent<CharacterController>()->_isAlive;

		//Target update
		if (_targetList.lock()->size() <= 0 || tempisAlive == false)
		{
			cout << "true";
		}
		else
		{
			float minAggro = 0.f;
			shared_ptr<Transform> _lastTarget;
			for (auto& target : *_targetList.lock())
			{
				if (target->Target == _targetTransform.lock()->GetGameObject())
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
				_controller.lock()->SetTargetTransform(_targetTransform.lock());
			}
		}
	}
}

void CoreHoundBattle::Out(const wstring& nextTransition)
{
	if (_controller.lock() != nullptr)
	{
		_controller.lock()->SetCurrentFsmStrategy(_name, nextTransition);
	}
}

CoreHoundAttack::CoreHoundAttack()
{
	_name = L"CoreHoundAttack";

	//Attack1 Sound
	auto tempSound1 = MANAGER_RESOURCES()->GetResource<Sounds>(L"CoreHound_Attack1");
	if (tempSound1 == nullptr)
	{
		shared_ptr<Sounds> sound = make_shared<Sounds>();
		wstring soundPath = RESOURCES_ADDR_SOUND;
		soundPath += L"Character/Enemy/CoreHound/CoreHound_Attack1.mp3";
		sound->Load(soundPath);
		sound->SetVolume(100);
		MANAGER_RESOURCES()->AddResource<Sounds>(L"CoreHound_Attack1", sound);

		_attack1Sound = sound->Clone();
	}
	else
	{
		_attack1Sound = tempSound1->Clone();
	}

	//Attack2 Sound
	auto tmepSound2 = MANAGER_RESOURCES()->GetResource<Sounds>(L"CoreHound_Attack2");
	if (tmepSound2 == nullptr)
	{
		shared_ptr<Sounds> sound = make_shared<Sounds>();
		wstring soundPath = RESOURCES_ADDR_SOUND;
		soundPath += L"Character/Enemy/CoreHound/CoreHound_Attack2.mp3";
		sound->Load(soundPath);
		sound->SetVolume(100);
		MANAGER_RESOURCES()->AddResource<Sounds>(L"CoreHound_Attack2", sound);

		_attack2Sound = sound->Clone();
	}
	else
	{
		_attack2Sound = tmepSound2->Clone();
	}
}

CoreHoundAttack::~CoreHoundAttack()
{
}

void CoreHoundAttack::Enter(const shared_ptr<AIController>& controller, const wstring& prevTransition)
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

			
			shared_ptr<CharacterController> targetCon = _targetTransform.lock()->GetGameObject()->GetComponent<CharacterController>();

			if (targetCon != nullptr)
			{
				float attackDamage = _characterInfo.lock()->GetCharacterInfo()._atk;
				targetCon->TakeDamage(_transform.lock()->GetGameObject(), attackDamage);
			}

			int randAttack = rand() % 2;

			if (randAttack == 0)
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

void CoreHoundAttack::Update()
{
	if (_controller.lock() != nullptr)
	{
		_dt = MANAGER_TIME()->GetDeltaTime();

		if (_animator.lock()->GetFrameEnd() == true)
		{
			Out(L"CoreHoundBattle");
		}

		if (_targetTransform.lock()->GetGameObject()->GetComponent<CharacterController>()->_isAlive)
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
		}
	}
}

void CoreHoundAttack::UpdateFromServer()
{
	cout << "server";
}

void CoreHoundAttack::Out(const wstring& nextTransition)
{
	if (_controller.lock() != nullptr)
	{
		_controller.lock()->SetCurrentFsmStrategy(_name, nextTransition);
	}
}
