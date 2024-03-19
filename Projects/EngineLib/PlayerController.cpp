#include "pch.h"
#include "PlayerController.h"
#include "PlayerAnimState.h"
#include "HeightGetter.h"
#include <float.h>
#include "CharacterInfo.h"

PlayerController::PlayerController()
{
}

PlayerController::~PlayerController()
{
}

void PlayerController::AnimStateInit()
{
	//Idle
	_animStateList.push_back(make_shared<PlayerAnimIdle>());
	//FrontWalk
	_animStateList.push_back(make_shared<PlayerAnimFrontWalk>());
	//BackWalk
	_animStateList.push_back(make_shared<PlayerAnimBackWalk>());
	//FrontRun
	_animStateList.push_back(make_shared<PlayerAnimFrontRun>());
	//BackRun
	_animStateList.push_back(make_shared<PlayerAnimBackRun>());
	//JumpStart
	_animStateList.push_back(make_shared<PlayerAnimJumpStart>());
	//JumpFall
	_animStateList.push_back(make_shared<PlayerAnimJumpFall>());
	//JumpEnd
	_animStateList.push_back(make_shared<PlayerAnimJumpEnd>());
	//Stun
	_animStateList.push_back(make_shared<PlayerAnimStun>());
	//Loot
	_animStateList.push_back(make_shared<PlayerAnimLoot>());
	//Damaged
	_animStateList.push_back(make_shared<PlayerAnimDamaged>());
	//Death
	_animStateList.push_back(make_shared<PlayerAnimDeath>());
	//Battle
	_animStateList.push_back(make_shared<PlayerAnimBattle>());
	//Attack1
	_animStateList.push_back(make_shared<PlayerAnimAttack1>());
	//Attack2
	_animStateList.push_back(make_shared<PlayerAnimAttack2>());
	//Casting
	_animStateList.push_back(make_shared<PlayerAnimCasting>());
	//Ability1
	_animStateList.push_back(make_shared<PlayerAnimAbility1>());
	//Ability2
	_animStateList.push_back(make_shared<PlayerAnimAbility2>());

	_animState = _animStateList[0];
	_animState->Enter(shared_from_this());
}

void PlayerController::CameraMove()
{
	_dt = MANAGER_TIME()->GetDeltaTime();
	_currentMousePos = MANAGER_INPUT()->GetScreenMousePos();

	//휠 올렸을 때(시점 앞으로)
	if (g_gameDesc.WheelState == 1)
	{
		_camDist -= _camSpeed * _dt;
		_camPos = _camera.lock()->GetCamera()->GetCameraPosition();

		if (_camDist < _camMinDist)
		{
			_camDist = _camMinDist;
		}

		Vec3 cameraDirection = _camera.lock()->GetCamera()->GetCameraLookVector();
		Vec3 newCamPos = _playerPos - cameraDirection * _camDist; // 타깃에서 카메라 방향으로 _camDist만큼 떨어진 위치
		newCamPos += _camera.lock()->GetCamera()->GetCameraToTargetOffset();

		_camera.lock()->GetCamera()->SetCameraPosition(newCamPos);
		_camera.lock()->GetCamera()->Update();
	}
	//휠 내렸을 때(시점 뒤로)
	else if (g_gameDesc.WheelState == -1)
	{
		_camDist += _camSpeed * _dt;
		_camPos = _camera.lock()->GetCamera()->GetCameraPosition();

		if (_camDist > _camMaxDist)
		{
			_camDist = _camMaxDist;
		}


		Vec3 cameraDirection = _camera.lock()->GetCamera()->GetCameraLookVector();
		cameraDirection.Normalize(cameraDirection);
		Vec3 newCamPos = _playerPos - cameraDirection * _camDist; // 타깃에서 카메라 방향으로 _camDist만큼 떨어진 위치
		newCamPos += _camera.lock()->GetCamera()->GetCameraToTargetOffset();

		_camera.lock()->GetCamera()->SetCameraPosition(newCamPos);
		_camera.lock()->GetCamera()->SetCameraDistance(_camDist);
		_camera.lock()->GetCamera()->Update();

	}
	//마우스 오른쪽 버튼 누르고 있을 때(캐릭터회전 = 카메라 회전(위치이동))
	if (MANAGER_INPUT()->GetButton(KEY_TYPE::RBUTTON) && _isAlive == true)
	{
		{
			_playerRot = _transform.lock()->GetLocalRotation();
			float deltaX = _currentMousePos.x - _prevMousePos.x;
			_playerRot.y += ::XMConvertToRadians(deltaX) * 10 * _dt;
			_transform.lock()->SetLocalRotation(_playerRot);

			_camera.lock()->GetCamera()->SetCameraRotationYaw(_playerRot.y);
		}
	}
	//마우스 왼쪽 버튼 누르고 있을 때(카메라만 시점 변경)
	else if (MANAGER_INPUT()->GetButton(KEY_TYPE::LBUTTON))
	{
		{
			float deltaX = _currentMousePos.x - _prevMousePos.x;
			float deltaY = _currentMousePos.y - _prevMousePos.y;

			_camRot.x = ::XMConvertToRadians(deltaY) * 0.1f;
			_camRot.y = ::XMConvertToRadians(deltaX) * 0.1f;
			_camRot.z = 0.f;

			_camera.lock()->GetCamera()->RotateAroundToTarget(_playerPos, _camRot);
		}
	}
	_camera.lock()->GetCamera()->SetCameraDistance(_camDist);
	//_camera.lock()->GetCamera()->SetCameraPosition(_camPos);
	_prevMousePos = _currentMousePos;
}

bool PlayerController::SetAnimState(const PlayerAnimType& animType)
{
	if (animType == PlayerAnimType::None)
	{
		return false;
	}
	else
	{
		_animState->Out();

		_animState = _animStateList[static_cast<int>(animType)];

		if (_animState)
		{
			_animState->Enter(shared_from_this());
			return true;
		}

	}

	return false;
}


const PlayerAnimType PlayerController::GetCurrentAnimType()
{
	return _animState->GetStateAnimtype();
}
PlayerAnimType& PlayerController::GetCurrentAnimTypeRef()
{
	return _animState->GetStateAnimtype();
}

void PlayerController::PlayerInput()
{
	if (_isAlive == true)
	{
		PlayerMove();
	}

	_animState->Update();

	if (_sound)
		_sound->PlaySound(_animState->GetStateAnimtype());

	//Debug
	//{
	//	string outputString;

	//	switch (*_currentState)
	//	{
	//	case PlayerUnitState::Stand:
	//	{
	//		outputString = "Stand";
	//	}
	//	break;
	//	case PlayerUnitState::FrontMove:
	//	{
	//		outputString = "FrontMove";
	//	}
	//	break;
	//	case PlayerUnitState::FrontRightMove:
	//	{
	//		outputString = "FrontRightMove";
	//	}
	//	break;
	//	case PlayerUnitState::FrontLeftMove:
	//	{
	//		outputString = "FrontLeftMove";
	//	}
	//	break;
	//	case PlayerUnitState::BackMove:
	//	{
	//		outputString = "BackMove";
	//	}
	//	break;
	//	case PlayerUnitState::BackRightMove:
	//	{
	//		outputString = "BackRightMove";
	//	}
	//	break;
	//	case PlayerUnitState::BackLeftMove:
	//	{
	//		outputString = "BackLeftMove";
	//	}
	//	break;
	//	case PlayerUnitState::RightMove:
	//	{
	//		outputString = "RightMove";
	//	}
	//	break;
	//	case PlayerUnitState::LeftMove:
	//	{
	//		outputString = "LeftMove";
	//	}
	//	break;
	//	case PlayerUnitState::Jump:
	//	{
	//		outputString = "Jump";
	//	}
	//	break;
	//	}
	//	outputString += "\n";
	//	::OutputDebugStringA(outputString.c_str());
	//}
}

void PlayerController::PlayerMove()
{
	_dt = MANAGER_TIME()->GetDeltaTime();
	_playerPos = _transform.lock()->GetPosition();
	_playerForward = _transform.lock()->GetLookVector();
	_playerRight = _transform.lock()->GetRightVector();

	if (_heightGetterCom.lock())
	{
		if (_jumpState->isJump == false)
			_playerPos.y = _heightGetterCom.lock()->GetHeight();
	}

	if (_isSlow)
	{
		_currentSpeed = _slowSpeed;
	}
	else
	{
		_currentSpeed = _defaultSpeed;
	}

	PlayerPicking();
	PlayerAttack();

	if (_isAttack == false)
	{
		//왼쪽
		if (MANAGER_INPUT()->GetButton(KEY_TYPE::A))
		{
			*_currentState = PlayerUnitState::LeftMove;
			{
				//_playerRight = _transform.lock()->GetRightVector();
				_playerPos -= _playerRight * _currentSpeed * _dt;
				_transform.lock()->SetPosition(_playerPos);
				_camera.lock()->GetCamera()->SetCameraIsRotationAround(false);
				_camera.lock()->GetCamera()->SetCameraRotationYaw(_playerRot.y);

			}
		}
		//오른쪽
		else if (MANAGER_INPUT()->GetButton(KEY_TYPE::D))
		{
			*_currentState = PlayerUnitState::RightMove;

			{
				//_playerRight = _transform.lock()->GetRightVector();
				_playerPos += _playerRight * _currentSpeed * _dt;
				_transform.lock()->SetPosition(_playerPos);
				_camera.lock()->GetCamera()->SetCameraIsRotationAround(false);
				_camera.lock()->GetCamera()->SetCameraRotationYaw(_playerRot.y);

			}
		}
		//앞
		if (MANAGER_INPUT()->GetButton(KEY_TYPE::W))
		{
			*_currentState = PlayerUnitState::FrontMove;

			_playerForward = _transform.lock()->GetLookVector();
			_playerPos += _playerForward * _currentSpeed * _dt;
			_transform.lock()->SetPosition(_playerPos);
			_camera.lock()->GetCamera()->SetCameraIsRotationAround(false);
			_camera.lock()->GetCamera()->SetCameraRotationYaw(_playerRot.y);

		}
		//뒤
		else if (MANAGER_INPUT()->GetButton(KEY_TYPE::S))
		{
			*_currentState = PlayerUnitState::BackMove;

			_playerForward = _transform.lock()->GetLookVector();
			_playerPos -= _playerForward * _currentSpeed * _dt;
			_transform.lock()->SetPosition(_playerPos);
			_camera.lock()->GetCamera()->SetCameraIsRotationAround(false);
			_camera.lock()->GetCamera()->SetCameraRotationYaw(_playerRot.y);

		}
	}
	else // _isAttack == true
	{
		//왼쪽
		if (MANAGER_INPUT()->GetButton(KEY_TYPE::A))
		{
			_playerRight = _transform.lock()->GetRightVector();
			_playerPos -= _playerRight * _currentSpeed * _dt;
			_transform.lock()->SetPosition(_playerPos);
			_camera.lock()->GetCamera()->SetCameraIsRotationAround(false);
			_camera.lock()->GetCamera()->SetCameraRotationYaw(_playerRot.y);

		}
		//오른쪽
		else if (MANAGER_INPUT()->GetButton(KEY_TYPE::D))
		{
			_playerRight = _transform.lock()->GetRightVector();
			_playerPos += _playerRight * _currentSpeed * _dt;
			_transform.lock()->SetPosition(_playerPos);
			_camera.lock()->GetCamera()->SetCameraIsRotationAround(false);
			_camera.lock()->GetCamera()->SetCameraRotationYaw(_playerRot.y);

		}

		//앞
		if (MANAGER_INPUT()->GetButton(KEY_TYPE::W))
		{
			_playerForward = _transform.lock()->GetLookVector();
			_playerPos += _playerForward * _currentSpeed * _dt;
			_transform.lock()->SetPosition(_playerPos);
			_camera.lock()->GetCamera()->SetCameraIsRotationAround(false);
			_camera.lock()->GetCamera()->SetCameraRotationYaw(_playerRot.y);

		}
		//뒤
		else if (MANAGER_INPUT()->GetButton(KEY_TYPE::S))
		{
			_playerForward = _transform.lock()->GetLookVector();
			_playerPos -= _playerForward * _currentSpeed * _dt;
			_transform.lock()->SetPosition(_playerPos);
			_camera.lock()->GetCamera()->SetCameraIsRotationAround(false);
			_camera.lock()->GetCamera()->SetCameraRotationYaw(_playerRot.y);

		}
	}

	if (_isAttack == false)
	{
		PlayerJump();
	}

	KeyStateCheck();
}

void PlayerController::PlayerJump()
{

	//점프
	if (MANAGER_INPUT()->GetButtonDown(KEY_TYPE::SPACE))
	{
		if (!_jumpState->isJump)
		{
			_jumpState->isJump = true;
			_jumpState->isJumpUP = true;
			_jumpUpMaxPos = _playerPos + (_jumpUpDir * _jumpPower);
			*_currentState = PlayerUnitState::Jump;
			_camera.lock()->GetCamera()->SetCameraIsRotationAround(false);
		}
	}

	if (_jumpState->isJump)
	{
		if (_jumpState->isJumpUP)
		{
			if (_playerPos.y < _jumpUpMaxPos.y + FLT_EPSILON)
			{
				_playerPos = Vec3::Lerp(_playerPos, Vec3(_playerPos + _jumpUpDir * _jumpPower), 2.0f * _dt);
				_transform.lock()->SetPosition(_playerPos);
			}
			else
			{
				_jumpState->isJumpUP = false;
				_jumpState->isJumpFall = true;
			}
		}
		if (_jumpState->isJumpFall)
		{
			float height = _heightGetterCom.lock()->GetHeight();
			float test = height + _jumpPower;
			if (_playerPos.y < (height + _jumpPower) / 1.4f)
			{
				_jumpState->isJumpFall = false;
				_jumpState->isJumEnd = true;
			}
			else
			{
				_playerPos = Vec3::Lerp(_playerPos, Vec3(_playerPos + _jumpDownDir * _jumpPower), 1.5f * _dt);
				_transform.lock()->SetPosition(_playerPos);
			}
		}
		if (_jumpState->isJumEnd)
		{
			if (_playerPos.y < _heightGetterCom.lock()->GetHeight() + FLT_EPSILON)
			{
				_playerPos.y = _heightGetterCom.lock()->GetHeight();
				_transform.lock()->SetLocalPosition(_playerPos);
				_jumpState->isJumEnd = false;
				_jumpState->isJump = false;
				return;
			}
			else
			{
				_playerPos = Vec3::Lerp(_playerPos, Vec3(_playerPos + _jumpDownDir * _jumpPower), 1.5f * _dt);
				_transform.lock()->SetPosition(_playerPos);
			}
		}
	}
}

void PlayerController::PlayerAttack()
{
	if (_isAttack == true && _jumpState->isJump == false && _isPicked == true)
	{
		_battleTimer = 0.f;
		*_currentState = PlayerUnitState::Attack;
	}
}

void PlayerController::PlayerPicking()
{
	if (_isPicked)
	{
		_pickedInfo = _pickedObj->GetComponent<CharacterInfo>()->GetCharacterInfo();
		MANAGER_IMGUI()->UpdatePicked(true, _pickedInfo._maxHp, _pickedInfo._hp, _pickedObj->GetName());
		if (_pickedInfo._hp == 0)
		{
			MANAGER_IMGUI()->UpdatePicked(false);
			_isPicked = false;
			//MANAGER_SCENE()->GetCurrentScene()->Remove(_pickedObj);
		}

		if (MANAGER_INPUT()->GetButtonDown(KEY_TYPE::RBUTTON))
		{
			int32 mx = MANAGER_INPUT()->GetScreenMousePos().x;
			int32 my = MANAGER_INPUT()->GetScreenMousePos().y;

			auto pickObj = MANAGER_SCENE()->GetCurrentScene()->Pick(mx, my);

			if (pickObj && pickObj->GetName() != L"") //어떤 타입이든 인식할수 있게 수정해야할 필요 있음
			{
				Vec3 diff = DirectX::XMVectorSubtract(_playerPos, _pickedInfo._pos);
				float distance = DirectX::XMVectorGetX(DirectX::XMVector3Length(diff));

				if (distance <= 15.0f)
				{
					if (_isAttack == false)
					{
						_attackQueue.push(SkillType::NormalAttack);
					}
					_isAttack = true;
					_isBattle = true;
				}

			}
		}

		if (MANAGER_INPUT()->GetButton(KEY_TYPE::KEY_1))
		{
			Vec3 diff = DirectX::XMVectorSubtract(_playerPos, _pickedInfo._pos);
			float distance = DirectX::XMVectorGetX(DirectX::XMVector3Length(diff));

			if (distance > 15.0f)
			{
				if (_isAttack == false)
				{
					_attackQueue.push(SkillType::Test_AllAttack);
				}
				_isAttack = true;
				_isBattle = true;
			}
		}
	}

	if (MANAGER_INPUT()->GetButtonDown(KEY_TYPE::LBUTTON))
	{
		int32 mx = MANAGER_INPUT()->GetScreenMousePos().x;
		int32 my = MANAGER_INPUT()->GetScreenMousePos().y;

		auto pickObj = MANAGER_SCENE()->GetCurrentScene()->Pick(mx, my);

		if (pickObj && pickObj->GetName() == L"MagniBronzebeard")
		{
			MANAGER_IMGUI()->BeginDialogue();
		}
		else if (pickObj && pickObj->GetName() != L"") //어떤 타입이든 인식할수 있게 수정해야할 필요 있음
		{
			_isPicked = true;
			_pickedObj = pickObj;
		}
		else
		{
			MANAGER_IMGUI()->UpdatePicked(false);
			_isPicked = false;
		}
	}
}

void PlayerController::KeyStateCheck()
{
	if (_jumpState->isJump == false)
	{
		if (_isAttack == false)
		{
			if (*_currentState == PlayerUnitState::LeftMove ||
				*_currentState == PlayerUnitState::RightMove ||
				*_currentState == PlayerUnitState::FrontMove)
			{
				_isSlow = false;
			}
			else if (*_currentState == PlayerUnitState::BackMove)
			{
				_isSlow = true;
			}

			if (MANAGER_INPUT()->GetButton(KEY_TYPE::W) &&
				MANAGER_INPUT()->GetButton(KEY_TYPE::A))
			{
				*_currentState = PlayerUnitState::FrontLeftMove;
			}
			else if (MANAGER_INPUT()->GetButton(KEY_TYPE::S) &&
				MANAGER_INPUT()->GetButton(KEY_TYPE::A))
			{
				_isSlow = true;
				*_currentState = PlayerUnitState::BackLeftMove;
			}
			else if (MANAGER_INPUT()->GetButton(KEY_TYPE::W) &&
				MANAGER_INPUT()->GetButton(KEY_TYPE::D))
			{
				*_currentState = PlayerUnitState::FrontRightMove;
			}
			else if (MANAGER_INPUT()->GetButton(KEY_TYPE::S) &&
				MANAGER_INPUT()->GetButton(KEY_TYPE::D))
			{
				_isSlow = true;
				*_currentState = PlayerUnitState::BackRightMove;
			}
			else if (!MANAGER_INPUT()->GetButton(KEY_TYPE::W) &&
				!MANAGER_INPUT()->GetButton(KEY_TYPE::S) &&
				!MANAGER_INPUT()->GetButton(KEY_TYPE::A) &&
				!MANAGER_INPUT()->GetButton(KEY_TYPE::D))
			{

				if (_isBattle)
				{
					_currentSpeed = _defaultSpeed;
					*_currentState = PlayerUnitState::Battle;
				}
				else
				{
					_currentSpeed = _defaultSpeed;
					*_currentState = PlayerUnitState::Stand;
				}

			}
		}
	}
}

void PlayerController::NotifyPlayerAlive(bool isAlive)
{
	_isAlive = isAlive;

	if (isAlive == true)
	{
		SetAnimState(PlayerAnimType::Stand);
	}

	if (isAlive == false)
	{
		SetAnimState(PlayerAnimType::Death);
	}
}

SkillType PlayerController::GetFrontAttackQueue()
{
	if (_attackQueue.empty() == false)
	{
		SkillType result = _attackQueue.front();
		_attackQueue.pop();
		return result;
	}

	return SkillType::NormalAttack;
}

int PlayerController::GetAttackQueueSize()
{
	if (_attackQueue.empty() == false)
	{
		return _attackQueue.size();
	}

	return -1;
}

void PlayerController::ReceiveEvent(const EventArgs& args)
{
}

void PlayerController::DispatchEvent()
{
}

void PlayerController::Start()
{
	_currentState = make_shared<PlayerUnitState>();
	*_currentState = PlayerUnitState::Stand;
	_jumpState = make_shared<JumpFlag>();

	_transform = GetGameObject()->GetTransform();
	_camera = MANAGER_SCENE()->GetCurrentScene()->GetCamera();
	_animator = GetGameObject()->GetChildByName(L"Model")->GetModelAnimator();
	_heightGetterCom = GetGameObject()->GetComponent<HeightGetter>();
	{
		if (_transform.lock())
		{
			Vec3 temPos = _transform.lock()->GetLocalPosition();
			temPos.y = _heightGetterCom.lock()->GetHeight();
			_transform.lock()->SetLocalPosition(temPos);
		}
	}
	_camDist = _camera.lock()->GetCamera()->GetCameraDistance();
	_camMaxDist = _camDist + FLT_EPSILON;
	_camMinDist = 25.f;

	AnimStateInit();
}

void PlayerController::FixedUpdate()
{
	if (_isAlive == false)
	{
		_isBattle = false;

	}

	if (_isBattle)
	{
		if (_battleTimer + FLT_EPSILON >= _battleTime)
		{
			_isBattle = false;
		}

		_battleTimer += MANAGER_TIME()->GetDeltaTime();
	}
}

void PlayerController::Update()
{
	PlayerInput();
	CameraMove();
}

void PlayerController::LateUpdate()
{
}