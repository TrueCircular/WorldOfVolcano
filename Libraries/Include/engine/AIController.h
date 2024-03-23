#pragma once
#include "CharacterController.h"
#include "PlayerAnimState.h"
#include "PlayerSoundController.h"
#include "EnemySoundController.h"
#include "UnitStrategy.h"

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
	shared_ptr<JumpFlag>		_jumpState;
	float _defaultSpeed = 0.f;
	float _currentSpeed = 0.f;
	float _slowSpeed = 0.f;
	bool _isSlow = false;
private:
	weak_ptr<Transform>			_targetTransform;
	TarceTargetList				_targetList;
	Vec3						_targetPos = Vec3(0.f);
	float						_traceRadius = 0.f;
private:
	shared_ptr<UnitFSM>					_unitFsm;
	vector<shared_ptr<UnitStrategy>>	_unitStrategyList;
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
	void SetJumpState(const JumpFlag& jumpFlag) { *_jumpState = jumpFlag; }
	void notifyEnemyDeath() { _isAlive = false; }
	void SetTargetTransform(const shared_ptr<Transform> transform) { _targetTransform = transform; }
	void SetCurrentFsmStrategy(const wstring& preTransition, const wstring& nextTransition);
	void SetFsmStrategyList(const vector<shared_ptr<UnitStrategy>>& strategyList) { _unitStrategyList = strategyList; }
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
	const shared_ptr<UnitFSM>& GetUnitFsm() const { return _unitFsm; }
	//Animation Controll
	const shared_ptr<ModelAnimator>& GetAnimator() const { return _animator.lock(); }
	const shared_ptr<PlayerUnitState>& GetCurrentPlayerUnitState() const { return _currentPlayerState; }
	const PlayerAnimType& GetCurrentPlayerAnimType() const { return _currentPlayerAnimState->GetStateAnimtype(); }
public:
	//Event
	virtual void TakeDamage(const shared_ptr<GameObject>& sender, float damage) override;
	virtual void DeadEvent() override;
	void SearchTraceTarget();
public:
	virtual void Start() override;
	virtual void FixedUpdate() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
};

