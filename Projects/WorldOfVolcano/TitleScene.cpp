#include "pch.h"
#include "TitleScene.h"
#include "BaseScene.h"
#include "DungeonScene.h"

void TitleScene::Init()
{
}

void TitleScene::Start()
{
}

void TitleScene::Update()
{
	if (MANAGER_IMGUI()->CheckGameStart())
	{
		shared_ptr<Scene> scene = make_shared<BaseScene>();
		scene->SetSceneName(L"BaseScene");

		wstring name = MANAGER_SCENE()->GetCurrentScene()->GetSceneName();
		SpawnManager::GetInstance().Reset(name);
		SpawnManager::GetInstance().EraseSpawnerMap(name);
		MANAGER_SCENE()->ChangeScene(scene);
	}
}

void TitleScene::LateUpdate()
{
}