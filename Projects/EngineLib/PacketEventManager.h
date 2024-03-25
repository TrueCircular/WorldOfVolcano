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
private:
	std::queue<PacketEvent> eventQueue;
};
