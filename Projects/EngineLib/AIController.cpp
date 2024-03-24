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
		_unitFsm->SetStrategy(_unitStrategyList[0]);
		_unitFsm->Enter(shared_from_this(), _unitStrategyList[0]->_name);
	}break;
	}
}

void AIController::SetCurrentFsmStrategy(const wstring& preTransition, const wstring& nextTransition)
{
	if (_unitStrategyList.size() <= 0)
	{
		return;
	}
	else
	{
		for (const auto& strategy : _unitStrategyList)
		{
			if (strategy->_name == nextTransition)
			{
				_unitFsm->SetStrategy(strategy);
				_unitFsm->Enter(shared_from_this(), preTransition);
			}
		}
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

void AIController::TakeDamage(const shared_ptr<GameObject>& sender, float damage, float aggroValue)
{
	if (_characterInfo.lock() == nullptr)
		return;

	//Aggro Calculte
	{
		for (auto target : _targetList)
		{
			if (sender == target.Target)
			{
				target.AggroValue += aggroValue;
			}
			else
			{
				TargetDesc temp;
				temp.Target = sender;
				temp.AggroValue = aggroValue;
				_targetList.insert(temp);
			}
		}
	}

	//Damage Calculate
	{
		auto myInfo = _characterInfo.lock()->GetCharacterInfo();
		float defEff= pow(myInfo._def* log(2), 0.5) * 3;
		float calDamage = damage * (1 - defEff/100);
		float finalHp = myInfo._hp - calDamage;

		if (finalHp < 1.f + FLT_EPSILON)
		{
			finalHp = 0;
			myInfo._hp = (uint32)finalHp;
			_characterInfo.lock()->SetCharacterInfo(myInfo);
			for (auto& st : _unitStrategyList)
			{
				if (st->_type == UnitStrategyType::Dead)
				{
					SetCurrentFsmStrategy(_unitFsm->GetStrategyName(), st->_name);
				}
			}
		}
		else
		{
			myInfo._hp = (uint32)finalHp;
			_characterInfo.lock()->SetCharacterInfo(myInfo);
		}
	}

	if (_type == AIType::PlayableUnit)
	{
		if (_currentPlayerAnimState->GetStateAnimtype() != PlayerAnimType::Attack1 ||
			_currentPlayerAnimState->GetStateAnimtype() != PlayerAnimType::Attack2 ||
			_currentPlayerAnimState->GetStateAnimtype() != PlayerAnimType::Ability1 ||
			_currentPlayerAnimState->GetStateAnimtype() != PlayerAnimType::Ability2 ||
			_currentPlayerAnimState->GetStateAnimtype() != PlayerAnimType::JumpStart ||
			_currentPlayerAnimState->GetStateAnimtype() != PlayerAnimType::JumpFall ||
			_currentPlayerAnimState->GetStateAnimtype() != PlayerAnimType::JumpEnd ||
			_currentPlayerAnimState->GetStateAnimtype() != PlayerAnimType::Death ||
			_currentPlayerAnimState->GetStateAnimtype() != PlayerAnimType::Casting ||
			_currentPlayerAnimState->GetStateAnimtype() != PlayerAnimType::Damaged)
		{
			*_currentPlayerState = PlayerUnitState::Damaged;
		}
	}
	else if (_type == AIType::EnemyUnit)
	{
		for (auto& st : _unitStrategyList)
		{
			if (st->_type == UnitStrategyType::Damaged)
			{
				if (_unitFsm->GetStrategy()->_type != UnitStrategyType::Damaged &&
					_unitFsm->GetStrategy()->_type != UnitStrategyType::Stun &&
					_unitFsm->GetStrategy()->_type != UnitStrategyType::Dead &&
					_unitFsm->GetStrategy()->_type != UnitStrategyType::Event &&
					_unitFsm->GetStrategy()->_type != UnitStrategyType::Attack &&
					_unitFsm->GetStrategy()->_type != UnitStrategyType::Ability)
				{
					SetCurrentFsmStrategy(_unitFsm->GetStrategyName(), st->_name);
				}
			}
		}
	}
}

void AIController::DeadEvent()
{
	GetGameObject()->SetActive(false);
}

void AIController::SearchTarget()
{
}

void AIController::Start()
{
	{
		_transform = GetGameObject()->GetTransform();
		_animator = GetGameObject()->GetChildByName(L"Model")->GetModelAnimator();
		_jumpState = make_shared<JumpFlag>();
		_characterInfo = GetGameObject()->GetComponent<CharacterInfo>();
		_unitFsm = make_shared<UnitFSM>();
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
	if (_isAlive == false)
	{
		DeadEvent();
	}
	else
	{
		if (_heightGetterCom.lock())
		{
			Vec3 tempPos = _transform.lock()->GetLocalPosition();
			tempPos.y = _heightGetterCom.lock()->GetHeight();
			_transform.lock()->SetLocalPosition(tempPos);
		}

		if (_jumpState->isJump == false)
		{
			if (_heightGetterCom.lock())
			{
				Vec3 tempPos = _transform.lock()->GetLocalPosition();
				tempPos.y = _heightGetterCom.lock()->GetHeight();
				_transform.lock()->SetLocalPosition(tempPos);
			}
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
			_unitFsm->Update();
		}
		break;
		}
	}
}

void AIController::Update()
{

}
void AIController::LateUpdate()
{
}
