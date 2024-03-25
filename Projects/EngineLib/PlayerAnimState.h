#pragma once

#pragma region Declaration
class CharacterController;
class PlayerController;
class AIController;
class Sounds;
struct JumpFlag;
#pragma endregion

#pragma region PlayerAnimationStateMachine
class PlayerAnimState
{
protected:
	weak_ptr<PlayerController>		_contoller;
	weak_ptr<AIController>			_aiContoller;
	weak_ptr<ModelAnimator>			_animator;
	weak_ptr<PlayerUnitState>		_playerState;
	PlayerAnimType					_stateAnim;
public:
	PlayerAnimState() { _stateAnim = PlayerAnimType::None; };
	virtual ~PlayerAnimState() {};
public:
	virtual bool Enter(const shared_ptr<CharacterController>& playerController) = 0;
	virtual bool Update();
	virtual bool Out() = 0;
	PlayerAnimType& GetStateAnimtype() { return _stateAnim; }
};

class PlayerAnimIdle : public PlayerAnimState
{
	using Super = PlayerAnimState;
public:
	PlayerAnimIdle() { _stateAnim = PlayerAnimType::Stand; };
	virtual ~PlayerAnimIdle() {};
public:
	virtual bool Enter(const shared_ptr<CharacterController>& playerController) override;
	virtual bool Update() override;
	virtual bool Out() override;
};

class PlayerAnimFrontWalk : public PlayerAnimState
{
	using Super = PlayerAnimState;
public:
	PlayerAnimFrontWalk() { _stateAnim = PlayerAnimType::FrontWalk; };
	virtual ~PlayerAnimFrontWalk() {};
public:
	virtual bool Enter(const shared_ptr<CharacterController>& playerController) override;
	virtual bool Update() override;
	virtual bool Out() override;
};

class PlayerAnimBackWalk : public PlayerAnimState
{
	using Super = PlayerAnimState;
public:
	PlayerAnimBackWalk() { _stateAnim = PlayerAnimType::BackWalk; };
	virtual ~PlayerAnimBackWalk() {};
public:
	virtual bool Enter(const shared_ptr<CharacterController>& playerController) override;
	virtual bool Update() override;
	virtual bool Out() override;
};

class PlayerAnimFrontRun : public PlayerAnimState
{
	using Super = PlayerAnimState;
public:
	PlayerAnimFrontRun() { _stateAnim = PlayerAnimType::FrontRun; };
	virtual ~PlayerAnimFrontRun() {};
public:
	virtual bool Enter(const shared_ptr<CharacterController>& playerController) override;
	virtual bool Update() override;
	virtual bool Out() override;
};

class PlayerAnimBackRun : public PlayerAnimState
{
	using Super = PlayerAnimState;
public:
	PlayerAnimBackRun() { _stateAnim = PlayerAnimType::BackRun; };
	virtual ~PlayerAnimBackRun() {};
public:
	virtual bool Enter(const shared_ptr<CharacterController>& playerController) override;
	virtual bool Update() override;
	virtual bool Out() override;
};

class PlayerAnimJumpStart : public PlayerAnimState
{
	using Super = PlayerAnimState;
private:
	weak_ptr<JumpFlag> _jumpState;
public:
	PlayerAnimJumpStart() { _stateAnim = PlayerAnimType::JumpStart; };
	virtual ~PlayerAnimJumpStart() {};
public:
	virtual bool Enter(const shared_ptr<CharacterController>& playerController) override;
	virtual bool Update() override;
	virtual bool Out() override;
};

class PlayerAnimJumpFall : public PlayerAnimState
{
	using Super = PlayerAnimState;
private:
	weak_ptr<JumpFlag> _jumpState;
public:
	PlayerAnimJumpFall() { _stateAnim = PlayerAnimType::JumpFall; };
	virtual ~PlayerAnimJumpFall() {};
public:
	virtual bool Enter(const shared_ptr<CharacterController>& playerController) override;
	virtual bool Update() override;
	virtual bool Out() override;
};

class PlayerAnimJumpEnd : public PlayerAnimState
{
	using Super = PlayerAnimState;
private:
	weak_ptr<JumpFlag> _jumpState;
public:
	PlayerAnimJumpEnd() { _stateAnim = PlayerAnimType::JumpEnd; };
	virtual ~PlayerAnimJumpEnd() {};
public:
	virtual bool Enter(const shared_ptr<CharacterController>& playerController) override;
	virtual bool Update() override;
	virtual bool Out() override;
};

class PlayerAnimStun : public PlayerAnimState
{
	using Super = PlayerAnimState;
public:
	PlayerAnimStun() { _stateAnim = PlayerAnimType::Stun; };
	virtual ~PlayerAnimStun() {};
public:
	virtual bool Enter(const shared_ptr<CharacterController>& playerController) override;
	virtual bool Update() override;
	virtual bool Out() override;
};

class PlayerAnimLoot : public PlayerAnimState
{
	using Super = PlayerAnimState;
public:
	PlayerAnimLoot() { _stateAnim = PlayerAnimType::Loot; };
	virtual ~PlayerAnimLoot() {};
public:
	virtual bool Enter(const shared_ptr<CharacterController>& playerController) override;
	virtual bool Update() override;
	virtual bool Out() override;
};

class PlayerAnimDamaged : public PlayerAnimState
{
	using Super = PlayerAnimState;
public:
	PlayerAnimDamaged();
	virtual ~PlayerAnimDamaged() {};
private:
	shared_ptr<Sounds> _damagedSound;
public:
	virtual bool Enter(const shared_ptr<CharacterController>& playerController) override;
	virtual bool Update() override;
	virtual bool Out() override;
};

class PlayerAnimDeath : public PlayerAnimState
{
	using Super = PlayerAnimState;
public:
	PlayerAnimDeath();
	virtual ~PlayerAnimDeath() {};
private:
	shared_ptr<Sounds> _deathSound;
public:
	virtual bool Enter(const shared_ptr<CharacterController>& playerController) override;
	virtual bool Update() override;
	virtual bool Out() override;
};

class PlayerAnimBattle : public PlayerAnimState
{
	using Super = PlayerAnimState;
public:
	PlayerAnimBattle() { _stateAnim = PlayerAnimType::Battle; };
	virtual ~PlayerAnimBattle() {};
public:
	virtual bool Enter(const shared_ptr<CharacterController>& playerController) override;
	virtual bool Update() override;
	virtual bool Out() override;
};

class PlayerAnimAttack1 : public PlayerAnimState
{
	using Super = PlayerAnimState;
public:
	PlayerAnimAttack1();
	virtual ~PlayerAnimAttack1() {};
private:
	shared_ptr<Sounds> _attackSound;
public:
	virtual bool Enter(const shared_ptr<CharacterController>& playerController) override;
	virtual bool Update() override;
	virtual bool Out() override;
};

class PlayerAnimAttack2 : public PlayerAnimState
{
	using Super = PlayerAnimState;
public:
	PlayerAnimAttack2();
	virtual ~PlayerAnimAttack2() {};
private:
	shared_ptr<Sounds> _attackSound;
public:
	virtual bool Enter(const shared_ptr<CharacterController>& playerController) override;
	virtual bool Update() override;
	virtual bool Out() override;
};

class PlayerAnimCasting : public PlayerAnimState
{
	using Super = PlayerAnimState;
public:
	PlayerAnimCasting() { _stateAnim = PlayerAnimType::Casting; };
	virtual ~PlayerAnimCasting() {};
public:
	virtual bool Enter(const shared_ptr<CharacterController>& playerController) override;
	virtual bool Update() override;
	virtual bool Out() override;
};

class PlayerAnimAbility1 : public PlayerAnimState
{
	using Super = PlayerAnimState;
public:
	PlayerAnimAbility1();
	virtual ~PlayerAnimAbility1() {};
private:
	shared_ptr<Sounds> _abilitySound;
public:
	virtual bool Enter(const shared_ptr<CharacterController>& playerController) override;
	virtual bool Update() override;
	virtual bool Out() override;
};

class PlayerAnimAbility2 : public PlayerAnimState
{
	using Super = PlayerAnimState;
public:
	PlayerAnimAbility2();
	virtual ~PlayerAnimAbility2() {};
private:
	shared_ptr<Sounds> _abilitySound;
public:
	virtual bool Enter(const shared_ptr<CharacterController>& playerController) override;
	virtual bool Update() override;
	virtual bool Out() override;
};
#pragma endregion

