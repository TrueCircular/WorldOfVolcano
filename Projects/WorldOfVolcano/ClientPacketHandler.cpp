#include "pch.h"
#include "ClientPacketHandler.h"
#include "BufferReader.h"
#include "BufferWriter.h"

#include "ObjectExporter2.h"
#include "engine/CharacterInfo.h"

void ClientPacketHandler::HandlePacket(BYTE* buffer, int32 len)
{
	BufferReader br(buffer, len);

	PacketHeader header;
	br >> header;

	switch (header.id)
	{
	case PACKET_USER_INFO:
		Handle_USER_INFO(buffer, len);
		break;
	case PACKET_MONSTER_INFO:
		Handle_MONSTER_INFO(buffer, len);
		break;
	case PACKET_MESSAGE:
		Handle_MESSAGE(buffer, len);
		break;
	case PACKET_BATTLE:
		break;
	case PACKET_HOST:
		Handle_HOST(buffer, len);
		break;
	case PACKET_DISCONNECT:
		Handle_USER_DISCONNECT(buffer, len);
		break;
	default:
		break;
	}
}

Player_INFO CopyPacketPlayerInfo(const PACKET_Player_INFO& playerInfo, wstring name) {
	Player_INFO realPlayerInfo;
	realPlayerInfo._name = name;
	realPlayerInfo._instanceId = playerInfo._instanceId;
	realPlayerInfo._spawnMapType = playerInfo._spawnMapType;
	realPlayerInfo._maxHp = playerInfo._maxHp;
	realPlayerInfo._maxMp = playerInfo._maxMp;
	realPlayerInfo._hp = playerInfo._hp;
	realPlayerInfo._mp = playerInfo._mp;
	realPlayerInfo._atk = playerInfo._atk;
	realPlayerInfo._def = playerInfo._def;
	realPlayerInfo._moveSpeed = playerInfo._moveSpeed;
	realPlayerInfo._aggroLevel = playerInfo._aggroLevel;
	realPlayerInfo._attackRange = playerInfo._attackRange;
	realPlayerInfo._attackTime = playerInfo._attackTime;
	realPlayerInfo._traceRadius = playerInfo._traceRadius;
	realPlayerInfo._pos = playerInfo._pos;
	realPlayerInfo._Rotate = playerInfo._Rotate;
	realPlayerInfo._isAlive = playerInfo._isAlive;
	realPlayerInfo._isAttack = playerInfo._isAttack;
	realPlayerInfo._isBattle = playerInfo._isBattle;
	realPlayerInfo._timeStamp = playerInfo._timeStamp;

	realPlayerInfo._uid = playerInfo._uid;
	realPlayerInfo._isOnline = playerInfo._isOnline;
	realPlayerInfo._animState = playerInfo._animState;
	realPlayerInfo._jumpFlag = playerInfo._jumpFlag;

	return realPlayerInfo;
}

MONSTER_INFO CopyPacketMonsterInfo(const PACKET_Mob_INFO& mobInfo, wstring name) {
	MONSTER_INFO realMobInfo;
	realMobInfo._name = name;
	realMobInfo._instanceId = mobInfo._instanceId;
	realMobInfo._spawnMapType = mobInfo._spawnMapType;
	realMobInfo._maxHp = mobInfo._maxHp;
	realMobInfo._maxMp = mobInfo._maxMp;
	realMobInfo._hp = mobInfo._hp;
	realMobInfo._mp = mobInfo._mp;
	realMobInfo._atk = mobInfo._atk;
	realMobInfo._def = mobInfo._def;
	realMobInfo._moveSpeed = mobInfo._moveSpeed;
	realMobInfo._aggroLevel = mobInfo._aggroLevel;
	realMobInfo._attackRange = mobInfo._attackRange;
	realMobInfo._attackTime = mobInfo._attackTime;
	realMobInfo._traceRadius = mobInfo._traceRadius;
	realMobInfo._pos = mobInfo._pos;
	realMobInfo._Rotate = mobInfo._Rotate;
	realMobInfo._isAlive = mobInfo._isAlive;
	realMobInfo._isAttack = mobInfo._isAttack;
	realMobInfo._isBattle = mobInfo._isBattle;
	realMobInfo._timeStamp = mobInfo._timeStamp;

	realMobInfo._monsterType = mobInfo._monsterType;
	realMobInfo._animState = mobInfo._animState;

	return realMobInfo;
}

void ClientPacketHandler::Handle_USER_INFO(BYTE* buffer, int32 len)
{
	BufferReader br(buffer, len);

	PacketHeader header;
	br >> header;

	PACKET_Player_INFO userInfo;
	br >> userInfo;

	wstring name;
	uint16 nameLen;
	br >> nameLen;
	name.resize(nameLen);
	br.Read((void*)name.data(), nameLen * sizeof(WCHAR));

	std::lock_guard<std::mutex> lock(_mutex);

	if (_userInfo._uid == userInfo._uid)
	{
		Player_INFO realUserInfo = CopyPacketPlayerInfo(userInfo, name);
		_userInfo = realUserInfo;
	}

	if (header.other == false)
	{
		Player_INFO realUserInfo = CopyPacketPlayerInfo(userInfo, name);
		_userInfo = realUserInfo;
	}
	else if (_userInfo._uid != userInfo._uid)
	{
		// uid가 이미 otherUserInfoMap에 있는지 확인
		auto it = otherUserInfoMap.find(userInfo._uid);

		if (it != otherUserInfoMap.end()) {
			// 이미 존재하는 키에 해당하는 값을 수정
			Player_INFO realUserInfo = CopyPacketPlayerInfo(userInfo, name);
			it->second = realUserInfo;
		}
		else {
			// 존재하지 않는 경우, 새로운 값을 삽입
			Player_INFO realUserInfo = CopyPacketPlayerInfo(userInfo, name);
			Player_INFO otherUserInfo = realUserInfo;
			otherUserInfo._isOnline = true;
			cout << "other uid : " << userInfo._uid << endl;
			cout << "other position : ( " << userInfo._pos.x << ", " << userInfo._pos.y << ", " << userInfo._pos.z << " )" << endl;
			otherUserInfoMap.insert(std::make_pair(userInfo._uid, otherUserInfo));
		}
	}
}

void ClientPacketHandler::Handle_MONSTER_INFO(BYTE* buffer, int32 len)
{
	BufferReader br(buffer, len);

	PacketHeader header;
	br >> header;

	//br의 커서가 끝에 도달할때까지 계속 뽑아냄
	std::lock_guard<std::mutex> lock(_mutex);

	while (br.ReadSize() < br.Size())
	{
		PACKET_Mob_INFO mobInfo;
		br >> mobInfo;

		wstring stgName;
		uint16 stgNameLen;
		br >> stgNameLen;
		stgName.resize(stgNameLen);
		br.Read((void*)stgName.data(), stgNameLen * sizeof(WCHAR));

		auto stg = _strategyName.find(mobInfo._instanceId);
		if (stg != _strategyName.end())
		{
			stg->second = stgName;
		}
		else
		{
			_strategyName.insert(make_pair(mobInfo._instanceId, stgName));
		}

		wstring name = L"";
		switch (mobInfo._monsterType)
		{
		case MonsterType::CoreHound:
			name = L"CoreHound";
			break;
		case MonsterType::MoltenGiant:
			name = L"MoltenGiant";
			break;
		case MonsterType::BaronGeddon:
			name = L"BaronGeddon";
			break;
		case MonsterType::Ragnaros:
			name = L"Ragnaros";
			break;
		default:
			break;
		}

		auto it = _mobInfoList.find(mobInfo._instanceId);
		if (it != _mobInfoList.end())
		{
			MONSTER_INFO realMobInfo = CopyPacketMonsterInfo(mobInfo, name);
			if (realMobInfo._hp == 0)
			{
				realMobInfo._isAlive = false;
			}
			it->second = realMobInfo;
		}
		else
		{
			MONSTER_INFO realMobInfo = CopyPacketMonsterInfo(mobInfo, name);
			_mobInfoList.insert(make_pair(mobInfo._instanceId, realMobInfo));
		}
	}
}
void ClientPacketHandler::Handle_MESSAGE(BYTE* buffer, int32 len)
{
	BufferReader br(buffer, len);

	PacketHeader header;
	br >> header;

	MESSAGE message;
	br >> message;

	MANAGER_IMGUI()->AddMessage(message._messageBox);
}

void ClientPacketHandler::Handle_HOST(BYTE* buffer, int32 len)
{
	BufferReader br(buffer, len);

	PacketHeader header;
	br >> header;

	_isMapHost = header.isMapHost;
	GenerateMobList();
}

void ClientPacketHandler::Handle_USER_DISCONNECT(BYTE* buffer, int32 len)
{
	
	BufferReader br(buffer, len);

	PacketHeader header;
	br >> header;

	uint64 uid;

	br >> uid;

	std::lock_guard<std::mutex> lock(_mutex);

	auto it = otherUserInfoMap.find(uid);

	if (it != otherUserInfoMap.end()) {
		it->second._isOnline = false;
	}
}

PACKET_Player_INFO CopyPlayerInfo(const Player_INFO& playerInfo) {
	PACKET_Player_INFO sendPlayerInfo;
	sendPlayerInfo._instanceId = playerInfo._instanceId;
	sendPlayerInfo._spawnMapType = playerInfo._spawnMapType;
	sendPlayerInfo._maxHp = playerInfo._maxHp;
	sendPlayerInfo._maxMp = playerInfo._maxMp;
	sendPlayerInfo._hp = playerInfo._hp;
	sendPlayerInfo._mp = playerInfo._mp;
	sendPlayerInfo._atk = playerInfo._atk;
	sendPlayerInfo._def = playerInfo._def;
	sendPlayerInfo._moveSpeed = playerInfo._moveSpeed;
	sendPlayerInfo._aggroLevel = playerInfo._aggroLevel;
	sendPlayerInfo._attackRange = playerInfo._attackRange;
	sendPlayerInfo._attackTime = playerInfo._attackTime;
	sendPlayerInfo._traceRadius = playerInfo._traceRadius;
	sendPlayerInfo._pos = playerInfo._pos;
	sendPlayerInfo._Rotate = playerInfo._Rotate;
	sendPlayerInfo._isAlive = playerInfo._isAlive;
	sendPlayerInfo._isAttack = playerInfo._isAttack;
	sendPlayerInfo._isBattle = playerInfo._isBattle;
	sendPlayerInfo._timeStamp = playerInfo._timeStamp;

	sendPlayerInfo._uid = playerInfo._uid;
	sendPlayerInfo._isOnline = playerInfo._isOnline;
	sendPlayerInfo._animState = playerInfo._animState;
	sendPlayerInfo._jumpFlag = playerInfo._jumpFlag;

	return sendPlayerInfo;
}

PACKET_Mob_INFO CopyMonsterInfo(const MONSTER_INFO& mobInfo) {
	PACKET_Mob_INFO sendMobInfo;
	sendMobInfo._instanceId = mobInfo._instanceId;
	sendMobInfo._spawnMapType = mobInfo._spawnMapType;
	sendMobInfo._maxHp = mobInfo._maxHp;
	sendMobInfo._maxMp = mobInfo._maxMp;
	sendMobInfo._hp = mobInfo._hp;
	sendMobInfo._mp = mobInfo._mp;
	sendMobInfo._atk = mobInfo._atk;
	sendMobInfo._def = mobInfo._def;
	sendMobInfo._moveSpeed = mobInfo._moveSpeed;
	sendMobInfo._aggroLevel = mobInfo._aggroLevel;
	sendMobInfo._attackRange = mobInfo._attackRange;
	sendMobInfo._attackTime = mobInfo._attackTime;
	sendMobInfo._traceRadius = mobInfo._traceRadius;
	sendMobInfo._pos = mobInfo._pos;
	sendMobInfo._Rotate = mobInfo._Rotate;
	sendMobInfo._isAlive = mobInfo._isAlive;
	sendMobInfo._isAttack = mobInfo._isAttack;
	sendMobInfo._isBattle = mobInfo._isBattle;
	sendMobInfo._timeStamp = mobInfo._timeStamp;

	sendMobInfo._monsterType = mobInfo._monsterType;
	sendMobInfo._animState = mobInfo._animState;

	return sendMobInfo;
}

SendBufferRef ClientPacketHandler::Make_USER_INFO(Player_INFO userInfo, wstring name)
{
	std::lock_guard<std::mutex> lock(_mutex);

	SendBufferRef sendBuffer = GSendBufferManager->Open(4096); //4kb
	BufferWriter bw(sendBuffer->Buffer(), sendBuffer->AllocSize());
	PacketHeader* header = bw.Reserve<PacketHeader>();

	PACKET_Player_INFO sendUserInfo = CopyPlayerInfo(userInfo);

	bw << sendUserInfo;

	bw << (uint16)name.size();
	bw.Write((void*)name.data(), name.size() * sizeof(WCHAR));

	header->size = bw.WriteSize();
	header->id = PACKET_USER_INFO;

	sendBuffer->Close(bw.WriteSize()); //사용한 길이만큼 닫아줌

	return sendBuffer;
}

SendBufferRef ClientPacketHandler::Make_MONSTER_INFO(MONSTER_INFO info, wstring strategyName)
{
	std::lock_guard<std::mutex> lock(_mutex);

	SendBufferRef sendBuffer = GSendBufferManager->Open(4096); //4kb
	BufferWriter bw(sendBuffer->Buffer(), sendBuffer->AllocSize());
	PacketHeader* header = bw.Reserve<PacketHeader>();

	PACKET_Mob_INFO sendMobInfo = CopyMonsterInfo(info);

	bw << sendMobInfo;

	bw << (uint16)strategyName.size();
	bw.Write((void*)strategyName.data(), strategyName.size() * sizeof(WCHAR));

	header->size = bw.WriteSize();
	header->id = PACKET_MONSTER_INFO;

	sendBuffer->Close(bw.WriteSize()); //사용한 길이만큼 닫아줌

	return sendBuffer;
}

SendBufferRef ClientPacketHandler::Make_MESSAGE(MESSAGE message)
{
	std::lock_guard<std::mutex> lock(_mutex);

	SendBufferRef sendBuffer = GSendBufferManager->Open(4096); //4kb
	BufferWriter bw(sendBuffer->Buffer(), sendBuffer->AllocSize());
	PacketHeader* header = bw.Reserve<PacketHeader>();

	bw << message;

	header->size = bw.WriteSize();
	header->id = PACKET_MESSAGE;

	sendBuffer->Close(bw.WriteSize()); //사용한 길이만큼 닫아줌

	return sendBuffer;
}

SendBufferRef ClientPacketHandler::Make_BATTLE(float damage, uint32 targerId)
{
	std::lock_guard<std::mutex> lock(_mutex);

	SendBufferRef sendBuffer = GSendBufferManager->Open(4096); //4kb
	BufferWriter bw(sendBuffer->Buffer(), sendBuffer->AllocSize());
	PacketHeader* header = bw.Reserve<PacketHeader>();

	bw << damage << targerId;

	header->size = bw.WriteSize();
	header->id = PACKET_BATTLE;

	sendBuffer->Close(bw.WriteSize()); //사용한 길이만큼 닫아줌

	return sendBuffer;
}

void ClientPacketHandler::GenerateMobList()
{
	ObjectExporter2 exporter;
	exporter.OpenFile(L"../../Resources/Assets/MobDungeon.dat");
	for (int id = 0; id < exporter.enemyListforServer.size(); id++)
	{
		MONSTER_INFO mobInfo;

		mobInfo._instanceId = id;

		wstring name = exporter.enemyListforServer[id].first;
		mobInfo._pos = exporter.enemyListforServer[id].second;
		mobInfo._spawnMapType = MapType::Dungeon;

		auto chinfo = make_shared<CharacterInfo>();
		CHARACTER_INFO chrInfo;
		if (name == L"CoreHound")
		{
			wstring LoadPath = DATA_ADDR_UNIT;
			LoadPath += L"CoreHound/Information.xml";
			chinfo->LoadCharacterInformationFromFile(LoadPath);
			chrInfo = chinfo->GetCharacterInfo();
			mobInfo._monsterType = MonsterType::CoreHound;
			mobInfo._name = L"CoreHound";
		}
		if (name == L"MoltenGiant")
		{
			wstring LoadPath = DATA_ADDR_UNIT;
			LoadPath += L"MoltenGiant/Information.xml";
			chinfo->LoadCharacterInformationFromFile(LoadPath);
			chrInfo = chinfo->GetCharacterInfo();
			mobInfo._monsterType = MonsterType::MoltenGiant;
			mobInfo._name = L"MoltenGiant";
		}
		if (name == L"BaronGeddon")
		{
			wstring LoadPath = DATA_ADDR_UNIT;
			LoadPath += L"BaronGeddon/Information.xml";
			chinfo->LoadCharacterInformationFromFile(LoadPath);
			chrInfo = chinfo->GetCharacterInfo();
			mobInfo._monsterType = MonsterType::BaronGeddon;
			mobInfo._name = L"BaronGeddon";
		}

		mobInfo._hp = chrInfo._hp;
		mobInfo._mp = chrInfo._mp;
		mobInfo._atk = chrInfo._atk;
		mobInfo._def = chrInfo._def;

		AddMobInfoList(id, mobInfo);
	}
}

MONSTER_INFO ClientPacketHandler::GetMobInfo(uint64 uid)
{
	auto it = _mobInfoList.find(uid);
	if (it != _mobInfoList.end())
	{
		return it->second;
	}

	return MONSTER_INFO();
}

wstring ClientPacketHandler::GetStrategyName(uint64 id)
{
	auto it = _strategyName.find(id);
	if (it != _strategyName.end())
	{
		return it->second;
	}
	else
	{
		return L"crashDummy";
	}
}

MONSTER_INFO ClientPacketHandler::CopyChraracterToMobInfo(CHARACTER_INFO chrInfo, MONSTER_INFO mobInfo)
{
	mobInfo._name = chrInfo._name;
	mobInfo._instanceId = chrInfo._instanceId;
	mobInfo._spawnMapType = chrInfo._spawnMapType;
	mobInfo._maxHp = chrInfo._maxHp;
	mobInfo._maxMp = chrInfo._maxMp;
	mobInfo._hp = chrInfo._hp;
	mobInfo._mp = chrInfo._mp;
	mobInfo._atk = chrInfo._atk;
	mobInfo._def = chrInfo._def;
	mobInfo._moveSpeed = chrInfo._moveSpeed;
	mobInfo._aggroLevel = chrInfo._aggroLevel;
	mobInfo._attackRange = chrInfo._attackRange;
	mobInfo._attackTime = chrInfo._attackTime;
	mobInfo._traceRadius = chrInfo._traceRadius;
	mobInfo._pos = chrInfo._pos;
	mobInfo._Rotate = chrInfo._Rotate;
	mobInfo._isAlive = chrInfo._isAlive;
	mobInfo._isAttack = chrInfo._isAttack;
	mobInfo._isBattle = chrInfo._isBattle;
	mobInfo._timeStamp = chrInfo._timeStamp;

	return mobInfo;
}

void ClientPacketHandler::UpdateMobInfo(uint64 uid, MONSTER_INFO mobInfo)
{
	auto it = _mobInfoList.find(uid);
	if (it != _mobInfoList.end())
	{
		it->second = mobInfo;
	}
}