#pragma once

class PacketEventManager
{
private:
	PacketEventManager() {};
	static PacketEventManager* _instance;
public:
	~PacketEventManager();
	static PacketEventManager* GetInstance()
	{
		if (_instance == nullptr)
			_instance = new PacketEventManager();

		return _instance;
	}
public:
	void AddEvent(PacketEvent event);
	PacketEvent PopEvent();
	bool GetIsMapHost() { return _isMapHost; }
	void SetIsMapHost(bool isHost) { _isMapHost = isHost; }
private:
	std::queue<PacketEvent> eventQueue;
	bool _isMapHost = false;
};
