#include "pch.h"
#include "Demo.h"


#include "TitleScene.h"
#include "BaseScene.h"
#include "DungeonScene.h"
#include "BossScene.h"

ClientServiceRef _service;

void Demo::Init() {
#pragma region Client Thread
	_service = MakeShared<ClientService>(
		NetAddress(L"192.168.0.11", 7777),
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

	MANAGER_RESOURCES()->Init();

	MANAGER_SOUND()->Init();

	MANATER_PARTICLE()->Init();
	wstring maintitle = RESOURCES_ADDR_TEXTURE;
	maintitle += L"MainTitle2.png";
	wstring victory = RESOURCES_ADDR_TEXTURE;
	victory += L"victory.jpg";
	wstring logo = RESOURCES_ADDR_TEXTURE;
	logo += L"logo.png";
	wstring endlogo = RESOURCES_ADDR_TEXTURE;
	endlogo += L"endlogo.png";
	wstring chSkill = RESOURCES_ADDR_TEXTURE;
	chSkill += L"Skill/skill_charge.jpg";
	wstring shSkill = RESOURCES_ADDR_TEXTURE;
	shSkill += L"Skill/skill_shouting.jpg";
	wstring healPotion = RESOURCES_ADDR_TEXTURE;
	healPotion += L"Item/Consumable/HealthPotion.png";
	wstring manaPotion = RESOURCES_ADDR_TEXTURE;
	manaPotion += L"Item/Consumable/ManaPotion.png";

	shared_ptr<Texture> tex = MANAGER_RESOURCES()->LoadResource<Texture>(L"mainTitle", maintitle);
	tex = MANAGER_RESOURCES()->LoadResource<Texture>(L"victory", victory);
	tex = MANAGER_RESOURCES()->LoadResource<Texture>(L"logo", logo);
	tex = MANAGER_RESOURCES()->LoadResource<Texture>(L"endlogo", endlogo);
	tex = MANAGER_RESOURCES()->LoadResource<Texture>(L"skill_charge", chSkill);
	tex = MANAGER_RESOURCES()->LoadResource<Texture>(L"skill_shouting", shSkill);
	tex = MANAGER_RESOURCES()->LoadResource<Texture>(L"HealthPotion", healPotion);
	tex = MANAGER_RESOURCES()->LoadResource<Texture>(L"ManaPotion", manaPotion);

#pragma endregion Client Thread
	//shared_ptr<Scene> scene = make_shared<TitleScene>();
	//MANAGER_SCENE()->ChangeScene(scene);
	//
	shared_ptr<Scene> scene = make_shared<BaseScene>();
	scene->SetSceneName(L"BaseScene");
	// 
	//shared_ptr<Scene> scene = make_shared<DungeonScene>();
	//scene->SetSceneName(L"DungeonScene");
	//
	//shared_ptr<Scene> scene = make_shared<TestAbilityScene>();
	//scene->SetSceneName(L"TestAbility");
	//shared_ptr<Scene> scene = make_shared<BossScene>();
	//scene->SetSceneName(L"BossScene");

	MANAGER_SCENE()->ChangeScene(scene);
}

void Demo::Update()
{

}

void Demo::Render()
{
}
