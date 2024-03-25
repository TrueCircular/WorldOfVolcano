#include "pch.h"
#include "BaronGeddonPattern.h"

#include "PlayerController.h"
#include "AIController.h"
#include "CharacterInfo.h"
#include "Sounds.h"

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
		_spwanPos = _controller.lock()->GetSpawnPosition();
	}
}

void BaronGeddonStand::Update()
{
	if (_controller.lock() != nullptr)
	{
		Vec3 myPos = _transform.lock()->GetLocalPosition();
		_spwanPos.y = myPos.y;

		float dist = Vec3::Distance(myPos, _spwanPos);

		if (dist > 5.f + FLT_EPSILON)
		{
			Out(L"BaronGeddonMoveToSpwanPoint");
		}
	}
}

void BaronGeddonStand::Out(const wstring& nextTransition)
{
	if (_controller.lock() != nullptr)
	{
		_controller.lock()->SetCurrentFsmStrategy(_name, nextTransition);
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

void BaronGeddonDamaged::Update()
{
	if (_controller.lock() != nullptr)
	{
		if (_animator.lock()->GetFrameEnd() == true)
		{
			Out(L"BaronGeddonBattle");
		}
	}
}

void BaronGeddonDamaged::Out(const wstring& nextTransition)
{
	if (_controller.lock() != nullptr)
	{
		_controller.lock()->SetCurrentFsmStrategy(_name, nextTransition);
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

void BaronGeddonDead::Update()
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

void BaronGeddonDead::Out(const wstring& nextTransition)
{
	if (_controller.lock() != nullptr)
	{
		_controller.lock()->SetCurrentFsmStrategy(_name, nextTransition);
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

void BaronGeddonTrace::Update()
{
	if (_controller.lock() != nullptr)
	{
		_dt = MANAGER_TIME()->GetDeltaTime();

		//Target Update
		{
			if (_targetList.lock()->size() > 0)
			{
				float minAggro = -1.f;

				shared_ptr<Transform> _lastTarget;
				for (auto& target : *_targetList.lock())
				{
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
			Out(L"BaronGeddonMoveToSpwanPoint");
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

void BaronGeddonTrace::Out(const wstring& nextTransition)
{
	if (_controller.lock() != nullptr)
	{
		_controller.lock()->SetCurrentFsmStrategy(_name, nextTransition);
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

void BaronGeddonMoveToSpwanPoint::Update()
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
			Out(L"BaronGeddonStand");
		}
	}
}

void BaronGeddonMoveToSpwanPoint::Out(const wstring& nextTransition)
{
	if (_controller.lock() != nullptr)
	{
		_controller.lock()->SetCurrentFsmStrategy(_name, nextTransition);
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

		if (_controller.lock()->GetTransform() != nullptr)
			_transform = _controller.lock()->GetTransform();

		if (_controller.lock()->GetTargetTransform() != nullptr)
			_targetTransform = _controller.lock()->GetTargetTransform();

		if (_controller.lock()->GetAnimator() != nullptr)
			_animator = _controller.lock()->GetAnimator();

		if (_animator.lock() != nullptr)
		{
			_animator.lock()->SetFrameEnd(false);
			_animator.lock()->SetNextAnimation(L"Stand");
		}

		_characterInfo = _controller.lock()->GetCharacterInfo();
		_targetList = _controller.lock()->GetTargetList();
		_traceRadius = _characterInfo.lock()->GetDefaultCharacterInfo()._traceRadius;
		_attackRange = _characterInfo.lock()->GetDefaultCharacterInfo()._attackRange;
		_attackTime = _characterInfo.lock()->GetDefaultCharacterInfo()._attackTime;
		_traceTime = 0.f;
	}
}

void BaronGeddonBattle::Update()
{
	if (_controller.lock() != nullptr)
	{
		_dt = MANAGER_TIME()->GetDeltaTime();
		_traceTime += _dt;

		bool& tempisAlive = _targetTransform.lock()->GetGameObject()->GetComponent<CharacterController>()->_isAlive;

		//Target update
		if (_targetList.lock()->size() <= 0 || tempisAlive == false)
		{
			Out(L"BaronGeddonMoveToSpwanPoint");
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

			toTargetDir.Normalize(toTargetDir);
			float distance = Vec3::Distance(myPos, targetPos);

			if (distance <= _attackRange)
			{
				_attackTimeCal += _dt;

				//Ability Transition
				{

				}

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

void BaronGeddonBattle::Out(const wstring& nextTransition)
{
	if (_controller.lock() != nullptr)
	{
		_controller.lock()->SetCurrentFsmStrategy(_name, nextTransition);
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

			auto targetCon = _targetTransform.lock()->GetGameObject()->GetComponent<CharacterController>();

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

void BaronGeddonAttack::Update()
{
	if (_controller.lock() != nullptr)
	{
		_dt = MANAGER_TIME()->GetDeltaTime();

		if (_animator.lock()->GetFrameEnd() == true)
		{
			Out(L"BaronGeddonBattle");
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

void BaronGeddonAttack::Out(const wstring& nextTransition)
{
	if (_controller.lock() != nullptr)
	{
		_controller.lock()->SetCurrentFsmStrategy(_name, nextTransition);
	}
}

BaronGeddonAbility::BaronGeddonAbility()
{
	_name = L"BaronGeddonAbility";
}

BaronGeddonAbility::~BaronGeddonAbility()
{
}

void BaronGeddonAbility::Enter(const shared_ptr<AIController>& controller, const wstring& prevTransition)
{
}

void BaronGeddonAbility::Update()
{
}

void BaronGeddonAbility::Out(const wstring& nextTransition)
{
}
