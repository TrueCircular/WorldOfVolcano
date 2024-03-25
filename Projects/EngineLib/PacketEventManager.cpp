#include "pch.h"
#include "PacketEventManager.h"

PacketEventManager* PacketEventManager::_instance = nullptr;

void PacketEventManager::AddEvent(PacketEvent event)
{
    eventQueue.push(event);
}

PacketEvent PacketEventManager::PopEvent()
{
    if (eventQueue.empty() == false)
    {
        PacketEvent event = eventQueue.front();
        eventQueue.pop();

        return event;
    }
    
    return PacketEvent{};
}
