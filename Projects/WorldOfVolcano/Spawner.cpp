#include "pch.h"
#include "Spawner.h"

#include "engine/EnemySoundController.h"
#include "engine/PlayerSoundController.h"
#include "engine\EquipmentSlot.h"
#include "engine/StrategyFactory.h"
#include "engine/AbilitySlot.h"
#include "engine/UnitFSM.h"
#include "engine/UnitStrategy.h"

using namespace std::chrono;

Vec3 interpolate(double alpha, Vec3 targetPos, Vec3 prePos) {
	// 선형 보간을 통한 실제 위치 계산
	float interpolatedX = (1.0 - alpha) * prePos.x + alpha * targetPos.x;
	float interpolatedY = (1.0 - alpha) * prePos.y + alpha * targetPos.y;
	float interpolatedZ = (1.0 - alpha) * prePos.z + alpha * targetPos.z;

	return Vec3{ interpolatedX, interpolatedY, interpolatedZ };
}

float QuatToEulerAngleY(Quaternion q)
{
	Vec3 angle;

	//y pitch
	double sinp = std::sqrt(1 + 2 * (q.w * q.y - q.x * q.z));
	double cosp = std::sqrt(1 - 2 * (q.w * q.y - q.x * q.z));
	angle.y = std::atan2(sinp, cosp) - PI;

	return angle.y;
}

float IsPlayerInRanger(const Vec3& playerPos, const Vec3& monsterPos)
{
	Vec3 diff = DirectX::XMVectorSubtract(playerPos, monsterPos);
	float distance = DirectX::XMVectorGetX(DirectX::XMVector3Length(diff));

	return distance;
}

void Spawner::Init()
{
	Reset();

	wstring name = MANAGER_SCENE()->GetCurrentScene()->GetSceneName();

	if (name == L"BaseScene")
	{
		_spawnMapType = MapType::Lobby;
	}

	if (name == L"DungeonScene")
	{
		_spawnMapType = MapType::Dungeon;
	}

	if (name == L"BossScene")
	{
		_spawnMapType = MapType::BossRoom;
	}
}

void Spawner::Update()
{
	SpawnOtherPlayers();
	SpawnMonsters();
}

void Spawner::SpawnOtherPlayer(uint64 uid, Vec3 spawnPos)
{
	shared_ptr<Shader> _shader = MANAGER_RESOURCES()->GetResource<Shader>(L"Default");

	shared_ptr<Warrior> _chr = make_shared<Warrior>();
	_chr->Awake();
	_aiCon = make_shared<AIController>();
	_aiCon->SetAIType(AIType::PlayableUnit);
	_chr->AddComponent(_aiCon);
	_chr->Start();
	_chr->GetTransform()->SetLocalPosition(spawnPos);
	//DOTO FOR TEMPLATE
	shared_ptr< PlayerSoundController> soundController = make_shared<PlayerSoundController>();

	soundController->Set(_chr->GetTransform());
	shared_ptr<Sounds> bgm2 = MANAGER_RESOURCES()->GetResource<Sounds>(L"Warrior_Attack");
	soundController->SetSound(PlayerAnimType::Attack1, bgm2);

	bgm2 = MANAGER_RESOURCES()->GetResource<Sounds>(L"Warrior_Attack2");
	soundController->SetSound(PlayerAnimType::Attack2, bgm2);

	bgm2 = MANAGER_RESOURCES()->GetResource<Sounds>(L"Warrior_Damaged");
	soundController->SetSound(PlayerAnimType::Damaged, bgm2);

	bgm2 = MANAGER_RESOURCES()->GetResource<Sounds>(L"Warrior_Death");
	soundController->SetSound(PlayerAnimType::Death, bgm2);

	//_aiCon->SetAiSound(soundController);
	
	_otherPlayers.insert(std::make_pair(uid, _chr)); //map에 모델과 식별id 추가
	MANAGER_SCENE()->GetCurrentScene()->Add(_chr);
	MANAGER_SCENE()->GetCurrentScene()->AddShadow(_chr);

}

void Spawner::SpawnOtherPlayers()
{
	for (const auto& pair : ClientPacketHandler::Instance().GetOtherUserInfoMap()) {
		if (pair.second._spawnMapType != _spawnMapType)
		{
			//내 Map정보가 아니면 스킵
			auto it = _otherPlayers.find(pair.first);

			if (it != _otherPlayers.end())
			{
				MANAGER_SCENE()->GetCurrentScene()->Remove(it->second);
			}

			continue;
		}
		//다른플레이어 위치 동기화
		auto it = _otherPlayers.find(pair.first);
		// it : first : uint64 / second : shared_ptr<LSkinningModel>
		if (it != _otherPlayers.end())
		{
			// 다른플레이어가 이미 있다.
			if (pair.second._isOnline == false)
			{
				//접속중이 아니라면 다른플레이어 목록에서 삭제
				MANAGER_SCENE()->GetCurrentScene()->Remove(it->second);
				_otherPlayers.erase(it);
				ClientPacketHandler::Instance().EraseOtherUserInfoMap(pair.first);
				//주의 : 이거 삭제한 이후로 얘 건들면 nullptr 뜰걸로 예상됨
			}
			else
			{
				// 보간을 위한 시간 계산 (0.0에서 1.0 사이의 값)
				auto calcTime = high_resolution_clock::now() - seconds(static_cast<int>(pair.second._timeStamp));
				auto durationSec = duration_cast<duration<double>>(calcTime.time_since_epoch()).count();
				double alpha = fmin(1.0, durationSec / 1.0);
				Vec3 target = pair.second._pos;
				Vec3 pos = it->second->GetTransform()->GetPosition();
				Vec3 rot = it->second->GetTransform()->GetLocalRotation();

				Vec3 direction = target - pos;
				pos += interpolate(alpha, direction, Vec3(0.0f, 0.0f, 0.0f)) * pair.second._moveSpeed * MANAGER_TIME()->GetDeltaTime();

				//회전 보간 계산
				Vec3 targetRot = pair.second._Rotate;
				Quaternion startRotation = Quaternion::CreateFromYawPitchRoll(rot.y, rot.x, rot.z);
				Quaternion endRotation = Quaternion::CreateFromYawPitchRoll(targetRot.y, 0.0f, 0.0f);
				Quaternion calcRot = Quaternion::Slerp(startRotation, endRotation, alpha);
				rot.y = QuatToEulerAngleY(calcRot);

				it->second->GetTransform()->SetPosition(pos);
				it->second->GetTransform()->SetLocalRotation(targetRot);
				it->second->GetComponent<AIController>()->SetUnitState(pair.second._animState);
				it->second->GetComponent<AIController>()->SetJumpState(pair.second._jumpFlag);
			}
		}
		else
		{
			//다른플레이어 처음 등장시 스폰
			SpawnOtherPlayer(pair.first, pair.second._pos);
			cout << "find not key, new player spawn" << endl;
		}
	}
}

void Spawner::SpawnMonster(uint64 uid, MONSTER_INFO mobInfo)
{
#pragma region Initialize
	shared_ptr<Shader> _shader = MANAGER_RESOURCES()->GetResource<Shader>(L"Default");

	// monsterId : 0. CoreHound    1. MoltenGiant    2. BaronGeddon
	auto _chr = make_shared<EnemyUnit>();;
	switch (mobInfo._monsterType)
	{
	case MonsterType::CoreHound:
		_chr = make_shared<CoreHound>();
		break;
	case MonsterType::MoltenGiant:
		_chr = make_shared<MoltenGiant>();
		break;
	case MonsterType::BaronGeddon:
		_chr = make_shared<BaronGeddon>();
		break;
	case MonsterType::Ragnaros:
		_chr = make_shared<Ragnaros>();
		break;
	default:
		break;
	}

	//Init
	{
		_chr->Awake();
		_chr->SetCharacterController(make_shared<AIController>(), AIType::EnemyUnit);
		_chr->GetComponent<AIController>()->SetIsAiHost(ClientPacketHandler::Instance().GetIsMapHost());

		switch (mobInfo._monsterType)
		{
		case MonsterType::CoreHound:
			_chr->GetComponent<AIController>()->SetFsmStrategyList(StrategyFactory::GetStrategyList<CoreHound>());
			break;
		case MonsterType::MoltenGiant:
			_chr->GetComponent<AIController>()->SetFsmStrategyList(StrategyFactory::GetStrategyList<MoltenGiant>());
			break;
		case MonsterType::BaronGeddon:
			_chr->GetComponent<AbilitySlot>()->SetController(_chr->GetComponent<AIController>());
			_chr->GetComponent<AIController>()->SetFsmStrategyList(StrategyFactory::GetStrategyList<BaronGeddon>());
			break;
		case MonsterType::Ragnaros:
			_chr->GetComponent<AIController>()->SetFsmStrategyList(StrategyFactory::GetStrategyList<Ragnaros>());
			break;
		default:
			break;
		}
		
		_chr->SetSpwanPosition(mobInfo._pos);
		_chr->GetComponent<CharacterInfo>()->SetCharacterInfo(mobInfo);
		_chr->Start();

		_monsters.insert(std::make_pair(uid, _chr)); //map에 모델과 식별id 추가
		MANAGER_SCENE()->GetCurrentScene()->Add(_chr);
		MANAGER_SCENE()->GetCurrentScene()->AddShadow(_chr);
	}
#pragma endregion
}

void Spawner::SpawnMonsters()
{
	for (auto pair : ClientPacketHandler::Instance().GetMobInfoList()) {
		if (pair.second._spawnMapType != _spawnMapType)
		{
			//내 Map정보가 아니면 스킵
			auto it = _monsters.find(pair.first);

			if (it != _monsters.end())
			{
				MANAGER_SCENE()->GetCurrentScene()->Remove(it->second);
			}

			continue;
		}

		auto it = _monsters.find(pair.first);

		if (it != _monsters.end())
		{
			if (pair.second._isAlive == false)
			{
				//주의 : 이거 삭제한 이후로 얘 건들면 nullptr 뜰걸로 예상됨
				it->second->GetComponent<AIController>()->notifyEnemyDeath();
				//it->second->GetComponent<AIController>()->SetUnitState(PlayerUnitState::Death); <- 동작수정
				it->second->GetComponent<CharacterInfo>()->SetDefaultCharacterInfo(pair.second);
				if (it->second->GetComponent<CharacterInfo>()->GetCharacterInfo()._name == L"BaronGeddon")
				{
					MANAGER_IMGUI()->NotifyGeddonDeath();
				}
				_monsters.erase(it);
				ClientPacketHandler::Instance().EraseMonster(pair.first);
			}
			else
			{
				if (ClientPacketHandler::Instance().GetIsMapHost())
				{
					for (auto mob : _monsters)
					{
						CHARACTER_INFO chrInfo = mob.second->GetComponent<CharacterInfo>()->GetCharacterInfo();
						MANAGER_EVENT()->SetIsMapHost(true);
						auto unitFsm = mob.second->GetComponent<AIController>()->GetUnitFsm();
						if (unitFsm)
						{
							auto strategy = unitFsm->GetStrategy();
							if (strategy)
							{
								weak_ptr<Transform> transform = strategy->GetWeakTransform();
								if (auto sharedTransform = transform.lock())
								{
									chrInfo._pos = transform.lock()->GetLocalPosition();
									chrInfo._Rotate = transform.lock()->GetLocalRotation();
									mob.second->GetComponent<CharacterInfo>()->SetCharacterInfo(chrInfo);
								}
								
							}
						};
					}
				}
				else
				{
					auto calcTime = high_resolution_clock::now() - seconds(static_cast<int>(pair.second._timeStamp));
					auto durationSec = duration_cast<duration<double>>(calcTime.time_since_epoch()).count();
					double alpha = fmin(1.0, durationSec / 1.0);

					Vec3 pos;
					Vec3 rot;

					weak_ptr<Transform> transform = it->second->GetComponent<AIController>()->GetUnitFsm()->GetStrategy()->GetWeakTransform().lock();
					if (auto sharedTransform = transform.lock())
					{
						pos = transform.lock()->GetLocalPosition();
						rot = transform.lock()->GetLocalRotation();
					}

					Vec3 target = pair.second._pos;
					Vec3 direction = target - pos;

					pos += interpolate(alpha, direction, Vec3(0.0f, 0.0f, 0.0f)) * MANAGER_TIME()->GetDeltaTime();

					//회전 보간 계산
					Vec3 targetRot = pair.second._Rotate;
					/*Quaternion startRotation = Quaternion::CreateFromYawPitchRoll(rot.y, rot.x, rot.z);
					Quaternion endRotation = Quaternion::CreateFromYawPitchRoll(targetRot.y, 0.0f, 0.0f);
					Quaternion calcRot = Quaternion::Slerp(startRotation, endRotation, alpha);
					rot.y = QuatToEulerAngleY(calcRot);*/

					pair.second._pos = pos;
					//it->second->GetTransform()->SetLocalRotation(targetRot);
					
					CHARACTER_INFO chrInfo = it->second->GetComponent<CharacterInfo>()->GetCharacterInfo();
					wstring stgName = ClientPacketHandler::Instance().GetStrategyName(chrInfo._instanceId);

					it->second->GetComponent<CharacterInfo>()->SetCharacterInfo(pair.second);
					it->second->GetComponent<AIController>()->GetUnitFsm()->GetStrategy()->UpdateInfo(pair.second); // 정보 동기화

					auto preStg = _preStrategyName.find(chrInfo._instanceId);
					if (preStg != _preStrategyName.end())
					{
						if (preStg->second != stgName)
						{
							it->second->GetComponent<AIController>()->SetCurrentFsmStrategy(preStg->second, stgName);
							preStg->second = stgName;
						}
					}
					else
					{
						_preStrategyName.insert(make_pair(chrInfo._instanceId, stgName));
					}
				}
			}
		}
		else
		{
			//다른플레이어 처음 등장시 스폰
			if (pair.second._isAlive == true)
			{
				SpawnMonster(pair.first, pair.second);
			}
		}
	}
}

void Spawner::Reset()
{
	for (auto pair : _otherPlayers)
	{
		std::shared_ptr<GameObject> gameObject = pair.second;
		if (gameObject) {
			MANAGER_SCENE()->GetCurrentScene()->Remove(gameObject);
		}
	}

	for (auto pair : _monsters)
	{
		std::shared_ptr<GameObject> gameObject = pair.second;
		if (gameObject) {
			MANAGER_SCENE()->GetCurrentScene()->Remove(gameObject);
		}
	}

	_otherPlayers.clear();
	_monsters.clear();
}
