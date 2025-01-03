#pragma once
#include "Character.h"
#include "Player.h"
#include "Monster.h"

enum
{
	PACKET_CONNECT = 1,
	PACKET_USER_INFO = 2,
	PACKET_MONSTER_INFO = 3,
	PACKET_MESSAGE = 4,
	PACKET_BATTLE = 5,
	PACKET_DISCONNECT = 99,
};

class ClientPacketHandler
{
public:
	static ClientPacketHandler& Instance() {
		static ClientPacketHandler instance;
		return instance;
	}

	void HandlePacket(BYTE* buffer, int32 len);
	void Handle_USER_INFO(BYTE* buffer, int32 len);
	void Handle_MONSTER_INFO(BYTE* buffer, int32 len);
	void Handle_MESSAGE(BYTE* buffer, int32 len);
	void Handle_USER_DISCONNECT(BYTE* buffer, int32 len);

	SendBufferRef Make_USER_INFO(Player_INFO userInfo);
	SendBufferRef Make_MONSTER_INFO(MONSTER_INFO info);
	SendBufferRef Make_MESSAGE(MESSAGE message);
	SendBufferRef Make_BATTLE(Player_INFO attackerInfo, uint32 targerId, SkillType skillType);


	Player_INFO GetUserInfo() { return _userInfo; }
	map<uint64, MONSTER_INFO> GetMobInfoList() { return _mobInfoList; }
	map<uint64, Player_INFO> GetOtherUserInfoMap()
	{
		std::lock_guard<std::mutex> lock(_mutex);
		return otherUserInfoMap;
	}

	void EraseOtherUserInfoMap(uint64 uid)
	{
		auto it = otherUserInfoMap.find(uid);

		if (it != otherUserInfoMap.end()) {
			otherUserInfoMap.erase(it);
		}
	}

	void EraseMonster(uint64 uid)
	{
		auto it = _mobInfoList.find(uid);

		if (it != _mobInfoList.end()) {
			_mobInfoList.erase(it);
		}
	}

private:
	ClientPacketHandler() = default;
	~ClientPacketHandler() = default;

	Player_INFO _userInfo;
	map<uint64, Player_INFO> otherUserInfoMap;
	std::mutex _mutex;
	map<uint64, MONSTER_INFO> _mobInfoList;
};