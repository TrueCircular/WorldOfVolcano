#include "pch.h"
#include "PlayerController.h"

#include "AIController.h"
#include "PlayerAnimState.h"
#include "HeightGetter.h"
#include "CharacterInfo.h"
#include "UnitFSM.h"
#include "AbilitySlot.h"
#include "Utils.h"
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

	*_currentState = PlayerUnitState::Stand;
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
			_playerRot.y += ::XMConvertToRadians(deltaX) * _dt * (10.f);
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
		for (auto& animState : _animStateList)
		{
			if (animState->GetStateAnimtype() == animType)
			{
				_animState = animState;
				_animState->Enter(shared_from_this());
				return true;
			}
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
		CameraMove();
		PlayerAttack();
		PlayerAbility1();
		PlayerAbility2();
		PlayerAbility3();
		PlayerAbility4();
	}

	_animState->Update();
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

	if (!MANAGER_INPUT()->GetButton(KEY_TYPE::W) &&
		!MANAGER_INPUT()->GetButton(KEY_TYPE::S) &&
		!MANAGER_INPUT()->GetButton(KEY_TYPE::A) &&
		!MANAGER_INPUT()->GetButton(KEY_TYPE::D))
	{
		if (_isBattle)
		{
			*_currentState = PlayerUnitState::Battle;
		}
		else
		{
			*_currentState = PlayerUnitState::Stand;
		}
	}
	//왼쪽
	else if (MANAGER_INPUT()->GetButton(KEY_TYPE::A))
	{
		*_currentState = PlayerUnitState::LeftMove;
		{
			_isSlow = false;

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
			_isSlow = false;

			_playerPos += _playerRight * _currentSpeed * _dt;
			_transform.lock()->SetPosition(_playerPos);
			_camera.lock()->GetCamera()->SetCameraIsRotationAround(false);
			_camera.lock()->GetCamera()->SetCameraRotationYaw(_playerRot.y);
		}
	}
	//앞
	if (MANAGER_INPUT()->GetButton(KEY_TYPE::W))
	{
		_isSlow = false;
		*_currentState = PlayerUnitState::FrontMove;

		_playerPos += _playerForward * _currentSpeed * _dt;
		_transform.lock()->SetPosition(_playerPos);
		_camera.lock()->GetCamera()->SetCameraIsRotationAround(false);
		_camera.lock()->GetCamera()->SetCameraRotationYaw(_playerRot.y);
	}
	//뒤
	else if (MANAGER_INPUT()->GetButton(KEY_TYPE::S))
	{
		_isSlow = true;
		*_currentState = PlayerUnitState::BackMove;

		_playerPos -= _playerForward * _currentSpeed * _dt;
		_transform.lock()->SetPosition(_playerPos);
		_camera.lock()->GetCamera()->SetCameraIsRotationAround(false);
		_camera.lock()->GetCamera()->SetCameraRotationYaw(_playerRot.y);
	}

	PlayerJump();

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
	if (_isAttack == true)
	{
		if (_attackTimer > _attackTime)
		{
			_isAttack = false;
			_attackTimer = 0.f;
		}
		else
		{
			_attackTimer += MANAGER_TIME()->GetDeltaTime();
		}
	}

	if (_isAttack == false && _jumpState->isJump == false)
	{
		if (_pickedObj != nullptr)
		{
			auto pickTransform = _pickedObj->GetTransform();
			Vec3 myPos = _transform.lock()->GetPosition();
			Vec3 pickPos = pickTransform->GetPosition();
			pickPos.y = myPos.y;
			float dist = Vec3::Distance(myPos, pickPos);

			if (dist < _attackRange + FLT_EPSILON)
			{
				if (_pickedObj->GetComponent<AIController>()->GetUnitFsm()->GetStrategy()->_type != UnitStrategyType::Dead)
				{
					if (MANAGER_INPUT()->GetButtonDown(KEY_TYPE::RBUTTON))
					{
						auto pickController = _pickedObj->GetComponent<AIController>();

						if (pickController != nullptr)
						{
							_isBattle = true;
							_isAttack = true;

							float damage = _unitInfo.lock()->GetCharacterInfo()._atk;
							pickController->TakeDamage(GetGameObject(), damage);
							MANAGER_IMGUI()->UpdatePicked(true, _pickedObj);

							*_currentState = PlayerUnitState::Attack;

							::srand(time(NULL));

							int selectAttack = rand() % 2;

							if (selectAttack == 0)
							{
								SetAnimState(PlayerAnimType::Attack1);
							}
							else
							{
								SetAnimState(PlayerAnimType::Attack2);
							}
						}
					}
				}
			}
		}
	}
}

void PlayerController::PlayerAbility1()
{
	if (MANAGER_INPUT()->GetButtonDown(KEY_TYPE::KEY_1))
	{
		_abilitySlot.lock()->ExecuteAbility(0, _pickedObj);
	}
}

void PlayerController::PlayerAbility2()
{
	if (MANAGER_INPUT()->GetButtonDown(KEY_TYPE::KEY_2))
	{
		_abilitySlot.lock()->ExecuteAbility(1, _pickedObj);
	}
}
void PlayerController::PlayerAbility3()
{
	if (MANAGER_INPUT()->GetButtonDown(KEY_TYPE::KEY_3))
	{
		_abilitySlot.lock()->ExecuteAbility(2, _pickedObj);
	}
}
void PlayerController::PlayerAbility4()
{
	if (MANAGER_INPUT()->GetButtonDown(KEY_TYPE::KEY_4))
	{
		_abilitySlot.lock()->ExecuteAbility(3, _pickedObj);
	}
}

void PlayerController::PlayerPicking()
{
	//if (MANAGER_INPUT()->GetButtonDown(KEY_TYPE::LBUTTON))
	//{
	//	int32 mx = MANAGER_INPUT()->GetScreenMousePos().x;
	//	int32 my = MANAGER_INPUT()->GetScreenMousePos().y;

	//	auto pickObj = MANAGER_SCENE()->GetCurrentScene()->Pick(mx, my);

	//	if (pickObj && pickObj->GetName() == L"MagniBronzebeard")
	//	{
	//		MANAGER_IMGUI()->BeginDialogue();
	//	}
	//	else if (pickObj && pickObj->GetName() != L"") //어떤 타입이든 인식할수 있게 수정해야할 필요 있음
	//	{
	//		_isPicked = true;
	//		_pickedObj = pickObj;
	//	}
	//	else
	//	{
	//		MANAGER_IMGUI()->UpdatePicked(false);
	//		_isPicked = false;
	//	}
	//}

	//if (_isPicked)
	//{
	//	_pickedInfo = _pickedObj->GetComponent<CharacterInfo>()->GetDefaultCharacterInfo();

	//	MANAGER_IMGUI()->UpdatePicked(true, _pickedInfo._maxHp, _pickedInfo._hp, _pickedObj->GetName());
	//	if (_pickedInfo._hp == 0)
	//	{
	//		MANAGER_IMGUI()->UpdatePicked(false);
	//		_isPicked = false;
	//		//MANAGER_SCENE()->GetCurrentScene()->Remove(_pickedObj);
	//	}

	//	if (MANAGER_INPUT()->GetButtonDown(KEY_TYPE::RBUTTON))
	//	{
	//		int32 mx = MANAGER_INPUT()->GetScreenMousePos().x;
	//		int32 my = MANAGER_INPUT()->GetScreenMousePos().y;

	//		auto pickObj = MANAGER_SCENE()->GetCurrentScene()->Pick(mx, my);

	//		if (pickObj && pickObj->GetName() != L"") //어떤 타입이든 인식할수 있게 수정해야할 필요 있음
	//		{
	//			Vec3 diff = DirectX::XMVectorSubtract(_playerPos, _pickedInfo._pos);
	//			float distance = DirectX::XMVectorGetX(DirectX::XMVector3Length(diff));

	//			if (distance <= 15.0f)
	//			{
	//				if (_isAttack == false)
	//				{
	//					//_attackQueue.push(SkillType::NormalAttack);
	//				}
	//				_isAttack = true;
	//				_isBattle = true;
	//			}
	//		}
	//	}
	//}
}

void PlayerController::PlayerTargetControll()
{
	if (MANAGER_INPUT()->GetButtonDown(KEY_TYPE::LBUTTON))
	{
		int32 mx = MANAGER_INPUT()->GetScreenMousePos().x;
		int32 my = MANAGER_INPUT()->GetScreenMousePos().y;

		auto pickObj = MANAGER_SCENE()->GetCurrentScene()->Pick(mx, my);

		if (pickObj && pickObj->GetName() == L"MagniBronzebeard")
		{
			MANAGER_IMGUI()->BeginDialogue();
		}
		else if (pickObj != nullptr && pickObj->GetObjectType() == ObjectType::EnemyUnit)
		{
			_pickedObj = pickObj;
			MANAGER_IMGUI()->UpdatePicked(true, _pickedObj);
		}
		else
		{
			MANAGER_IMGUI()->UpdatePicked(false);
			_isPicked = false;
			_pickedObj = nullptr;
		}
	}
}

void PlayerController::KeyStateCheck()
{
	if (_jumpState->isJump == false)
	{
		if (_isAttack == false)
		{
			if (MANAGER_INPUT()->GetButton(KEY_TYPE::W) &&
				MANAGER_INPUT()->GetButton(KEY_TYPE::A))
			{
				*_currentState = PlayerUnitState::FrontLeftMove;
			}
			else if (MANAGER_INPUT()->GetButton(KEY_TYPE::S) &&
				MANAGER_INPUT()->GetButton(KEY_TYPE::A))
			{
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
				*_currentState = PlayerUnitState::BackRightMove;
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

void PlayerController::InitController()
{
	//Character;
	{
		_playerPos = { 0,0,0 };
		_playerRot = { 0,0,0 };
		_playerForward = { 0,0,0 };
		_playerRight = { 0,0,0 };
		_playerUp = { 0,0,0 };
		_jumpStartPos = { 0,0,0 };
		_jumpUpMaxPos = { 0,0,0 };
		_jumpUpDir = { 0,1,0 };
		_jumpDownDir = { 0,-1,0 };
		_jumpPower = 20.f;
		_jumpState->Init();

		_defaultSpeed = 40.f;
		_currentSpeed = 40.f;
		_slowSpeed = 20.f;
		_dt = 0.f;
		_isSlow = false;
		_unitInfo.lock()->InitInformation();
	}

	//Picking
	{
		_isPicked = false;
		_pickedObj = nullptr;
	}

	//Animation State & Unit State
	{
		*_currentState = PlayerUnitState::Stand;
		_animState = _animStateList[0];
		_animState->Enter(shared_from_this());
	}

	//Camera
	{
		//_camPos = Vec3(0.f);
		_camRot = Vec3(0.f);
		_camDist = _camera.lock()->GetCamera()->GetCameraDistance();
		_camMaxDist = _camDist + FLT_EPSILON;
		_camMinDist = 25.f;
	}

}

void PlayerController::TakeDamage(const shared_ptr<GameObject>& sender, float damage)
{
	if (_unitInfo.lock() == nullptr)
		return;
	else
	{
		if (_isAlive)
		{
			//Damage Calculate
			{
				auto myInfo = _unitInfo.lock()->GetCharacterInfo();
				float defEff = pow(myInfo._def * log(2), 0.5) * 3;
				float calDamage = (damage * (1 - defEff / 100)) * Utils::Randstep(0.8, 1.2);
				float finalHp = myInfo._hp - calDamage;

				if (finalHp < 1.f + FLT_EPSILON)
				{
					finalHp = 0;

					myInfo._hp = (uint32)finalHp;
					_unitInfo.lock()->SetCharacterInfo(myInfo);
					MANAGER_IMGUI()->UpdatePicked(true, GetGameObject());

					*_currentState = PlayerUnitState::Death;
					SetAnimState(PlayerAnimType::Death);
				}
				else
				{
					myInfo._hp = (uint32)finalHp;
					_unitInfo.lock()->SetCharacterInfo(myInfo);
					MANAGER_IMGUI()->UpdatePicked(true, GetGameObject());

					if (_animState->GetStateAnimtype() != PlayerAnimType::Damaged &&
						_animState->GetStateAnimtype() != PlayerAnimType::Death &&
						_animState->GetStateAnimtype() != PlayerAnimType::Casting &&
						_animState->GetStateAnimtype() != PlayerAnimType::JumpStart &&
						_animState->GetStateAnimtype() != PlayerAnimType::JumpFall &&
						_animState->GetStateAnimtype() != PlayerAnimType::JumpEnd &&
						_animState->GetStateAnimtype() != PlayerAnimType::Attack1 &&
						_animState->GetStateAnimtype() != PlayerAnimType::Attack2 &&
						_animState->GetStateAnimtype() != PlayerAnimType::Ability1 &&
						_animState->GetStateAnimtype() != PlayerAnimType::Ability2)
					{
						*_currentState = PlayerUnitState::Damaged;
						SetAnimState(PlayerAnimType::Damaged);
					}
				}
			}
		}
	}
}

void PlayerController::DeadEvent()
{
	*_currentState = PlayerUnitState::Stand;
}

void PlayerController::Respawn(const Vec3& spawnPos)
{
	if (spawnPos != Vec3::Zero)
	{
		_isAlive = true;
		_isAttack = false;
		_isBattle = false;
		_isAbility = false;
		_animator.lock()->SetLoop(true);
		_animator.lock()->SetPlay(true);

		InitController();
		_transform.lock()->SetPosition(spawnPos);
		MANAGER_IMGUI()->UpdatePicked(true, GetGameObject());
	}
	else
	{
		_isAlive = true;
		_isAttack = false;
		_isBattle = false;
		_isAbility = false;
		_animator.lock()->SetLoop(true);
		_animator.lock()->SetPlay(true);

		InitController();
		_transform.lock()->SetPosition(_spawnPos);
		MANAGER_IMGUI()->UpdatePicked(true, GetGameObject());
	}
}

void PlayerController::Start()
{
	{
		_currentState = make_shared<PlayerUnitState>();
		_jumpState = make_shared<JumpFlag>();
	}

	{
		_transform = GetGameObject()->GetTransform();
		_camera = MANAGER_SCENE()->GetCurrentScene()->GetCamera();
		_animator = GetGameObject()->GetChildByName(L"Model")->GetModelAnimator();
		_heightGetterCom = GetGameObject()->GetComponent<HeightGetter>();
		_abilitySlot = GetGameObject()->GetComponent<AbilitySlot>();

		_unitInfo = GetGameObject()->GetComponent<CharacterInfo>();
		_attackTime = _unitInfo.lock()->GetDefaultCharacterInfo()._attackTime;
		_attackRange = _unitInfo.lock()->GetDefaultCharacterInfo()._attackRange;
		_defaultSpeed = _unitInfo.lock()->GetDefaultCharacterInfo()._moveSpeed;
		_currentSpeed = _defaultSpeed;
		_slowSpeed = _defaultSpeed / 2;

		if (_transform.lock() && _heightGetterCom.lock() != nullptr)
		{
			Vec3 temPos = _transform.lock()->GetLocalPosition();
			temPos.y = _heightGetterCom.lock()->GetHeight();
			_transform.lock()->SetLocalPosition(temPos);
		}
	}

	{
		_camDist = _camera.lock()->GetCamera()->GetCameraDistance();
		_camMaxDist = _camDist + FLT_EPSILON;
		_camMinDist = 25.f;
	}


	AnimStateInit();
}

void PlayerController::FixedUpdate()
{
	if (_isAlive == false)
	{
		DeadEvent();
	}

	if (_isBattle)
	{
		if (_battleTimer + FLT_EPSILON > _battleTime)
		{
			_isBattle = false;
			_battleTimer = 0.f;
		}
		else
		{
			_battleTimer += MANAGER_TIME()->GetDeltaTime();
		}
	}

	PlayerTargetControll();
}

void PlayerController::Update()
{
	PlayerInput();
	if (_pickedObj != nullptr)
	{
		MANAGER_IMGUI()->UpdatePicked(true, _pickedObj);
	}
}

void PlayerController::LateUpdate()
{
}