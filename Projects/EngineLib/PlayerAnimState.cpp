#include "pch.h"
#include "PlayerAnimState.h"
#include "CharacterController.h"
#include "PlayerController.h"
#include "AIController.h"
#include "stdlib.h"
#include "time.h"
#include <float.h>
#include "Sounds.h"

bool PlayerAnimState::Update()
{
	if (_contoller.lock())
	{
		switch (*_playerState.lock())
		{
		case PlayerUnitState::Stand:
		{
			_contoller.lock()->SetAnimState(PlayerAnimType::Stand);
			return true;
		}break;
		case PlayerUnitState::FrontMove:
		case PlayerUnitState::FrontRightMove:
		case PlayerUnitState::FrontLeftMove:
		case PlayerUnitState::RightMove:
		case PlayerUnitState::LeftMove:
		{
			_contoller.lock()->SetAnimState(PlayerAnimType::FrontRun);
			return true;
		}break;
		case PlayerUnitState::BackMove:
		case PlayerUnitState::BackRightMove:
		case PlayerUnitState::BackLeftMove:
		{
			_contoller.lock()->SetAnimState(PlayerAnimType::BackRun);
			return true;
		}break;
		case PlayerUnitState::Jump:
		{
			_contoller.lock()->SetAnimState(PlayerAnimType::JumpStart);
			return true;
		}break;
		case PlayerUnitState::Stun:
		{
			_contoller.lock()->SetAnimState(PlayerAnimType::Stun);
			return true;
		}break;
		case PlayerUnitState::Loot:
		{
			_contoller.lock()->SetAnimState(PlayerAnimType::Loot);
			return true;
		}break;
		case PlayerUnitState::Battle:
		{
			_contoller.lock()->SetAnimState(PlayerAnimType::Battle);
			return true;
		}break;
		}
	}
	else if (_aiContoller.lock())
	{
		switch (*_playerState.lock())
		{
		case PlayerUnitState::Stand:
		{
			_aiContoller.lock()->SetAnimState(PlayerAnimType::Stand);
			return true;
		}break;
		case PlayerUnitState::FrontMove:
		case PlayerUnitState::FrontRightMove:
		case PlayerUnitState::FrontLeftMove:
		case PlayerUnitState::RightMove:
		case PlayerUnitState::LeftMove:
		{
			_aiContoller.lock()->SetAnimState(PlayerAnimType::FrontRun);
			return true;
		}break;
		case PlayerUnitState::BackMove:
		case PlayerUnitState::BackRightMove:
		case PlayerUnitState::BackLeftMove:
		{
			_aiContoller.lock()->SetAnimState(PlayerAnimType::BackRun);
			return true;
		}break;
		case PlayerUnitState::Jump:
		{
			_aiContoller.lock()->SetAnimState(PlayerAnimType::JumpStart);
			return true;
		}break;
		case PlayerUnitState::Stun:
		{
			_aiContoller.lock()->SetAnimState(PlayerAnimType::Stun);
			return true;
		}break;
		case PlayerUnitState::Loot:
		{
			_aiContoller.lock()->SetAnimState(PlayerAnimType::Loot);
			return true;
		}break;
		case PlayerUnitState::Battle:
		{
			_aiContoller.lock()->SetAnimState(PlayerAnimType::Battle);
			return true;
		}break;
		}
	}

	return false;
}


bool PlayerAnimIdle::Enter(const shared_ptr<CharacterController>& playerController)
{
	if (dynamic_pointer_cast<PlayerController>(playerController))
	{
		_contoller = dynamic_pointer_cast<PlayerController>(playerController);
		_animator = _contoller.lock()->GetAnimator();
		_playerState = _contoller.lock()->GetCurrentUnitState();
	}
	else
	{
		_aiContoller = dynamic_pointer_cast<AIController>(playerController);
		_animator = _aiContoller.lock()->GetAnimator();
		_playerState = _aiContoller.lock()->GetCurrentPlayerUnitState();
	}

	if (_animator.lock()->GetFrameEnd() == true)
	{
		_animator.lock()->SetFrameEnd(false);
	}
	_animator.lock()->SetNextAnimation(L"Stand");

	return true;
}

bool PlayerAnimIdle::Update()
{
	if (_playerState.lock() != nullptr)
	{
		if (*_playerState.lock() != PlayerUnitState::Stand)
		{
			Super::Update();
		}
	}

	return false;
}

bool PlayerAnimIdle::Out()
{
	return false;
}

bool PlayerAnimFrontWalk::Enter(const shared_ptr<CharacterController>& playerController)
{
	if (dynamic_pointer_cast<PlayerController>(playerController))
	{
		_contoller = dynamic_pointer_cast<PlayerController>(playerController);
		_animator = _contoller.lock()->GetAnimator();
		_playerState = _contoller.lock()->GetCurrentUnitState();
	}
	else
	{
		_aiContoller = dynamic_pointer_cast<AIController>(playerController);
		_animator = _aiContoller.lock()->GetAnimator();
		_playerState = _aiContoller.lock()->GetCurrentPlayerUnitState();
	}

	if (_animator.lock()->GetFrameEnd() == true)
	{
		_animator.lock()->SetFrameEnd(false);
	}
	_animator.lock()->SetNextAnimation(L"Walk");

	return true;
}

bool PlayerAnimFrontWalk::Update()
{
	if (_playerState.lock() != nullptr)
	{
		if (*_playerState.lock() != PlayerUnitState::FrontMove ||
			*_playerState.lock() != PlayerUnitState::FrontLeftMove ||
			*_playerState.lock() != PlayerUnitState::FrontRightMove)
		{
			Super::Update();
		}
	}

	return false;
}

bool PlayerAnimFrontWalk::Out()
{
	return false;
}

bool PlayerAnimBackWalk::Enter(const shared_ptr<CharacterController>& playerController)
{
	if (dynamic_pointer_cast<PlayerController>(playerController))
	{
		_contoller = dynamic_pointer_cast<PlayerController>(playerController);
		_animator = _contoller.lock()->GetAnimator();
		_playerState = _contoller.lock()->GetCurrentUnitState();
	}
	else
	{
		_aiContoller = dynamic_pointer_cast<AIController>(playerController);
		_animator = _aiContoller.lock()->GetAnimator();
		_playerState = _aiContoller.lock()->GetCurrentPlayerUnitState();
	}

	if (_animator.lock()->GetFrameEnd() == true)
	{
		_animator.lock()->SetFrameEnd(false);
	}
	_animator.lock()->SetNextAnimation(L"BackWalk");

	return true;
}

bool PlayerAnimBackWalk::Update()
{
	if (_playerState.lock() != nullptr)
	{
		if (*_playerState.lock() != PlayerUnitState::BackMove)
		{
			Super::Update();
		}
	}

	return false;
}

bool PlayerAnimBackWalk::Out()
{
	return false;
}

bool PlayerAnimFrontRun::Enter(const shared_ptr<CharacterController>& playerController)
{
	if (dynamic_pointer_cast<PlayerController>(playerController))
	{
		_contoller = dynamic_pointer_cast<PlayerController>(playerController);
		_animator = _contoller.lock()->GetAnimator();
		_playerState = _contoller.lock()->GetCurrentUnitState();
	}
	else
	{
		_aiContoller = dynamic_pointer_cast<AIController>(playerController);
		_animator = _aiContoller.lock()->GetAnimator();
		_playerState = _aiContoller.lock()->GetCurrentPlayerUnitState();
	}

	if (_animator.lock()->GetFrameEnd() == true)
	{
		_animator.lock()->SetFrameEnd(false);
	}
	_animator.lock()->SetNextAnimation(L"FrontRun");

	return true;
}

bool PlayerAnimFrontRun::Update()
{
	if (_playerState.lock() != nullptr)
	{
		if (*_playerState.lock() != PlayerUnitState::FrontMove &&
			*_playerState.lock() != PlayerUnitState::FrontLeftMove &&
			*_playerState.lock() != PlayerUnitState::FrontRightMove &&
			*_playerState.lock() != PlayerUnitState::RightMove &&
			*_playerState.lock() != PlayerUnitState::LeftMove)
		{
			Super::Update();
		}
	}

	return false;
}

bool PlayerAnimFrontRun::Out()
{
	return false;
}

bool PlayerAnimBackRun::Enter(const shared_ptr<CharacterController>& playerController)
{
	if (dynamic_pointer_cast<PlayerController>(playerController))
	{
		_contoller = dynamic_pointer_cast<PlayerController>(playerController);
		_animator = _contoller.lock()->GetAnimator();
		_playerState = _contoller.lock()->GetCurrentUnitState();
	}
	else
	{
		_aiContoller = dynamic_pointer_cast<AIController>(playerController);
		_animator = _aiContoller.lock()->GetAnimator();
		_playerState = _aiContoller.lock()->GetCurrentPlayerUnitState();
	}

	if (_animator.lock()->GetFrameEnd() == true)
	{
		_animator.lock()->SetFrameEnd(false);
	}
	_animator.lock()->SetNextAnimation(L"BackRun");

	return true;
}

bool PlayerAnimBackRun::Update()
{
	if (_playerState.lock() != nullptr)
	{
		if (*_playerState.lock() != PlayerUnitState::BackMove &&
			*_playerState.lock() != PlayerUnitState::BackRightMove &&
			*_playerState.lock() != PlayerUnitState::BackLeftMove)
		{
			Super::Update();
		}
	}

	return false;
}

bool PlayerAnimBackRun::Out()
{
	return false;
}

bool PlayerAnimJumpStart::Enter(const shared_ptr<CharacterController>& playerController)
{
	if (dynamic_pointer_cast<PlayerController>(playerController))
	{
		_contoller = dynamic_pointer_cast<PlayerController>(playerController);
		_animator = _contoller.lock()->GetAnimator();
		_playerState = _contoller.lock()->GetCurrentUnitState();
		_jumpState = _contoller.lock()->GetJumpState();
	}
	else
	{
		_aiContoller = dynamic_pointer_cast<AIController>(playerController);
		_animator = _aiContoller.lock()->GetAnimator();
		_playerState = _aiContoller.lock()->GetCurrentPlayerUnitState();
		_jumpState = _aiContoller.lock()->GetJumpState();
	}

	if (_animator.lock()->GetFrameEnd() == true)
	{
		_animator.lock()->SetFrameEnd(false);
	}
	_animator.lock()->SetNextAnimation(L"JumpStart");

	return true;
}

bool PlayerAnimJumpStart::Update()
{
	if (_contoller.lock())
	{
		if (_jumpState.lock())
		{
			if (_jumpState.lock()->isJumpFall)
			{
				_contoller.lock()->SetAnimState(PlayerAnimType::JumpFall);
				return true;
			}
		}
	}
	else if (_aiContoller.lock())
	{
		if (_jumpState.lock())
		{
			if (_jumpState.lock()->isJumpFall)
			{
				_aiContoller.lock()->SetAnimState(PlayerAnimType::JumpFall);
				return true;
			}
		}
	}

	return false;
}

bool PlayerAnimJumpStart::Out()
{
	return false;
}

bool PlayerAnimJumpFall::Enter(const shared_ptr<CharacterController>& playerController)
{
	if (dynamic_pointer_cast<PlayerController>(playerController))
	{
		_contoller = dynamic_pointer_cast<PlayerController>(playerController);
		_animator = _contoller.lock()->GetAnimator();
		_playerState = _contoller.lock()->GetCurrentUnitState();
		_jumpState = _contoller.lock()->GetJumpState();
	}
	else
	{
		_aiContoller = dynamic_pointer_cast<AIController>(playerController);
		_animator = _aiContoller.lock()->GetAnimator();
		_playerState = _aiContoller.lock()->GetCurrentPlayerUnitState();
		_jumpState = _aiContoller.lock()->GetJumpState();
	}

	if (_animator.lock()->GetFrameEnd() == true)
	{
		_animator.lock()->SetFrameEnd(false);
	}
	_animator.lock()->SetNextAnimation(L"JumpFall");

	return true;
}

bool PlayerAnimJumpFall::Update()
{
	if (_contoller.lock())
	{
		if (_jumpState.lock())
		{
			if (_jumpState.lock()->isJumEnd)
			{
				_contoller.lock()->SetAnimState(PlayerAnimType::JumpEnd);
				return true;
			}
		}
	}
	else if (_aiContoller.lock())
	{
		if (_jumpState.lock())
		{
			if (_jumpState.lock()->isJumEnd)
			{
				_aiContoller.lock()->SetAnimState(PlayerAnimType::JumpEnd);
				return true;
			}
		}
	}

	return false;
}

bool PlayerAnimJumpFall::Out()
{
	return false;
}

bool PlayerAnimJumpEnd::Enter(const shared_ptr<CharacterController>& playerController)
{
	if (dynamic_pointer_cast<PlayerController>(playerController))
	{
		_contoller = dynamic_pointer_cast<PlayerController>(playerController);
		_animator = _contoller.lock()->GetAnimator();
		_playerState = _contoller.lock()->GetCurrentUnitState();
		_jumpState = _contoller.lock()->GetJumpState();
	}
	else
	{
		_aiContoller = dynamic_pointer_cast<AIController>(playerController);
		_animator = _aiContoller.lock()->GetAnimator();
		_playerState = _aiContoller.lock()->GetCurrentPlayerUnitState();
		_jumpState = _aiContoller.lock()->GetJumpState();
	}

	if (_animator.lock()->GetFrameEnd() == true)
	{
		_animator.lock()->SetFrameEnd(false);
	}
	_animator.lock()->SetNextAnimation(L"JumpEnd");

	return true;
}

bool PlayerAnimJumpEnd::Update()
{
	if (_contoller.lock())
	{
		if (_jumpState.lock())
		{
			if (_jumpState.lock()->isJump == false)
			{
				Super::Update();
			}
		}
	}

	return false;
}

bool PlayerAnimJumpEnd::Out()
{
	return false;
}

bool PlayerAnimStun::Enter(const shared_ptr<CharacterController>& playerController)
{
	return false;
}

bool PlayerAnimStun::Update()
{
	return false;
}

bool PlayerAnimStun::Out()
{
	return false;
}

bool PlayerAnimLoot::Enter(const shared_ptr<CharacterController>& playerController)
{
	return false;
}

bool PlayerAnimLoot::Update()
{
	return false;
}

bool PlayerAnimLoot::Out()
{
	return false;
}

PlayerAnimDamaged::PlayerAnimDamaged()
{
	_stateAnim = PlayerAnimType::Damaged;

	//Attack Sound
	auto tempSound = MANAGER_RESOURCES()->GetResource<Sounds>(L"Warrior_Damaged");
	if (tempSound == nullptr)
	{
		shared_ptr<Sounds> sound = make_shared<Sounds>();
		wstring soundPath = RESOURCES_ADDR_SOUND;
		soundPath += L"Character/Playable/Warrior/Warrior_Damaged.mp3";
		sound->Load(soundPath);
		sound->SetVolume(50);
		MANAGER_RESOURCES()->AddResource<Sounds>(L"Warrior_Damaged", sound);

		_damagedSound = sound->Clone();
	}
	else
	{
		_damagedSound = tempSound->Clone();
	}
}

bool PlayerAnimDamaged::Enter(const shared_ptr<CharacterController>& playerController)
{
	if (dynamic_pointer_cast<PlayerController>(playerController))
	{
		_contoller = dynamic_pointer_cast<PlayerController>(playerController);
		_animator = _contoller.lock()->GetAnimator();
		_playerState = _contoller.lock()->GetCurrentUnitState();
	}
	else
	{
		_aiContoller = dynamic_pointer_cast<AIController>(playerController);
		_animator = _aiContoller.lock()->GetAnimator();
		_playerState = _aiContoller.lock()->GetCurrentPlayerUnitState();
	}

	if (_animator.lock()->GetFrameEnd() == true)
	{
		_animator.lock()->SetFrameEnd(false);
	}
	_animator.lock()->SetNextAnimation(L"Damaged");
	_damagedSound->Play(false);
	_contoller.lock()->_isBattle = true;

	return false;
}

bool PlayerAnimDamaged::Update()
{
	if (_contoller.lock())
	{
		if (_animator.lock()->GetFrameEnd() == true)
		{
			Super::Update();
		}
	}

	return false;
}

bool PlayerAnimDamaged::Out()
{
	return false;
}

PlayerAnimDeath::PlayerAnimDeath()
{
	_stateAnim = PlayerAnimType::Death;

	//Death Sound
	auto tempSound = MANAGER_RESOURCES()->GetResource<Sounds>(L"Warrior_Death");
	if (tempSound == nullptr)
	{
		shared_ptr<Sounds> sound = make_shared<Sounds>();
		wstring soundPath = RESOURCES_ADDR_SOUND;
		soundPath += L"Character/Playable/Warrior/Warrior_Death.mp3";
		sound->Load(soundPath);
		sound->SetVolume(50);
		MANAGER_RESOURCES()->AddResource<Sounds>(L"Warrior_Death", sound);

		_deathSound = sound->Clone();
	}
	else
	{
		_deathSound = tempSound->Clone();
	}
}

bool PlayerAnimDeath::Enter(const shared_ptr<CharacterController>& playerController)
{
	if (dynamic_pointer_cast<PlayerController>(playerController))
	{
		_contoller = dynamic_pointer_cast<PlayerController>(playerController);
		_animator = _contoller.lock()->GetAnimator();
		_playerState = _contoller.lock()->GetCurrentUnitState();
	}
	else
	{
		_aiContoller = dynamic_pointer_cast<AIController>(playerController);
		_animator = _aiContoller.lock()->GetAnimator();
		_playerState = _aiContoller.lock()->GetCurrentPlayerUnitState();
	}

	if (_animator.lock()->GetFrameEnd() == true)
	{
		_animator.lock()->SetFrameEnd(false);
	}

	_animator.lock()->SetNextAnimation(L"Death");
	_animator.lock()->SetLoop(false);
	_deathSound->Play(false);
	_contoller.lock()->_isAlive = false;

	return false;
}

bool PlayerAnimDeath::Update()
{
	return false;
}

bool PlayerAnimDeath::Out()
{
	return false;
}

bool PlayerAnimBattle::Enter(const shared_ptr<CharacterController>& playerController)
{
	if (dynamic_pointer_cast<PlayerController>(playerController))
	{
		_contoller = dynamic_pointer_cast<PlayerController>(playerController);
		_animator = _contoller.lock()->GetAnimator();
		_playerState = _contoller.lock()->GetCurrentUnitState();
	}
	else
	{
		_aiContoller = dynamic_pointer_cast<AIController>(playerController);
		_animator = _aiContoller.lock()->GetAnimator();
		_playerState = _aiContoller.lock()->GetCurrentPlayerUnitState();
	}

	if (_animator.lock()->GetFrameEnd() == true)
	{
		_animator.lock()->SetFrameEnd(false);
	}
	_animator.lock()->SetNextAnimation(L"Battle");

	return false;
}

bool PlayerAnimBattle::Update()
{
	if (_playerState.lock() != nullptr)
	{
		if (*_playerState.lock() != PlayerUnitState::Battle)
		{
			Super::Update();
		}
	}

	return false;
}

bool PlayerAnimBattle::Out()
{
	return false;
}

PlayerAnimAttack1::PlayerAnimAttack1()
{
	_stateAnim = PlayerAnimType::Attack1;

	//Attack Sound
	auto tempSound = MANAGER_RESOURCES()->GetResource<Sounds>(L"Warrior_Attack1");
	if (tempSound == nullptr)
	{
		shared_ptr<Sounds> sound = make_shared<Sounds>();
		wstring soundPath = RESOURCES_ADDR_SOUND;
		soundPath += L"Character/Playable/Warrior/Warrior_Attack1.mp3";
		sound->Load(soundPath);
		sound->SetVolume(50);
		MANAGER_RESOURCES()->AddResource<Sounds>(L"Warrior_Attack1", sound);

		_attackSound = sound->Clone();
	}
	else
	{
		_attackSound = tempSound->Clone();
	}
}

bool PlayerAnimAttack1::Enter(const shared_ptr<CharacterController>& playerController)
{
	if (dynamic_pointer_cast<PlayerController>(playerController))
	{
		_contoller = dynamic_pointer_cast<PlayerController>(playerController);
		_animator = _contoller.lock()->GetAnimator();
		_playerState = _contoller.lock()->GetCurrentUnitState();
	}
	else
	{
		_aiContoller = dynamic_pointer_cast<AIController>(playerController);
		_animator = _aiContoller.lock()->GetAnimator();
		_playerState = _aiContoller.lock()->GetCurrentPlayerUnitState();
	}

	if (_animator.lock()->GetFrameEnd() == true)
	{
		_animator.lock()->SetFrameEnd(false);
	}

	_animator.lock()->SetNextAnimation(L"Attack1");
	_attackSound->Play(false);
	return false;
}

bool PlayerAnimAttack1::Update()
{
	if (_contoller.lock())
	{
		if (_animator.lock()->GetFrameEnd() == true)
		{
			*_playerState.lock() = PlayerUnitState::Battle;
			_contoller.lock()->SetAnimState(PlayerAnimType::Battle);
			return true;
		}
	}
	else if (_aiContoller.lock())
	{
		if (_animator.lock()->GetFrameEnd() == true)
		{
			*_playerState.lock() = PlayerUnitState::Battle;
			_aiContoller.lock()->SetAnimState(PlayerAnimType::Battle);
			return true;
		}
	}

	return false;
}

bool PlayerAnimAttack1::Out()
{
	return true;
}

PlayerAnimAttack2::PlayerAnimAttack2()
{
	_stateAnim = PlayerAnimType::Attack2;

	//Attack Sound
	auto tempSound = MANAGER_RESOURCES()->GetResource<Sounds>(L"Warrior_Attack2");
	if (tempSound == nullptr)
	{
		shared_ptr<Sounds> sound = make_shared<Sounds>();
		wstring soundPath = RESOURCES_ADDR_SOUND;
		soundPath += L"Character/Playable/Warrior/Warrior_Attack2.mp3";
		sound->Load(soundPath);
		sound->SetVolume(50);
		MANAGER_RESOURCES()->AddResource<Sounds>(L"Warrior_Attack2", sound);

		_attackSound = sound->Clone();
	}
	else
	{
		_attackSound = tempSound->Clone();
	}
}

bool PlayerAnimAttack2::Enter(const shared_ptr<CharacterController>& playerController)
{
	if (dynamic_pointer_cast<PlayerController>(playerController))
	{
		_contoller = dynamic_pointer_cast<PlayerController>(playerController);
		_animator = _contoller.lock()->GetAnimator();
		_playerState = _contoller.lock()->GetCurrentUnitState();
	}
	else
	{
		_aiContoller = dynamic_pointer_cast<AIController>(playerController);
		_animator = _aiContoller.lock()->GetAnimator();
		_playerState = _aiContoller.lock()->GetCurrentPlayerUnitState();
	}

	if (_animator.lock()->GetFrameEnd() == true)
	{
		_animator.lock()->SetFrameEnd(false);

	}

	_animator.lock()->SetNextAnimation(L"Attack2");
	_attackSound->Play(false);

	return false;
}

bool PlayerAnimAttack2::Update()
{
	if (_contoller.lock())
	{
		if (_animator.lock()->GetFrameEnd() == true)
		{
			*_playerState.lock() = PlayerUnitState::Battle;
			_contoller.lock()->SetAnimState(PlayerAnimType::Battle);
			return true;
		}
	}
	else if (_aiContoller.lock())
	{
		if (_animator.lock()->GetFrameEnd() == true)
		{
			*_playerState.lock() = PlayerUnitState::Battle;
			_aiContoller.lock()->SetAnimState(PlayerAnimType::Battle);
			return true;
		}
	}

	return false;
}

bool PlayerAnimAttack2::Out()
{
	return true;
}

bool PlayerAnimCasting::Enter(const shared_ptr<CharacterController>& playerController)
{
	if (dynamic_pointer_cast<PlayerController>(playerController))
	{
		_contoller = dynamic_pointer_cast<PlayerController>(playerController);
		_animator = _contoller.lock()->GetAnimator();
		_playerState = _contoller.lock()->GetCurrentUnitState();
	}
	else
	{
		_aiContoller = dynamic_pointer_cast<AIController>(playerController);
		_animator = _aiContoller.lock()->GetAnimator();
		_playerState = _aiContoller.lock()->GetCurrentPlayerUnitState();
	}

	if (_animator.lock()->GetFrameEnd() == true)
	{
		_animator.lock()->SetFrameEnd(false);
	}
	_animator.lock()->SetNextAnimation(L"Casting");

	return false;
}

bool PlayerAnimCasting::Update()
{
	return false;
}

bool PlayerAnimCasting::Out()
{
	return false;
}

PlayerAnimAbility1::PlayerAnimAbility1()
{
	_stateAnim = PlayerAnimType::Ability1;

	//Ability Sound
	auto tempSound = MANAGER_RESOURCES()->GetResource<Sounds>(L"Warrior_Ability1");
	if (tempSound == nullptr)
	{
		shared_ptr<Sounds> sound = make_shared<Sounds>();
		wstring soundPath = RESOURCES_ADDR_SOUND;
		soundPath += L"Character/Playable/Warrior/Warrior_Ability1.mp3";
		sound->Load(soundPath);
		sound->SetVolume(50);
		MANAGER_RESOURCES()->AddResource<Sounds>(L"Warrior_Ability1", sound);

		_abilitySound = sound->Clone();
	}
	else
	{
		_abilitySound = tempSound->Clone();
	}
}

bool PlayerAnimAbility1::Enter(const shared_ptr<CharacterController>& playerController)
{
	if (dynamic_pointer_cast<PlayerController>(playerController))
	{
		_contoller = dynamic_pointer_cast<PlayerController>(playerController);
		_animator = _contoller.lock()->GetAnimator();
		_playerState = _contoller.lock()->GetCurrentUnitState();
	}
	else
	{
		_aiContoller = dynamic_pointer_cast<AIController>(playerController);
		_animator = _aiContoller.lock()->GetAnimator();
		_playerState = _aiContoller.lock()->GetCurrentPlayerUnitState();
	}

	if (_animator.lock()->GetFrameEnd() == true)
	{
		_animator.lock()->SetFrameEnd(false);
	}
	_animator.lock()->SetNextAnimation(L"Ability1");
	_abilitySound->Play(false);

	return false;
}

bool PlayerAnimAbility1::Update()
{
	return false;
}

bool PlayerAnimAbility1::Out()
{

	return true;
}

PlayerAnimAbility2::PlayerAnimAbility2()
{
	_stateAnim = PlayerAnimType::Ability2;

	//Ability Sound
	auto tempSound = MANAGER_RESOURCES()->GetResource<Sounds>(L"Warrior_Ability2");
	if (tempSound == nullptr)
	{
		shared_ptr<Sounds> sound = make_shared<Sounds>();
		wstring soundPath = RESOURCES_ADDR_SOUND;
		soundPath += L"Character/Playable/Warrior/Warrior_Ability2.mp3";
		sound->Load(soundPath);
		sound->SetVolume(50);
		MANAGER_RESOURCES()->AddResource<Sounds>(L"Warrior_Ability2", sound);

		_abilitySound = sound->Clone();
	}
	else
	{
		_abilitySound = tempSound->Clone();
	}
}

bool PlayerAnimAbility2::Enter(const shared_ptr<CharacterController>& playerController)
{
	if (dynamic_pointer_cast<PlayerController>(playerController))
	{
		_contoller = dynamic_pointer_cast<PlayerController>(playerController);
		_animator = _contoller.lock()->GetAnimator();
		_playerState = _contoller.lock()->GetCurrentUnitState();
	}
	else
	{
		_aiContoller = dynamic_pointer_cast<AIController>(playerController);
		_animator = _aiContoller.lock()->GetAnimator();
		_playerState = _aiContoller.lock()->GetCurrentPlayerUnitState();
	}

	if (_animator.lock()->GetFrameEnd() == true)
	{
		_animator.lock()->SetFrameEnd(false);
	}
	_animator.lock()->SetNextAnimation(L"Ability2");
	_abilitySound->Play(false);

	return false;
}

bool PlayerAnimAbility2::Update()
{
	if (_contoller.lock() != nullptr)
	{
		if (_animator.lock()->GetFrameEnd() == true)
		{
			*_playerState.lock() = PlayerUnitState::Battle;
			_contoller.lock()->SetAnimState(PlayerAnimType::Battle);
			return true;
		}
	}
	else if (_aiContoller.lock())
	{
		if (_animator.lock()->GetFrameEnd() == true)
		{
			*_playerState.lock() = PlayerUnitState::Battle;
			_aiContoller.lock()->SetAnimState(PlayerAnimType::Battle);
			return true;
		}
	}

	return false;
}

bool PlayerAnimAbility2::Out()
{
	return false;
}

