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

struct PACKET_CHARACTER_INFO
{
	uint32 _instanceId = 0;
	MapType _spawnMapType = MapType::Lobby;
	uint32 _maxHp = 1000;
	uint32 _maxMp = 1000;
	uint32 _hp = 1000;
	uint32 _mp = 1000;
	uint16 _atk = 100;
	uint16 _def = 100;
	uint16 _moveSpeed = 10;
	uint16 _aggroLevel = 100;
	float _attackRange = 15.0f;
	float _attackTime = 0.8f;
	float _traceRadius = 0.f;
	Vec3 _pos = { 0.0f, 0.0f, 0.0f };
	Vec3 _Rotate = { 0.0f, 0.0f, 0.0f };
	bool _isAlive = true;
	bool _isAttack = false;
	bool _isBattle = false;
	double _timeStamp = 0.0f;
};

struct PACKET_Player_INFO : public PACKET_CHARACTER_INFO
{
	uint32 _uid = 0;
	bool _isOnline = false;
	PlayerUnitState _animState = PlayerUnitState::Stand;
	JumpFlag _jumpFlag;
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

	SendBufferRef Make_USER_INFO(Player_INFO userInfo, wstring name);
	SendBufferRef Make_MONSTER_INFO(MONSTER_INFO info);
	SendBufferRef Make_MESSAGE(MESSAGE message);
	SendBufferRef Make_BATTLE(Player_INFO attackerInfo, uint32 targerId);


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