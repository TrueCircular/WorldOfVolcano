#pragma once
#include "CharacterController.h"
#include "PlayerSoundController.h"

#pragma region Forward Declaration
class PlayerAnimState;
class CharacterInfo;
class AbilitySlot;
#pragma endregion

class HeightGetter;

class PlayerController : public CharacterController, public enable_shared_from_this<PlayerController>
{
public:
	PlayerController();
	~PlayerController();
private:
	//Character Controll
	Vec3 _playerPos = { 0,0,0 };
	Vec3 _playerRot = { 0,0,0 };
	Vec3 _playerForward = { 0,0,0 };
	Vec3 _playerRight = { 0,0,0 };
	Vec3 _playerUp = { 0,0,0 };
	shared_ptr<PlayerUnitState>	_currentState;
	shared_ptr<Transform>			_transform;
	shared_ptr<CharacterInfo>		_unitInfo;
	shared_ptr<HeightGetter>		_heightGetterCom;
	shared_ptr<AbilitySlot>		_abilitySlot;
	uint16 _defaultSpeed = 0;
	uint16 _currentSpeed = 0;
	uint16 _slowSpeed = 0;
	bool _isSlow = false;
private:
	//Pick
	bool				   _isPicked = false;
	CHARACTER_INFO		   _pickedInfo;
	shared_ptr<GameObject> _pickedObj;
private:
	//Jump
	shared_ptr<JumpFlag> _jumpState;
	Vec3				 _jumpStartPos = { 0,0,0 };
	Vec3				 _jumpUpMaxPos = { 0,0,0 };
	Vec3				 _jumpUpDir = { 0,1,0 };
	Vec3				 _jumpDownDir = { 0,-1,0 };
	float				 _jumpPower = 20.f;
public:
	float _dt = 0.f;
	float _battleTime = 15.f;
	float _battleTimer = 0.0f;
	float _attackTime = 0.0f;
	float _attackTimer = 0.0f;
	float _attackRange = 0.f;
private:
	//Animation Controll
	shared_ptr<ModelAnimator>				_animator;
	shared_ptr<PlayerAnimState>			_animState;
	vector<shared_ptr<PlayerAnimState>> _animStateList;
private:
	Vec3 _prevMousePos = Vec3(0.f);
	Vec3 _currentMousePos = Vec3(0.f);
private:
	//Camera Controll
	shared_ptr<GameObject> _camera;
	Vec3 _camPos;
	Vec3 _camRot;
	float _camDist = 0.f;
	float _camMinDist = 0.f;
	float _camMaxDist = 0.f;
	float _camSpeed = 500.f;
private:
	void AnimStateInit();
	//Player
	void PlayerInput();
	void PlayerMove();
	void PlayerJump();
	void PlayerAttack();
	void PlayerAbility1();
	void PlayerAbility2();
	void PlayerAbility3();
	void PlayerAbility4();
	void PlayerPicking();
	void PlayerTargetControll();
	void KeyStateCheck();
	//Camera
	void CameraMove();
public:
	//Setter
	bool SetAnimState(const PlayerAnimType& animType);
	void SetCurrentState(PlayerUnitState state) { *_currentState = state; }
public:
	//Getter Animation State
	const shared_ptr<ModelAnimator>& GetAnimator() { return _animator; }
	const shared_ptr<PlayerUnitState> GetCurrentUnitState() { return _currentState; }
	shared_ptr<PlayerAnimState> GetAnimState() { return _animState; }
	const PlayerAnimType GetCurrentAnimType();
	PlayerAnimType& GetCurrentAnimTypeRef();
public:
	//Getter Character State
	const shared_ptr<JumpFlag>& GetJumpState() { return _jumpState; }
	const float& GetDefaultSpeed() const { return _defaultSpeed; }
	const float& GetCurrentSpeed() const { return _currentSpeed; }
	void NotifyPlayerAlive(bool isAlive);
	const shared_ptr< CharacterInfo>& GetUnitInformation() const { return _unitInfo; }
	const CHARACTER_INFO& GetPickedInfo() const { return _pickedInfo; }
protected:
	virtual void InitController() override;
public:
	//Event
	virtual void TakeDamage(const shared_ptr<GameObject>& sender, float damage) override;
	virtual void DeadEvent() override;
	virtual void Respawn(const Vec3& spawnPos = Vec3(0.f)) override;
public:
	virtual void Start() override;
	virtual void FixedUpdate() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
};

