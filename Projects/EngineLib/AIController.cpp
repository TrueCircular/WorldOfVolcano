#include "pch.h"
#include "AIController.h"
#include "HeightGetter.h"
#include "CharacterInfo.h"
#include "UnitFSM.h"

void AIController::InitAnimState()
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

bool AIController::SetAnimState(const EnemyAnimType& type)
{
	if (type == EnemyAnimType::None)
	{
		return false;
	}
	else
	{
		_currentEnemyAnimState->Out();
		_currentEnemyAnimState = _enemyAnimStateList[static_cast<int>(type)];
		if (_currentEnemyAnimState)
		{
			_currentEnemyAnimState->Enter(shared_from_this());
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

	switch (_type)
	{
	case AIType::PlayableUnit:
	{
		_currentPlayerState = make_shared<PlayerUnitState>();
		*_currentPlayerState = PlayerUnitState::Stand;
	}break;
	case AIType::EnemyUnit:
	{
		_currentEnemyState = make_shared<EnemyUnitState>();
		*_currentEnemyState = EnemyUnitState::Stand;
	}break;
	}

	InitAnimState();
}

void AIController::FixedUpdate()
{
	if (_heightGetterCom.lock())
	{
		Vec3 tempPos = _transform.lock()->GetLocalPosition();
		tempPos.y = _heightGetterCom.lock()->GetHeight();
		_transform.lock()->SetLocalPosition(tempPos);
	}
}

void AIController::Update()
{
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

	if (_aiSound)
		_aiSound->PlaySound(GetCurrentPlayerAnimType());

	if (_enemySound)
		_enemySound->PlaySound(GetCurrentEnemyAnimType());
}

void AIController::LateUpdate()
{
}
