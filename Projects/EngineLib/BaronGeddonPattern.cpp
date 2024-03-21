#include "pch.h"
#include "BaronGeddonPattern.h"

#include <stdlib.h>
#include <time.h>
#include "PlayableUnit.h"
#include "CharacterInfo.h"
#include "Sounds.h"
#include "AIController.h"

BaronGeddonStand::BaronGeddonStand()
{
	_name = L"BaronGeddonStand";
}

BaronGeddonStand::~BaronGeddonStand()
{
}

void BaronGeddonStand::Enter(const shared_ptr<AIController>& controller)
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

void BaronGeddonStand::Update()
{
	//���� Scene�� �����ϴ� Ÿ�� ��� ĳ��
	_targetList = MANAGER_SCENE()->GetCurrentScene()->GetPlayableUnit();

	if (_controller.lock() != nullptr)
	{
		if (_targetList.size() > 0)
		{
			//Taget �ĺ� ����
			map<float, shared_ptr<PlayableUnit>> ToTargetList;

			for (const auto& target : _targetList)
			{
				Vec3 myPos = _transform.lock()->GetLocalPosition();
				Vec3 targetPos = target->GetTransform()->GetLocalPosition();
				float Length = Vec3::Distance(myPos, targetPos);

				//�ڽ��� ��ġ�� Ÿ�� ��ġ�� �����Ÿ� �ȿ� ���� �� ��� Ž��
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

				//���� Ÿ�� ���
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
					if (minDistance <= _attackRange && minDistance <= _traceRadius)
					{
						_controller.lock()->SetTargetTransform(FinalTarget->GetTransform());
						Out(L"BaronGeddonBattle");
					}
					else if (minDistance > _attackRange && minDistance <= _traceRadius)
					{
						_controller.lock()->SetTargetTransform(FinalTarget->GetTransform());
						Out(L"BaronGeddonTrace");
					}
				}
			}
		}
	}
}

void BaronGeddonStand::Out(const wstring& transition)
{
	if (_controller.lock() != nullptr)
	{
		_controller.lock()->SetCurrentFsmStrategy(transition);
	}
}

BaronGeddonDamaged::BaronGeddonDamaged()
{
	_name = L"BaronGeddonDamaged";
}

BaronGeddonDamaged::~BaronGeddonDamaged()
{
}

void BaronGeddonDamaged::Enter(const shared_ptr<AIController>& controller)
{
}

void BaronGeddonDamaged::Update()
{
}

void BaronGeddonDamaged::Out(const wstring& transition)
{

}

BaronGeddonStun::BaronGeddonStun()
{
	_name = L"BaronGeddonStun";
}

BaronGeddonStun::~BaronGeddonStun()
{
}

void BaronGeddonStun::Enter(const shared_ptr<AIController>& controller)
{
}

void BaronGeddonStun::Update()
{
}

void BaronGeddonStun::Out(const wstring& transition)
{
}

BaronGeddonDead::BaronGeddonDead()
{
	_name = L"BaronGeddonDead";
}

BaronGeddonDead::~BaronGeddonDead()
{
}

void BaronGeddonDead::Enter(const shared_ptr<AIController>& controller)
{
}

void BaronGeddonDead::Update()
{
}

void BaronGeddonDead::Out(const wstring& transition)
{
}

BaronGeddonTrace::BaronGeddonTrace()
{
	_name = L"BaronGeddonTrace";
}

BaronGeddonTrace::~BaronGeddonTrace()
{
}

void BaronGeddonTrace::Enter(const shared_ptr<AIController>& controller)
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

void BaronGeddonTrace::Update()
{
	if (_controller.lock() != nullptr)
	{
		_dt = MANAGER_TIME()->GetDeltaTime();

		//�ڽ��� ��ġ���� Ÿ�ٹ������� ���ϴ� ����ȭ �� ���� ���� ���(Normal Vector)
		Vec3 myPos = _transform.lock()->GetLocalPosition();
		Vec3 targetPos = _targetTransform.lock()->GetPosition();
		targetPos.y = myPos.y;
		Vec3 toTargetDir = targetPos - myPos;

		//Ÿ�� �������� ȸ��
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

		//Ÿ�� �������� �̵� & Attack Range üũ �� ���� �� Trasition
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

void BaronGeddonTrace::Out(const wstring& transition)
{
	if (_controller.lock() != nullptr)
	{
		_controller.lock()->SetCurrentFsmStrategy(transition);
	}
}

BaronGeddonMoveToSpwanPoint::BaronGeddonMoveToSpwanPoint()
{
	_name = L"BaronGeddonMoveToSpwanPoint";
}

BaronGeddonMoveToSpwanPoint::~BaronGeddonMoveToSpwanPoint()
{
}

void BaronGeddonMoveToSpwanPoint::Enter(const shared_ptr<AIController>& controller)
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

void BaronGeddonMoveToSpwanPoint::Update()
{
	if (_controller.lock() != nullptr)
	{
		_dt = MANAGER_TIME()->GetDeltaTime();

		Vec3 myPos = _transform.lock()->GetLocalPosition();
		_spwanPos.y = myPos.y;
		Vec3 toTargetDir = _spwanPos - myPos;

		//Ÿ�� �������� ȸ��
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
			Out(L"BaronGeddonStand");
		}
	}
}

void BaronGeddonMoveToSpwanPoint::Out(const wstring& transition)
{
	if (_controller.lock() != nullptr)
	{
		_controller.lock()->SetCurrentFsmStrategy(transition);
	}
}

BaronGeddonBattle::BaronGeddonBattle()
{
	_name = L"BaronGeddonBattle";
}

BaronGeddonBattle::~BaronGeddonBattle()
{
}

void BaronGeddonBattle::Enter(const shared_ptr<AIController>& controller)
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

void BaronGeddonBattle::Update()
{
	if (_controller.lock() != nullptr)
	{
		_dt = MANAGER_TIME()->GetDeltaTime();
		_traceTime += _dt;

		Vec3 myPos = _transform.lock()->GetLocalPosition();
		Vec3 targetPos = _targetTransform.lock()->GetLocalPosition();
		targetPos.y = myPos.y;
		Vec3 toTargetDir = targetPos - myPos;

		//Ÿ�� �������� ȸ��
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

			//if �б� ���� �ʿ�


			//Ability Transition
			{

			}

			//Attack Transition
			{
				if (_attackTimeCal >= _attackTime)
				{
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
}

void BaronGeddonBattle::Out(const wstring& transition)
{
	if (_controller.lock() != nullptr)
	{
		_controller.lock()->SetCurrentFsmStrategy(transition);
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

BaronGeddonAttack::~BaronGeddonAttack()
{
}

void BaronGeddonAttack::Enter(const shared_ptr<AIController>& controller)
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

void BaronGeddonAttack::Update()
{
	if (_controller.lock() != nullptr)
	{
		if (_animator.lock()->GetFrameEnd() == true)
		{
			Out(L"BaronGeddonBattle");
		}

		_dt = MANAGER_TIME()->GetDeltaTime();

		Vec3 myPos = _transform.lock()->GetLocalPosition();
		Vec3 targetPos = _targetTransform.lock()->GetLocalPosition();
		targetPos.y = myPos.y;
		Vec3 toTargetDir = targetPos - myPos;

		//Ÿ�� �������� ȸ��
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

void BaronGeddonAttack::Out(const wstring& transition)
{
	if (_controller.lock() != nullptr)
	{
		_controller.lock()->SetCurrentFsmStrategy(transition);
	}
}

BaronGeddonAbility::BaronGeddonAbility()
{
	_name = L"BaronGeddonAbility";
}

BaronGeddonAbility::~BaronGeddonAbility()
{
}

void BaronGeddonAbility::Enter(const shared_ptr<AIController>& controller)
{
}

void BaronGeddonAbility::Update()
{
}

void BaronGeddonAbility::Out(const wstring& transition)
{
}
