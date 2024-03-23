#include "pch.h"
#include "Spawner.h"

#include "engine/EnemySoundController.h"
#include "engine/PlayerSoundController.h"
using namespace std::chrono;

Vec3 interpolate(double alpha, Vec3 targetPos, Vec3 prePos) {
	// ���� ������ ���� ���� ��ġ ���
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
		_spawnMapId = 0;
	}

	if (name == L"DungeonScene")
	{
		_spawnMapId = 1;
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

	_aiCon->SetAiSound(soundController);
	
	_otherPlayers.insert(std::make_pair(uid, _chr)); //map�� �𵨰� �ĺ�id �߰�
	MANAGER_SCENE()->GetCurrentScene()->Add(_chr);
	MANAGER_SCENE()->GetCurrentScene()->AddShadow(_chr);

}

void Spawner::SpawnOtherPlayers()
{
	
}

void Spawner::SpawnMonster(uint64 uid, uint32 monsterId, Vec3 spawnPos)
{
#pragma region Initialize
	shared_ptr<Shader> _shader = MANAGER_RESOURCES()->GetResource<Shader>(L"Default");

	// monsterId : 0. CoreHound    1. MoltenGiant    2. BaronGeddon
	shared_ptr<EnemyUnit> _chr;
	switch (monsterId)
	{
	case 0:
		_chr = make_shared<CoreHound>();
		break;
	case 1:
		_chr = make_shared<CoreHound>();
		break;
	case 2:
		_chr = make_shared<BaronGeddon>();
		break;
	default:
		_chr = make_shared<CoreHound>();
		break;
	}
	_chr->Awake();
	_aiCon = make_shared<AIController>();
	_aiCon->SetAIType(AIType::EnemyUnit);
	_chr->AddComponent(_aiCon);
	_chr->AddComponent(make_shared<CharacterInfo>());
	_chr->Start();
	_chr->GetTransform()->SetLocalPosition(spawnPos);

	_monsters.insert(std::make_pair(uid, _chr)); //map�� �𵨰� �ĺ�id �߰�
	MANAGER_SCENE()->GetCurrentScene()->Add(_chr);
	MANAGER_SCENE()->GetCurrentScene()->AddShadow(_chr);
#pragma endregion
}

void Spawner::SpawnMonsters()
{
	
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
