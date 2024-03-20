#pragma once
#include "CharacterController.h"
#include "PlayerAnimState.h"
#include "PlayerSoundController.h"
#include "EnemySoundController.h"

class HeightGetter;
class CharacterInfo;
class UnitFSM;

class AIController : public CharacterController, public enable_shared_from_this<AIController>
{
	using TarceTargetList = unordered_set<shared_ptr<PlayableUnit>>;
public:
	AIController(){}
	virtual ~AIController(){}
private:
	weak_ptr<HeightGetter>		_heightGetterCom;
	weak_ptr<ModelAnimator>		_animator;
	weak_ptr<Transform>			_transform;
	weak_ptr<CharacterInfo>		_characterInfo;
	AIType						_type = AIType::None;
	shared_ptr<JumpFlag>		_jumpState;
	float _defaultSpeed = 0.f;
	float _currentSpeed = 0.f;
	float _slowSpeed = 0.f;
	bool _isSlow = false;
	bool _isAttack = false;
	bool _isAlive = true;
private:
	weak_ptr<Transform>			_targetTransform;
	TarceTargetList				_targetList;
	Vec3						_targetPos = Vec3(0.f);
	float						_traceRadius = 0.f;
private:
	shared_ptr<UnitFSM>			_currentFsmState;
	vector<shared_ptr<UnitFSM>>	_fsmList;
private:
	//Player
	shared_ptr<PlayerUnitState>			_currentPlayerState;
	shared_ptr<PlayerAnimState>			_currentPlayerAnimState;
	vector<shared_ptr<PlayerAnimState>> _playerAnimStateList;
private:
	//Sound
	shared_ptr<PlayerSoundController>	_aiSound;
	shared_ptr<EnemySoundController>	_enemySound;
private:
	void InitState();
public:
	//Setter
	//State Controll
	void SetIsAttack(bool attack) { _isAttack = attack; }
	void SetJumpState(const JumpFlag& jumpFlag) { *_jumpState = jumpFlag; }
	void SetAIType(AIType type) { _type = type; }
	void notifyEnemyDeath() { _isAlive = false; }
	void SetTargetTransform(const shared_ptr<Transform> transform) { _targetTransform = transform; }
	void SetCurrentFsmState(UnitFSMState state);
	//Animation Controll
	bool SetAnimState(const PlayerAnimType& type);
	void SetUnitState(const PlayerUnitState& state) { *_currentPlayerState = state; }
	//Sound Controll
	void SetEnemySound(shared_ptr<EnemySoundController> enemySound) { _enemySound = enemySound; };
	void SetAiSound(shared_ptr<PlayerSoundController> aiSound) { _aiSound = aiSound; };
public:
	//Getter
	//State Controll
	const shared_ptr<Transform>& GetTransform() const { return _transform.lock(); }
	const shared_ptr<Transform>& GetTargetTransform() const { return _targetTransform.lock(); }
	const shared_ptr<CharacterInfo>& GetCharacterInfo() const { return _characterInfo.lock(); }
	const shared_ptr<JumpFlag>& GetJumpState() const { return _jumpState; }
	const float& GetDefaultSpeed() const { return _defaultSpeed; }
	const float& GetCurrentSpeed() const { return _currentSpeed; }
	const bool& IsAttack() const { return _isAttack; }
	const bool& IsAlive() const { return _isAlive; }
	//Animation Controll
	const shared_ptr<ModelAnimator>& GetAnimator() const { return _animator.lock(); }
	const shared_ptr<PlayerUnitState>& GetCurrentPlayerUnitState() const { return _currentPlayerState; }
	const PlayerAnimType& GetCurrentPlayerAnimType() const { return _currentPlayerAnimState->GetStateAnimtype(); }
public:
	//Event
	virtual void TakeDamage(const shared_ptr<GameObject>& sender, uint16 damage) override;
	void SearchTraceTarget();
public:
	virtual void Start() override;
	virtual void FixedUpdate() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
};

