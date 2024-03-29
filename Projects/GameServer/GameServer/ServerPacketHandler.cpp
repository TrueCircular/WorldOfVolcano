#include "pch.h"
#include "ServerPacketHandler.h"
#include "BufferReader.h"
#include "BufferWriter.h"
#include "GameSession.h"
#include "GameSessionManager.h"

std::map<uint64_t, std::wstring> ServerPacketHandler::_strategyName;

void ServerPacketHandler::HandlePacket(BYTE* buffer, int32 len)
{
	BufferReader br(buffer, len);

	PacketHeader header;
	br.Peek(&header);

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
		Handle_BATTLE(buffer, len);
		break;
	default:
		break;
	}
}

void ServerPacketHandler::Handle_USER_INFO(BYTE* buffer, int32 len)
{
	BufferReader br(buffer, len);

	PacketHeader header;
	br >> header;

	PACKET_Player_INFO userInfo;
	br >> userInfo;
	userInfo._timeStamp = TIMER().getCurrentTime();

	wstring name;
	uint16 nameLen;
	br >> nameLen;
	name.resize(nameLen);
	br.Read((void*)name.data(), nameLen * sizeof(WCHAR));

	SendBufferRef sendBuffer = ServerPacketHandler::Make_USER_INFO(userInfo, name, true, false);
	GSessionManager.UpdateUserInfo(userInfo);
	GSessionManager.Broadcast(sendBuffer);
}

void ServerPacketHandler::Handle_MONSTER_INFO(BYTE* buffer, int32 len)
{
	BufferReader br(buffer, len);

	PacketHeader header;
	br >> header;

	PACKET_Mob_INFO info;
	br >> info;

	wstring name;
	uint16 nameLen;
	br >> nameLen;
	name.resize(nameLen);
	br.Read((void*)name.data(), nameLen * sizeof(WCHAR));

	auto it = _strategyName.find(info._instanceId);
	if (it != _strategyName.end())
	{
		it->second = name;
	}
	else
	{
		_strategyName.insert(make_pair(info._instanceId, name));
	}

	GSessionManager.UpdateMobInfo(info);
}

void ServerPacketHandler::Handle_MESSAGE(BYTE* buffer, int32 len)
{
	BufferReader br(buffer, len);

	PacketHeader header;
	br >> header;

	MESSAGE message;
	br >> message;

	SendBufferRef sendBuffer = ServerPacketHandler::Make_MESSAGE(message);
	GSessionManager.Broadcast(sendBuffer);
}

void ServerPacketHandler::Handle_BATTLE(BYTE* buffer, int32 len)
{
	BufferReader br(buffer, len);

	PacketHeader header;
	br >> header;

	float damage;
	uint32 tgtId;
	br >> damage >> tgtId;

	GSessionManager.MonsterBattleCalculate(damage, tgtId);
}

SendBufferRef ServerPacketHandler::Make_USER_CONNECT()
{
	//TODO: 플레이어 정보, 초기정보
	return nullptr;
}

SendBufferRef ServerPacketHandler::Make_USER_INFO(PACKET_Player_INFO userInfo, wstring name, bool otherPacket, bool isMapHost)
{
	SendBufferRef sendBuffer = GSendBufferManager->Open(4096); //4kb
	BufferWriter bw(sendBuffer->Buffer(), sendBuffer->AllocSize());
	PacketHeader* header = bw.Reserve<PacketHeader>();

	bw << userInfo;

	bw << (uint16)name.size();
	bw.Write((void*)name.data(), name.size() * sizeof(WCHAR));

	header->size = bw.WriteSize();
	header->id = PACKET_USER_INFO;
	header->other = otherPacket;
	header->isMapHost = isMapHost;

	sendBuffer->Close(bw.WriteSize()); //사용한 길이만큼 닫아줌

	return sendBuffer;
}

SendBufferRef ServerPacketHandler::Make_MONSTER_INFO(map<uint32, PACKET_Mob_INFO> charaInfo)
{
	SendBufferRef sendBuffer = GSendBufferManager->Open(4096); //4kb
	BufferWriter bw(sendBuffer->Buffer(), sendBuffer->AllocSize());
	PacketHeader* header = bw.Reserve<PacketHeader>();

	for (const auto& pair : charaInfo) {
		uint64 id = pair.first;
		PACKET_Mob_INFO info = pair.second;
		info._timeStamp = TIMER().getCurrentTime();
		
		bw << info;

		auto it = _strategyName.find(pair.second._instanceId);
		if (it != _strategyName.end())
		{
			bw << (uint16)it->second.size();
			bw.Write((void*)it->second.data(), it->second.size() * sizeof(WCHAR));
		}
	}

	header->size = bw.WriteSize();
	header->id = PACKET_MONSTER_INFO;

	sendBuffer->Close(bw.WriteSize()); //사용한 길이만큼 닫아줌

	return sendBuffer;
}

SendBufferRef ServerPacketHandler::Make_MESSAGE(MESSAGE message)
{
	SendBufferRef sendBuffer = GSendBufferManager->Open(4096); //4kb
	BufferWriter bw(sendBuffer->Buffer(), sendBuffer->AllocSize());
	PacketHeader* header = bw.Reserve<PacketHeader>();

	bw << message;

	header->size = bw.WriteSize();
	header->id = PACKET_MESSAGE;

	sendBuffer->Close(bw.WriteSize()); //사용한 길이만큼 닫아줌

	return sendBuffer;
}

SendBufferRef ServerPacketHandler::Make_HOST(bool isMapHost)
{
	SendBufferRef sendBuffer = GSendBufferManager->Open(4096); //4kb
	BufferWriter bw(sendBuffer->Buffer(), sendBuffer->AllocSize());
	PacketHeader* header = bw.Reserve<PacketHeader>();

	header->size = bw.WriteSize();
	header->id = PACKET_HOST;
	header->isMapHost = isMapHost;

	sendBuffer->Close(bw.WriteSize()); //사용한 길이만큼 닫아줌

	return sendBuffer;
}

SendBufferRef ServerPacketHandler::Make_USER_DISCONNECT(uint64 uid)
{
	SendBufferRef sendBuffer = GSendBufferManager->Open(4096); //4kb
	BufferWriter bw(sendBuffer->Buffer(), sendBuffer->AllocSize());
	PacketHeader* header = bw.Reserve<PacketHeader>();

	bw << uid;

	header->size = bw.WriteSize();
	header->id = PACKET_DISCONNECT;
	header->other = true;

	sendBuffer->Close(bw.WriteSize()); //사용한 길이만큼 닫아줌
	return sendBuffer;
}