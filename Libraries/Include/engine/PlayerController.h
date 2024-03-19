#pragma once
#include "CharacterController.h"
#include "PlayerSoundController.h"
#pragma region Declaration
class PlayerAnimState;
class PlayerAnimIdle;
class PlayerAnimRun;
class PlayerAnimBackRun;
class PlayerAnimJumpStart;
class PlayerAnimJumpFall;
class PlayerAnimJumpEnd;
#pragma endregion

class HeightGetter;

class PlayerController : public CharacterController, public enable_shared_from_this<PlayerController>
{
public:
	PlayerController();
	~PlayerController();
private:
	//Character Controll
	weak_ptr<HeightGetter> _heightGetterCom;
	weak_ptr<Transform> _transform;
	Vec3 _playerPos = { 0,0,0 };
	Vec3 _playerRot = { 0,0,0 };
	Vec3 _playerForward = { 0,0,0 };
	Vec3 _playerRight = { 0,0,0 };
	Vec3 _playerUp = { 0,0,0 };

	Vec3 _jumpStartPos = { 0,0,0 };
	Vec3 _jumpUpMaxPos = { 0,0,0 };
	Vec3 _jumpUpDir = { 0,1,0 };
	Vec3 _jumpDownDir = { 0,-1,0 };
	float _jumpPower = 20.f;
	shared_ptr<JumpFlag>		_jumpState;
	shared_ptr<PlayerUnitState>	_currentState;
public:
	//State Flag
	bool _isAttack = false;
	bool _isBattle = false;
	bool _isAlive = true;
public:
	//Pick
	bool _isPicked = false;
	CHARACTER_INFO _pickedInfo;
	shared_ptr<GameObject> _pickedObj;
	queue<CHARACTER_INFO> _attackQueue;

	float _battleTime = 15.f;
	float _battleTimer = 0.f;
	float _attackTime = 15.f;
	float _attackTimer = 0.0f;
	bool isend = false;
private:
	//Animation Controll
	weak_ptr<ModelAnimator>				_animator;
	shared_ptr<PlayerAnimState>			_animState;
	vector<shared_ptr<PlayerAnimState>> _animStateList;
	float _defaultSpeed = 40.f;
	float _currentSpeed = 40.f;
	float _slowSpeed = 20.f;
	float _dt = 0.f;
	bool _isSlow = false;
private:
	Vec3 _prevMousePos = Vec3(0.f);
	Vec3 _currentMousePos = Vec3(0.f);
private:
	//Camera Controll
	weak_ptr<GameObject> _camera;
	Vec3 _camPos;
	Vec3 _camRot;
	float _camDist = 0.f;
	float _camMinDist = 0.f;
	float _camMaxDist = 0.f;
	float _camSpeed = 500.f;
private:
	//Sound
	shared_ptr<PlayerSoundController> _sound = nullptr;
private:
	void AnimStateInit();
	//Player
	void PlayerInput();
	void PlayerMove();
	void PlayerJump();
	void PlayerAttack();
	void PlayerPicking();
	void KeyStateCheck();
	//Camera
	void CameraMove();
public:
	//Setter
	bool SetAnimState(const PlayerAnimType& animType);
	void SetIsAttack(bool setting) { _isAttack = setting; }
public:
	//Getter Animation
	const shared_ptr<ModelAnimator>& GetAnimator() { return _animator.lock(); }
	const shared_ptr<PlayerUnitState> GetCurrentUnitState() { return _currentState; }
	shared_ptr<PlayerAnimState> GetAnimState() { return _animState; }
	const PlayerAnimType GetCurrentAnimType();
	PlayerAnimType& GetCurrentAnimTypeRef();
public:
	//Getter Character State
	const shared_ptr<JumpFlag>& GetJumpState() { return _jumpState; }
	const float& GetDefaultSpeed() const { return _defaultSpeed; }
	const float& GetCurrentSpeed() const { return _currentSpeed; }
	const bool& IsAttack() const { return _isAttack; }
	const bool& IsBattle() const { return _isBattle; }
	void NotifyPlayerAlive(bool isAlive);
	const CHARACTER_INFO& GetPickedInfo() const { return _pickedInfo; }
	int GetAttackQueueSize();
public:
	//Sound
	void SetSoundController(shared_ptr<PlayerSoundController> controller) { _sound = controller; };
public:
	//Event
	virtual void TakeDamage(const shared_ptr<GameObject>& sender, uint16 damage) override;
public:
	virtual void Start() override;
	virtual void FixedUpdate() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
};

