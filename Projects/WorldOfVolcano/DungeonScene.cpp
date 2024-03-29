#include "pch.h"
#include "DungeonScene.h"

//Scene Include
#include "Demo.h"
#include "BossScene.h"
//Script Include
#include "LavaFlow.h"
#include "StruectedLavaSprite.h"
#include "LayerSplatter.h"
#include "ObjectExporter.h"
#include "CameraMove.h"
//Engine Include
#include "engine\\FrustomCamera.h"
#include "engine\HeightGetter.h"
#include "engine\Utils.h"
#include "engine/Warrior.h"
#include "engine/CoreHound.h"
#include "engine/BaronGeddon.h"
#include "engine/SphereCollider.h"
#include "engine/PlayerController.h"
#include "engine/AIController.h"
#include "engine/StrategyFactory.h"
#include "engine\PlayerSoundController.h"
#include "engine/AbilitySlot.h"
#include "engine/UnitFSM.h"
#include "engine/UnitStrategy.h"
#include "ObjectExporter2.h"
void DungeonScene::Init()
{
	//light
	{
		auto light = make_shared<GameObject>();
		light->AddComponent(make_shared<Light>());
		LightDesc lightDesc;
		lightDesc.ambient = Vec4(0.4f);
		lightDesc.diffuse = Vec4(1.f);
		lightDesc.specular = Vec4(0.1f);
		lightDesc.direction = Vec3(0.0f,-1.0f, 0.0f);
		light->GetLight()->SetLightDesc(lightDesc);
		MANAGER_SCENE()->GetCurrentScene()->Add(light);
	}

	//Terrain Object Load
	{
		ObjectExporter exporter;
		exporter.OpenFile(L"../../Resources/Assets/dungeon1fix.dat");
		for (int i = 0; i < exporter._structureList.size(); ++i) {
			Add(exporter._structureList[i]);
		}

		auto obj = make_shared<GameObject>();
		LavaSpriteDesc descs{};
		descs.duration = 10;
		descs.ShaderName = L"Lavashader";
		descs.ShaderPath = L"lava.fx";
		for (int i = 1; i < 31; i++) {
			descs.spritePathList.push_back(wstring(RESOURCES_ADDR_TEXTURE) + L"lava/lava." + to_wstring(i) + L".png");
		}
		obj->GetOrAddTransform()->SetLocalPosition(Vec3(0, 0.5f, 0));
		obj->AddComponent(make_shared<MeshRenderer>());
		shared_ptr<StruectedLavaSprite> sprite = make_shared<StruectedLavaSprite>(descs);
		shared_ptr<Mesh> mesh = make_shared<Mesh>();
		mesh->CreateGridWithDistance(3, 3, 4096);
		MANAGER_RESOURCES()->AddResource(L"lava", mesh);
		obj->GetMeshRenderer()->SetMesh(mesh);
		obj->GetMeshRenderer()->SetMaterial(MANAGER_RESOURCES()->GetResource<Material>(L"Veigar"));
		obj->GetMeshRenderer()->SetPass(0);
		obj->AddComponent(sprite);
		obj->AddComponent(make_shared<Lava>(100, 3, false));
		MANAGER_SCENE()->GetCurrentScene()->Add(obj);
	}

	{
		SkyBoxDesc descs{};
		descs.resourceFilePath[SkyBoxDesc::SKY_Front] = wstring(RESOURCES_ADDR_TEXTURE) + L"burningsteppsrock01.png";
		descs.resourceFilePath[SkyBoxDesc::SKY_Back] = wstring(RESOURCES_ADDR_TEXTURE) + L"burningsteppsrock01.png";
		descs.resourceFilePath[SkyBoxDesc::SKY_Right] = wstring(RESOURCES_ADDR_TEXTURE) + L"burningsteppsrock01.png";
		descs.resourceFilePath[SkyBoxDesc::SKY_Left] = wstring(RESOURCES_ADDR_TEXTURE) + L"burningsteppsrock01.png";
		descs.resourceFilePath[SkyBoxDesc::SKY_UP] = wstring(RESOURCES_ADDR_TEXTURE) + L"burningsteppsrock01.png";
		descs.resourceFilePath[SkyBoxDesc::SKY_DOWN] = wstring(RESOURCES_ADDR_TEXTURE) + L"burningsteppsrock01.png";
		descs.shaderPath = L"skyBox.fx";
		skyBox = make_shared < Skybox>();
		skyBox->Set(&descs);
		skyBox->Start();
	}

	//Terrain
	{
		HeightPlainInfo heightMapDesc;
		heightMapDesc.heightFilename = L"HeightMapDungeon";
		heightMapDesc.heightFilePath = wstring(RESOURCES_ADDR_TEXTURE) + L"dungeon1.bmp";
		heightMapDesc.shaderFilePath = L"SplattingMapping.fx";
		heightMapDesc.shaderFilename = L"HeightMapShaderDungeon";
		heightMapDesc.textureFilename = L"HeightMapTextureDungeon";
		heightMapDesc.textureFilePath = wstring(RESOURCES_ADDR_TEXTURE) + L"020.bmp";
		heightMapDesc.meshKey = L"TerrainMeshDungeon";
		heightMapDesc.distance = 2;
		heightMapDesc.row = 253;
		heightMapDesc.col = 253;

		_terrain = make_shared<Terrain>(heightMapDesc);
		_terrain->Awake();
		_terrain->AddComponent(make_shared<MeshRenderer>());
		_terrain->Start();
		_quadTreeTerrain = make_shared<QuadTreeTerrain>();
		_quadTreeTerrain->Set(_terrain, 3);
		_quadTreeTerrain->Start();

		SplatterDesc spDesc{};
		spDesc.texPath[0] = wstring(RESOURCES_ADDR_TEXTURE) + L"burningsteppsash01.png";
		spDesc.texPath[1] = wstring(RESOURCES_ADDR_TEXTURE) + L"burningsteppsashcracks.png";
		spDesc.texPath[2] = wstring(RESOURCES_ADDR_TEXTURE) + L"burningsteppscharcoal01.png";
		spDesc.texName[0] = L"Splat1Dungeon";
		spDesc.texName[1] = L"Splat2Dungeon";
		spDesc.texName[2] = L"Splat3Dungeon";
		spDesc.alphaPath = wstring(RESOURCES_ADDR_TEXTURE) + L"dungeon1alpha.bmp";
		spDesc.alphaName = L"SplatAlphaDungeon";
		_splatter = make_shared<LayerSplatter>();
		_splatter->Set(spDesc, MANAGER_RESOURCES()->GetResource<Shader>(L"HeightMapShaderDungeon"));

		_quadTreeTerrain->AddSplatter(_splatter);

		SetTerrain(_terrain);
	}

	//Camera
	{
		_frustom = make_shared<FrustomCamera>();
		_camera = make_shared<GameObject>();
		_camera->Awake();
		_camera->AddComponent(make_shared<Camera>());
		_camera->AddComponent(_frustom);
		_camera->Start();
		_camera->SetName(L"Camera");
		_camera->GetCamera()->Init(Vec3(0, 100.f, -100.f), CameraType::Target, ProjectionType::Perspective, 130.f);
		_camera->GetCamera()->SetCameraToTargetOffset(Vec3(0, 10, 0));
		MANAGER_SCENE()->GetCurrentScene()->Add(_camera);
	}
	//Character
	{
		//Player
		{
			auto height = make_shared<HeightGetter>();
			height->Set(MANAGER_SCENE()->GetCurrentScene()->GetCurrentTerrain().get());
			//Vec3 spwanPos = Vec3(-44.f, 0.f, 277.f);
			//spwanPos.y = height->GetHeight(spwanPos);

			_warrior = make_shared<Warrior>();
			_warrior->Awake();
			_warrior->SetCharacterController(make_shared<PlayerController>());
			_warrior->GetComponent<AbilitySlot>()->SetController(_warrior->GetComponent<PlayerController>());
			_warrior->SetSpwanPosition(spawnPos);
			_warrior->GetTransform()->SetLocalRotation(Vec3(0, ::XMConvertToRadians(105.f), 0));
			_warrior->Start();

			Add(_warrior);

			_camera->GetCamera()->SetTargetTransform(_warrior->GetTransform());
		}

		MANAGER_INDICATOR()->SetCamera(_camera->GetCamera());

	}

	//Background Sound
	{
		shared_ptr<Sounds> bgm = MANAGER_RESOURCES()->GetResource<Sounds>(L"DungeonBGM");
		if (bgm == nullptr) {
			shared_ptr<Sounds> bgm = make_shared<Sounds>();
			wstring bgmpath = RESOURCES_ADDR_SOUND;
			bgmpath += L"Scene/Dungeon.mp3";
			bgm->Load(bgmpath);
			bgm->SetVolume(0.35f);
			MANAGER_RESOURCES()->AddResource<Sounds>(L"DungeonBGM", bgm);
			bgm->Play(true);
		}
		else {
			bgm->Play(true);
		}
	}

	auto _playerAbSlot = _warrior->GetComponent<AbilitySlot>();
	MANAGER_IMGUI()->SetAbilitySlot(_playerAbSlot);
	SpawnManager::GetInstance().Init();
}
void DungeonScene::Start()
{
	Scene::Start();
}

void DungeonScene::Update()
{
	{
		sendInfo = ClientPacketHandler::Instance().GetUserInfo();

		CHARACTER_INFO chrInfo = _warrior->GetComponent<CharacterInfo>()->GetCharacterInfo();
		sendInfo._hp = chrInfo._hp;
		sendInfo._mp = chrInfo._mp;
		sendInfo._atk = chrInfo._atk;
		sendInfo._def = chrInfo._def;

		sendInfo._pos = _warrior->GetTransform()->GetPosition();
		sendInfo._Rotate = _warrior->GetTransform()->GetLocalRotation();
		sendInfo._jumpFlag = *_warrior->GetComponent<PlayerController>()->GetJumpState();
		sendInfo._animState = *_warrior->GetComponent<PlayerController>()->GetCurrentUnitState();
		sendInfo._spawnMapType = SpawnManager::GetInstance().GetSpawnMapType();

		//SendBuffer
		_sendBuffer = ClientPacketHandler::Instance().Make_USER_INFO(sendInfo, sendInfo._name);
	}

	//Dead
	if (sendInfo._hp <= 0)
	{
		MANAGER_IMGUI()->NotifyPlayerAlive(false);
	}
	//Rebirth
	if (MANAGER_IMGUI()->GetRebirthQueueSize() > 0)
	{
		_warrior->GetComponent<PlayerController>()->Respawn();
	}

	//Event
	{
		PacketEvent packetEvent = MANAGER_EVENT()->PopEvent();
		SendBufferRef eventBuffer;

		switch (packetEvent.type)
		{
		case PacketEventType::DamageRequest:
			eventBuffer = ClientPacketHandler::Instance().Make_BATTLE(packetEvent.damage, packetEvent.targetId);
			_service->Broadcast(eventBuffer);
			break;
		default:
			break;
		}
	}

	SendBufferRef mobBuffer;
	
	SpawnManager::GetInstance().Update();

	
#pragma region Client Thread
	//12분의1초 = 83.33ms
	//30분의1초 = 33.33ms
	//60분의1초 = 16.67ms

	if (_threadTimer < 0.1f)
	{
		_threadTimer += MANAGER_TIME()->GetDeltaTime();
	}
	else
	{
		if (ClientPacketHandler::Instance().GetIsMapHost() == true)
		{
			for (auto pair : SpawnManager::GetInstance().GetCurrentMobList())
			{
				MONSTER_INFO mobInfo = ClientPacketHandler::Instance().GetMobInfo(pair.first);
				CHARACTER_INFO chrInfo = pair.second->GetComponent<CharacterInfo>()->GetCharacterInfo();
				mobInfo = ClientPacketHandler::Instance().CopyChraracterToMobInfo(chrInfo, mobInfo);
				ClientPacketHandler::Instance().UpdateMobInfo(pair.first, mobInfo);
				wstring mobName = pair.second->GetComponent<AIController>()->GetUnitFsm()->GetStrategyName();
				mobBuffer = ClientPacketHandler::Instance().Make_MONSTER_INFO(mobInfo, mobName);
				_service->Broadcast(mobBuffer);
			}
		}

		if (_sendBuffer != nullptr)
		{
			_service->Broadcast(_sendBuffer);
		}
		_threadTimer = 0.0f;
	}
#pragma endregion Client Thread

	//채팅
	if (MANAGER_IMGUI()->GetLatestMessages().size() > 0 && (MANAGER_IMGUI()->GetLatestMessages().size() != latestMessageSize))
	{
		string text = MANAGER_IMGUI()->GetLatestMessages().back();
		const char* newMessage = text.c_str();
		MESSAGE message;
		std::strncpy(message._messageBox, newMessage, sizeof(message._messageBox) - 1);
		message._messageBox[sizeof(message._messageBox) - 1] = '\0'; // Null 문자 추가
		_sendBuffer = ClientPacketHandler::Instance().Make_MESSAGE(message);
		_service->Broadcast(_sendBuffer);
		MANAGER_IMGUI()->GetLatestMessages().clear();
	}
	latestMessageSize = MANAGER_IMGUI()->GetLatestMessages().size();

	//DamageIndiCatorBox box;
	//box.damage = 444;
	//box.pos = _warrior->GetTransform()->GetLocalPosition();
	//box.textDuration = 20; sexkinghfghfghrttrrtherethrteh
	//MANAGER_INDICATOR()->Add(box);

	Scene::Update();
	skyBox->Update();

	_quadTreeTerrain->Frame((*_frustom->frustomBox.get()));
	{
		Vec3 pos =  _warrior->GetTransform()->GetPosition();

		wstring Pstring = L"Player Pos X:";
		Pstring += ::to_wstring(pos.x);
		Pstring += L" Y:";
		Pstring += ::to_wstring(pos.y);
		Pstring += L" Z:";
		Pstring += ::to_wstring(pos.z);
		Pstring += L"\n";
		
		OutputDebugString(Pstring.c_str());
	}

	MANATER_PARTICLE()->Update();
	MANAGER_SOUND()->Update();
	MANAGER_INDICATOR()->Frame();

	shared_ptr<Scene> scene = make_shared<BossScene>();
	scene->SetSceneName(L"BossScene");

	if (MANAGER_INPUT()->GetButton(KEY_TYPE::Q))
	{
		wstring name = MANAGER_SCENE()->GetCurrentScene()->GetSceneName();
		SpawnManager::GetInstance().Reset(name);
		SpawnManager::GetInstance().EraseSpawnerMap(name);
		MANAGER_SCENE()->ChangeScene(scene);
	}

	if (MANAGER_INPUT()->GetButton(KEY_TYPE::E))
	{
		_warrior->GetComponent<PlayerController>()->Respawn();
	}
}

void DungeonScene::LateUpdate()
{
	Scene::LateUpdate();
	_quadTreeTerrain->Update();
	MANATER_PARTICLE()->Render();
	MANAGER_INDICATOR()->Render();
}
