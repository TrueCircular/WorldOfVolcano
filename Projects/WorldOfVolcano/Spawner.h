#pragma once

#include "Character.h"
#include "Player.h"
#include "ClientPacketHandler.h"
#include "engine\AIController.h"
#include "engine/Warrior.h"
#include "engine/CoreHound.h"
#include "engine/MoltenGiant.h"
#include "engine/Ragnaros.h"
#include "engine/BaronGeddon.h"
#include "engine/CharacterInfo.h"

enum
{
	Lobby = 0,
	Dungeon = 1,
	BossRoom = 2,
};

class Spawner : public GameObject
{
public:
	Spawner() = default;
	~Spawner() = default;
public:
	void Init();
	void Update();
public:
	//플레이어
	void SpawnOtherPlayer(uint64 uid, Vec3 spawnPos);
	void SpawnOtherPlayers();
	//몬스터
	void SpawnMonster(uint64 uid, MONSTER_INFO mobInfo);
	void SpawnMonsters();
	//호스트 스폰
	map<uint64, shared_ptr<GameObject>> GetMobList() { return _monsters; }
	int GetOtherPlayersSize() { return _otherPlayers.size(); }
	void Reset();
private:
	map<uint64, std::wstring> _preStrategyName;
	MapType _spawnMapType = MapType::None;
	shared_ptr<AIController> _aiCon;
	map<uint64, shared_ptr<GameObject>> _otherPlayers;
	map<uint64, shared_ptr<GameObject>> _monsters;
	bool ragnarosSpawned = false;
};