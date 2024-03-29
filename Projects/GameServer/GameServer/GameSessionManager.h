#pragma once

#include "GameSession.h"
#include "ServerPacketHandler.h"

#include <random>
#include <cfloat>

class GameSession;

using GameSessionRef = std::shared_ptr<GameSession>;

enum
{
	Lobby = 0,
	Dungeon = 1,
	BossRoom = 2,
};

class GameSessionManager
{
public:
	void Add(GameSessionRef session);
	void Remove(GameSessionRef session);
	void Broadcast(SendBufferRef sendBuffer);
	Set<GameSessionRef> GetSessionsRef() { return _sessions; }
	//플레이어
	map<uint32, PACKET_Player_INFO>& GetUserInfoList() { return _userInfoList; }
	void UpdateUserInfo(PACKET_Player_INFO info);
	//몬스터
	void GenerateMobList();
	map<uint32, PACKET_Mob_INFO>& GetMobInfoList() { return _mobInfoList; }
	void UpdateMobInfo(PACKET_Mob_INFO info);
	void ClearMobInfoList() { _mobInfoList.clear(); }
	void CheckAndResetMonster();

	void MonsterBattleCalculate(float damage, uint32 tgtId);
private:
	float attackTime = 1.5f;
	float attackTimer = 0.0f;
private:
	USE_LOCK;
	Set<GameSessionRef> _sessions;
private:
	uint64 sessionIdCount = 0;
	bool hostExist = false;
private:
	map<uint32, PACKET_Mob_INFO> _mobInfoList;
	map<uint32, PACKET_Player_INFO> _userInfoList;
};

extern GameSessionManager GSessionManager;
