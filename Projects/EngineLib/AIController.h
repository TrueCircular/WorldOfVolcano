#pragma once
#include "CharacterController.h"
#include "PlayerAnimState.h"
#include "EnemyAnimState.h"
#include "PlayerSoundController.h"
#include "EnemySoundController.h"

class HeightGetter;
class CharacterInfo;
class UnitFSM;

class AIController : public CharacterController, public enable_shared_from_this<AIController>
{
public:
	AIController(){}
	virtual ~AIController(){}
private:
	weak_ptr<HeightGetter>		_heightGetterCom;
	weak_ptr<Transform>		_transform;
	weak_ptr<Transform>		_targetTransform;
	Vec3						_targetPos;
	AIType						_type = AIType::None;
	weak_ptr<CharacterInfo>		_characterInfo;
	shared_ptr<JumpFlag>		_jumpState;
	shared_ptr<UnitFSM>			_currentFsmState;
	vector<shared_ptr<UnitFSM>>	_fsmList;
	float _defaultSpeed = 35.f;
	float _currentSpeed = 35.f;
	float _slowSpeed = 17.5f;
	bool _isSlow = false;
	bool _isTrace = false;
	bool _isAttack = false;
	bool _isAlive = true;
private:
	shared_ptr<ModelAnimator>			_animator;
private:
	//Player
	shared_ptr<PlayerUnitState>			_currentPlayerState;
	shared_ptr<PlayerAnimState>			_currentPlayerAnimState;
	vector<shared_ptr<PlayerAnimState>> _playerAnimStateList;
private:
	//Enemy
	shared_ptr<EnemyUnitState>			_currentEnemyState;
	shared_ptr<EnemyAnimState>			_currentEnemyAnimState;
	vector<shared_ptr<EnemyAnimState>>	_enemyAnimStateList;
	//Sound
	shared_ptr<PlayerSoundController>	_aiSound;
	shared_ptr<EnemySoundController>	_enemySound;
private:
	void InitAnimState();
public:
	//Setter
	//State Controll
	void SetIsAttack(bool attack) { _isAttack = attack; }
	void SetIsTrace(bool trace) { _isTrace = trace; }
	void SetJumpState(const JumpFlag& jumpFlag) { *_jumpState = jumpFlag; }
	void SetAIType(AIType type) { _type = type; }
	void notifyEnemyDeath() { _isAlive = false; }
	void SetTargetTransform(const shared_ptr<Transform> transform) { _targetTransform = transform; }
	void SetCurrentFsmState(UnitFSMState state);
	//Animation Controll
	bool SetAnimState(const PlayerAnimType& type);
	bool SetAnimState(const EnemyAnimType& type);
	void SetUnitState(const PlayerUnitState& state) { *_currentPlayerState = state; }
	void SetUnitState(const EnemyUnitState& state) { *_currentEnemyState = state; }
	//Sound Controll
	void SetEnemySound(shared_ptr<EnemySoundController> enemySound) { _enemySound = enemySound; };
	void SetAiSound(shared_ptr<PlayerSoundController> aiSound) { _aiSound = aiSound; };
public:
	//Getter
	//State Controll
	const shared_ptr<CharacterInfo>& GetCharacterInfo() const { return _characterInfo.lock(); }
	const float& GetDefaultSpeed() const { return _defaultSpeed; }
	const float& GetCurrentSpeed() const { return _currentSpeed; }
	const shared_ptr<JumpFlag>& GetJumpState() const { return _jumpState; }
	const bool& IsAttack() const { return _isAttack; }
	const bool& IsTrace() const { return _isTrace; }
	const bool& IsAlive() const { return _isAlive; }
	const shared_ptr<Transform>& GetTargetTransform() const { return _targetTransform.lock(); }
	const shared_ptr<HeightGetter>& GetHeightGetter() const { return _heightGetterCom.lock(); }
	//Animation Controll
	const shared_ptr<ModelAnimator>& GetAnimator() const { return _animator; }
	const shared_ptr<Transform>& GetTransform() const { return _transform.lock(); }
	const shared_ptr<PlayerUnitState>& GetCurrentPlayerUnitState() const { return _currentPlayerState; }
	const shared_ptr<EnemyUnitState>& GetCurrentEnemyUnitState() const { return _currentEnemyState; }
	const PlayerAnimType& GetCurrentPlayerAnimType() const { return _currentPlayerAnimState->GetStateAnimtype(); }
	const EnemyAnimType& GetCurrentEnemyAnimType() const { return _currentEnemyAnimState->GetStateAnimtype(); }
public:
	//Event
	virtual void TakeDamage(const shared_ptr<GameObject>& sender, uint16 damage) override;
public:
	virtual void Start() override;
	virtual void FixedUpdate() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
};

