#include "pch.h"
#include "TitleScene.h"
#include "BaseScene.h"

void TitleScene::Init()
{
	//리소스 매니저 초기화
	MANAGER_RESOURCES()->Init();

	shared_ptr<Texture> tex = MANAGER_RESOURCES()->LoadResource<Texture>(L"mainTitle", L"../../Resources/Texture/MainTitle2.png");
	tex = MANAGER_RESOURCES()->LoadResource<Texture>(L"skill_charge", L"../../Resources/Texture/Skill/skill_charge.jpg");
	tex = MANAGER_RESOURCES()->LoadResource<Texture>(L"skill_shouting", L"../../Resources/Texture/Skill/skill_shouting.jpg");
	tex = MANAGER_RESOURCES()->LoadResource<Texture>(L"HealthPotion", L"../../Resources/Texture/Item/Consumable/HealthPotion.png");
	tex = MANAGER_RESOURCES()->LoadResource<Texture>(L"ManaPotion", L"../../Resources/Texture/Item/Consumable/ManaPotion.png");
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
