#include "pch.h"
#include "SpawnManager.h"

void SpawnManager::Init()
{
    wstring name = MANAGER_SCENE()->GetCurrentScene()->GetSceneName();
    SpawnManager::GetInstance().AddSceneSpawner(name);
}

void SpawnManager::Update()
{
    for (const auto& pair : _spawnerMap) {
        const std::wstring& key = pair.first;
        const std::shared_ptr<Spawner>& spawner = pair.second;

        spawner->Update();
    }
}

void SpawnManager::AddSceneSpawner(wstring name)
{
    shared_ptr<Spawner> spawner = std::make_shared<Spawner>();
    spawner->Init();
    _spawnerMap.insert(std::make_pair(name, spawner));
}

void SpawnManager::Reset(wstring name)
{
    /*const auto it = _spawnerMap.find(name);

    if (it != _spawnerMap.end())
    {
        it->second->Reset();
    }*/

    for (auto& pair : _spawnerMap) {
        pair.second->Reset();
    }
}

MapType SpawnManager::GetSpawnMapType()
{
    wstring name = MANAGER_SCENE()->GetCurrentScene()->GetSceneName();

    if (name == L"BaseScene")
    {
        return MapType::Lobby;
    }

    if (name == L"DungeonScene")
    {
        return MapType::Dungeon;
    }

    if (name == L"BossScene")
    {
        return MapType::BossRoom;
    }
}

map<uint64, shared_ptr<GameObject>> SpawnManager::GetCurrentMobList()
{
    wstring name = MANAGER_SCENE()->GetCurrentScene()->GetSceneName();
    auto it = _spawnerMap.find(name);
    if (it != _spawnerMap.end())
    {
        return it->second->GetMobList();
    }

    return map<uint64, shared_ptr<GameObject>>();
}
