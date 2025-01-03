#pragma once
#include <map>
#include <DirectXMath.h>
#include "Timer.h"

enum
{
	PACKET_CONNECT = 1,
	PACKET_USER_INFO = 2,
	PACKET_MONSTER_INFO = 3,
	PACKET_MESSAGE = 4,
	PACKET_BATTLE = 5,
	PACKET_HOST = 6,
	PACKET_DISCONNECT = 99,
};

enum class PlayerUnitState
{
	None,
	Stand,
	FrontMove,
	FrontRightMove,
	FrontLeftMove,
	BackMove,
	BackRightMove,
	BackLeftMove,
	RightMove,
	LeftMove,
	Jump,
	Stun,
	Loot,
	Damaged,
	Death,
	Battle,
	Attack,
	Ability1,
	Ability2,
	End
};

enum class EnemyUnitState
{
	None,
	Stand,
	Walk,
	Run,
	Damaged,
	Death,
	Battle,
	Attack,
	Ability1,
	Ability2,
	End
};

enum class MapType
{
	None,
	Lobby,
	Dungeon,
	BossRoom
};

enum class SkillType
{
	NormalAttack,
	WhirlWind,
	IceArrow,
	Blizzard,
	Test_AllAttack,
};

enum class MonsterType
{
	None,
	CoreHound,
	MoltenGiant,
	BaronGeddon,
	Ragnaros
};

struct JumpFlag
{
	bool isJumpUP = false;
	bool isJumpFall = false;
	bool isJumEnd = false;
	bool isJump = false;
};

struct PACKET_CHARACTER_INFO
{
	//wstring _name;
	//std::wstring _strategyName;
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

struct MESSAGE
{
	char _messageBox[50];
};

struct PACKET_Player_INFO : public PACKET_CHARACTER_INFO
{
	uint32 _uid = 0;
	bool _isOnline = false;
	PlayerUnitState _animState = PlayerUnitState::Stand;
	JumpFlag _jumpFlag;
};

struct PACKET_Mob_INFO : public PACKET_CHARACTER_INFO
{
	uint32 _targetId = 0;
	MonsterType _monsterType = MonsterType::None;
	EnemyUnitState _animState = EnemyUnitState::Stand;
};

class ServerPacketHandler
{
public:
	static void HandlePacket(BYTE* buffer, int32 len);
	static void Handle_USER_INFO(BYTE* buffer, int32 len);
	static void Handle_MONSTER_INFO(BYTE* buffer, int32 len);
	static void Handle_MESSAGE(BYTE* buffer, int32 len);
	static void Handle_BATTLE(BYTE* buffer, int32 len);

	static SendBufferRef Make_USER_CONNECT();
	static SendBufferRef Make_USER_INFO(PACKET_Player_INFO userInfo, wstring name, bool otherPacket, bool isMapHost);
	static SendBufferRef Make_MONSTER_INFO(map<uint32, PACKET_Mob_INFO> mobInfo);
	static SendBufferRef Make_USER_DISCONNECT(uint64 uid);
	static SendBufferRef Make_MESSAGE(MESSAGE message);
	static SendBufferRef Make_HOST(bool isMapHost);
private:
	static map<uint64, std::wstring> _strategyName;
};
