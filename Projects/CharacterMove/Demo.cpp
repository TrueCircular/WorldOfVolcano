#include "pch.h"
#include "Demo.h"

#include "DungeonScene.h"
#include "BaseScene.h"
#include "MainScene.h"

ClientServiceRef _service;

void Demo::Init(){
#pragma region Client Thread
	_service = MakeShared<ClientService>(
		NetAddress(L"192.168.0.115", 7777),
		MakeShared<IocpCore>(),
		MakeShared<ServerSession>,
		1);

	ASSERT_CRASH(_service->Start());

	for (int32 i = 0; i < 3; i++)
	{
		GThreadManager->Launch([=]()
			{
				while (true)
				{
					_service->GetIocpCore()->Dispatch();
				}
			}
		);
	}
#pragma endregion Client Thread

	//shared_ptr<BaseScene> scene = make_shared<BaseScene>();
	//scene->SetSceneName(L"BaseScene");
//	shared_ptr<Scene> scene = make_shared<DungeonScene>();
//	scene->SetSceneName(L"DungeonScene");
	shared_ptr<Texture> tex = MANAGER_RESOURCES()->LoadResource<Texture>(L"mainTitle", L"../../Resources/Texture/MainTitle2.png");
	tex = MANAGER_RESOURCES()->LoadResource<Texture>(L"logo", L"../../Resources/Texture/logo.png");
	tex = MANAGER_RESOURCES()->LoadResource<Texture>(L"skill_charge", L"../../Resources/Texture/Skill/skill_charge.jpg");
	tex = MANAGER_RESOURCES()->LoadResource<Texture>(L"skill_shouting", L"../../Resources/Texture/Skill/skill_shouting.jpg");
	tex = MANAGER_RESOURCES()->LoadResource<Texture>(L"HealthPotion", L"../../Resources/Texture/Item/Consumable/HealthPotion.png");
	tex = MANAGER_RESOURCES()->LoadResource<Texture>(L"ManaPotion", L"../../Resources/Texture/Item/Consumable/ManaPotion.png");

	shared_ptr<Scene> scene = make_shared<TestScene>();
	scene->SetSceneName(L"TyestScene");
	//shared_ptr<Scene> scene = make_shared<MainScene>();
	MANAGER_SCENE()->ChangeScene(scene);
}

void Demo::Update()
{

}

void Demo::Render()
{
}
