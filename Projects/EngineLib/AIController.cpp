#include "pch.h"
#include "AIController.h"
#include "HeightGetter.h"
#include "CharacterInfo.h"
#include "UnitFSM.h"
#include "PlayableUnit.h"

void AIController::InitState()
{
	switch (_type)
	{
	case AIType::PlayableUnit:
	{
		//Idle
		_playerAnimStateList.push_back(make_shared<PlayerAnimIdle>());
		//FrontWalk
		_playerAnimStateList.push_back(make_shared<PlayerAnimFrontWalk>());
		//BackWalk
		_playerAnimStateList.push_back(make_shared<PlayerAnimBackWalk>());
		//FrontRun
		_playerAnimStateList.push_back(make_shared<PlayerAnimFrontRun>());
		//BackRun
		_playerAnimStateList.push_back(make_shared<PlayerAnimBackRun>());
		//JumpStart
		_playerAnimStateList.push_back(make_shared<PlayerAnimJumpStart>());
		//JumpFall
		_playerAnimStateList.push_back(make_shared<PlayerAnimJumpFall>());
		//JumpEnd
		_playerAnimStateList.push_back(make_shared<PlayerAnimJumpEnd>());
		//Stun
		_playerAnimStateList.push_back(make_shared<PlayerAnimStun>());
		//Loot
		_playerAnimStateList.push_back(make_shared<PlayerAnimLoot>());
		//Damaged
		_playerAnimStateList.push_back(make_shared<PlayerAnimDamaged>());
		//Death
		_playerAnimStateList.push_back(make_shared<PlayerAnimDeath>());
		//Battle
		_playerAnimStateList.push_back(make_shared<PlayerAnimBattle>());
		//Attack1
		_playerAnimStateList.push_back(make_shared<PlayerAnimAttack1>());
		//Attack2
		_playerAnimStateList.push_back(make_shared<PlayerAnimAttack2>());
		//Casting
		_playerAnimStateList.push_back(make_shared<PlayerAnimCasting>());
		//Ability1
		_playerAnimStateList.push_back(make_shared<PlayerAnimAbility1>());
		//Ability2
		_playerAnimStateList.push_back(make_shared<PlayerAnimAbility2>());

		_currentPlayerState = make_shared<PlayerUnitState>();
		*_currentPlayerState = PlayerUnitState::Stand;

		_currentPlayerAnimState = _playerAnimStateList[0];
		_currentPlayerAnimState->Enter(shared_from_this());
	}break;
	case AIType::EnemyUnit:
	{
		//Stand 0
		_fsmList.push_back(make_shared<UnitFSMStand>());
		//Trace 1
		_fsmList.push_back(make_shared<UnitFSMTrace>());
		//MoveToSpwanPoint
		_fsmList.push_back(make_shared<UnitFSMMoveToSpwanPoint>());
		//Battle
		_fsmList.push_back(make_shared<UnitFSMBattle>());
		//Attack
		_fsmList.push_back(make_shared<UnitFSMAttack>());
		//Ability1
		_fsmList.push_back(make_shared<UnitFSMAbility1>());
		//Ability2
		_fsmList.push_back(make_shared<UnitFSMAbility2>());

		_currentFsmState = _fsmList[0];
		_currentFsmState->Enter(shared_from_this());
	}break;
	}
}

void AIController::SetCurrentFsmState(UnitFSMState state)
{
	switch (state)
	{
	case UnitFSMState::Stand:
	{
		_currentFsmState = _fsmList[0];
		_currentFsmState->Enter(shared_from_this());
	}break;
	case UnitFSMState::Trace:
	{
		_currentFsmState = _fsmList[1];
		_currentFsmState->Enter(shared_from_this());
	}break;	
	case UnitFSMState::MoveToSpwanPoint:
	{
		_currentFsmState = _fsmList[2];
		_currentFsmState->Enter(shared_from_this());
	}break;	
	case UnitFSMState::Battle:
	{
		_currentFsmState = _fsmList[3];
		_currentFsmState->Enter(shared_from_this());
	}break;
	case UnitFSMState::Attack:
	{
		_currentFsmState = _fsmList[4];
		_currentFsmState->Enter(shared_from_this());
	}break;	
	case UnitFSMState::Ability1:
	{
		_currentFsmState = _fsmList[5];
		_currentFsmState->Enter(shared_from_this());
	}break;
	case UnitFSMState::Ability2:
	{
		_currentFsmState = _fsmList[6];
		_currentFsmState->Enter(shared_from_this());
	}break;
	}
}

bool AIController::SetAnimState(const PlayerAnimType& type)
{
	if (type == PlayerAnimType::None)
	{
		return false;
	}
	else
	{
		_currentPlayerAnimState->Out();
		_currentPlayerAnimState = _playerAnimStateList[static_cast<int>(type)];

		if (_currentPlayerAnimState)
		{
			_currentPlayerAnimState->Enter(shared_from_this());
			return true;
		}

	}

	return false;
}

void AIController::TakeDamage(const shared_ptr<GameObject>& sender, uint16 damage)
{
	auto characterInfo = sender->GetComponent<CharacterInfo>();

	if (characterInfo != nullptr)
	{
		//characterInfo->GetAddCharacterInfo

	}

}

void AIController::SearchTraceTarget()
{
	_targetList = MANAGER_SCENE()->GetCurrentScene()->GetPlayableUnit();

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
				if (minDistance <= _traceRadius)
				{
					SetTargetTransform(FinalTarget->GetTransform());
				}
			}
		}
	}
}

void AIController::Start()
{
	{
		_transform = GetGameObject()->GetTransform();
		_animator = GetGameObject()->GetChildByName(L"Model")->GetModelAnimator();
		_jumpState = make_shared<JumpFlag>();
		_characterInfo = GetGameObject()->GetComponent<CharacterInfo>();
	}

	_heightGetterCom = GetGameObject()->GetComponent<HeightGetter>();
	{
		if (_transform.lock())
		{
			Vec3 temPos = _transform.lock()->GetLocalPosition();
			temPos.y = _heightGetterCom.lock()->GetHeight();
			_transform.lock()->SetLocalPosition(temPos);
		}
	}

	InitState();
}

void AIController::FixedUpdate()
{

}

void AIController::Update()
{
	if (_heightGetterCom.lock())
	{
		Vec3 tempPos = _transform.lock()->GetLocalPosition();
		tempPos.y = _heightGetterCom.lock()->GetHeight();
		_transform.lock()->SetLocalPosition(tempPos);
	}

	switch (_type)
	{
	case AIType::PlayableUnit:
	{
		_currentPlayerAnimState->Update();
	}
	break;
	case AIType::EnemyUnit:
	{
		_currentFsmState->Update();
	}
	break;
	}
}

void AIController::LateUpdate()
{
}
