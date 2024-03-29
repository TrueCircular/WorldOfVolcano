#include "pch.h"
#include "BaseScene.h"

//Scene Include
#include "Demo.h"
#include "DungeonScene.h"
//Script Include
#include "CameraMove.h"
#include "LavaFlow.h"
#include "StruectedLavaSprite.h"
#include "LayerSplatter.h"
#include "ObjectExporter.h"
//Engine Include
#include "engine\\FrustomCamera.h"
#include "engine\HeightGetter.h"
#include "engine\Utils.h"
#include "engine/Warrior.h"
#include "engine/SphereCollider.h"
#include "engine/PlayerController.h"
#include "engine/AIController.h"
#include "engine/StrategyFactory.h"
#include "engine\PlayerSoundController.h"
#include "engine\Utils.h"
#include "engine/AbilitySlot.h"
void BaseScene::Init()
{
	//리소스 매니저 초기화
	MANAGER_RESOURCES()->Init();

	//랜더 매니저 초기화
	MANAGER_SOUND()->Init();
	//light
	{
		auto light = make_shared<GameObject>();
		light->AddComponent(make_shared<Light>());
		LightDesc lightDesc;
		lightDesc.ambient = Vec4(0.4f);
		lightDesc.diffuse = Vec4(1.f);
		lightDesc.specular = Vec4(0.1f);
		lightDesc.direction = Vec3(0.5f, -0.5f, 0.f);
		//		lightDesc.direction = Vec3(0, 0.0f, 1.f);
		light->GetLight()->SetLightDesc(lightDesc);
		MANAGER_SCENE()->GetCurrentScene()->Add(light);
//		MANAGER_RENDERER()->PushLightData(lightDesc);
	}

	{
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


	//Character
	_skydome = make_shared<SkyDome>();
	SkyDomeDesc _domeDesc;
	_domeDesc.shaderPath = L"SkyDome.fx";
	_domeDesc.SkyDomeTexPath = RESOURCES_ADDR_TEXTURE;
	_domeDesc.SkyDomeTexPath += L"firelandsskyclouds03.png";
	_domeDesc.SkyDomeBlendPath = RESOURCES_ADDR_TEXTURE;
	_domeDesc.SkyDomeBlendPath += L"firelandsskyhorizon01.png";
	_skydome->Set(&_domeDesc);
	_skydome->Start();

	HeightPlainInfo heightMapDesc;
	heightMapDesc.heightFilename = L"HeightMapBase";
	heightMapDesc.heightFilePath = wstring(RESOURCES_ADDR_TEXTURE) + L"test.bmp";
	heightMapDesc.shaderFilePath = L"ShadowSplattingMapping.fx";
	//heightMapDesc.shaderFilePath = L"TerrainMapping.fx";
	heightMapDesc.shaderFilename = L"HeightMapShaderBase";
	heightMapDesc.textureFilename = L"HeightMapTextureBase";
	heightMapDesc.textureFilePath = wstring(RESOURCES_ADDR_TEXTURE) + L"020.bmp";
	heightMapDesc.meshKey = L"TerrainMeshBase";
	heightMapDesc.distance = 1;
	heightMapDesc.row = 253;
	heightMapDesc.col = 253;

	_terrain = make_shared<Terrain>(heightMapDesc);
	_terrain->Awake();
	_terrain->AddComponent(make_shared<MeshRenderer>());
	_terrain->Start();
	quadTreeTerrain = make_shared<QuadTreeTerrain>();
	quadTreeTerrain->Set(_terrain, 1);
	quadTreeTerrain->Start();


	heightMapDesc.heightFilename = L"HeightMapOutLine";
	heightMapDesc.heightFilePath = wstring(RESOURCES_ADDR_TEXTURE) + L"testOutLine.bmp";
	heightMapDesc.shaderFilePath = L"SplattingMapping.fx";
	//heightMapDesc.shaderFilePath = L"TerrainMapping.fx";
	heightMapDesc.shaderFilename = L"HeightMapShaderDungeon";
	heightMapDesc.textureFilename = L"HeightMapTextureDungeon";
	heightMapDesc.textureFilePath = wstring(RESOURCES_ADDR_TEXTURE) + L"020.bmp";
	heightMapDesc.meshKey = L"TerrainMeshOutLine";
	heightMapDesc.distance = 20;
	heightMapDesc.heightScale = 20;
	heightMapDesc.row = 253;
	heightMapDesc.col = 253;

	_terrainOutLine = make_shared<Terrain>(heightMapDesc);
	_terrainOutLine->Awake();
	_terrainOutLine->AddComponent(make_shared<MeshRenderer>());
	_terrainOutLine->Start();
	quadTreeTerrainOutLine = make_shared<QuadTreeTerrain>();
	quadTreeTerrainOutLine->Set(_terrainOutLine, 3);
	quadTreeTerrainOutLine->Start();

	SplatterDesc spDesc{};
	spDesc.texPath[0] = wstring(RESOURCES_ADDR_TEXTURE) + L"burningsteppsash01.png";
	spDesc.texPath[1] = wstring(RESOURCES_ADDR_TEXTURE) + L"burningsteppsashcracks.png";
	spDesc.texPath[2] = wstring(RESOURCES_ADDR_TEXTURE) + L"burningsteppscharcoal01.png";
	spDesc.texName[0] = L"Splat1Base";
	spDesc.texName[1] = L"Splat2Base";
	spDesc.texName[2] = L"Splat3Base";
	spDesc.alphaPath = wstring(RESOURCES_ADDR_TEXTURE) + L"testalpha.bmp";
	spDesc.alphaName = L"SplatAlphaBase";
	splatter = make_shared<LayerSplatter>();
	splatter->Set(spDesc, MANAGER_RESOURCES()->GetResource<Shader>(L"HeightMapShaderBase"));
	quadTreeTerrain->AddSplatter(splatter);
	SetTerrain(_terrain);

	quadTreeTerrain->AddSplatter(splatter);
	spDesc.alphaPath = wstring(RESOURCES_ADDR_TEXTURE) + L"testalphaOutLine.bmp";
	spDesc.alphaName = L"SplatAlphaBaseOutLine";
	splatterOutLine = make_shared<LayerSplatter>();
	splatterOutLine->Set(spDesc, MANAGER_RESOURCES()->GetResource<Shader>(L"HeightMapShaderDungeon"));
	quadTreeTerrainOutLine->AddSplatter(splatterOutLine);

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
		_warrior = make_shared<Warrior>();
		_warrior->Awake();
		_warrior->SetCharacterController(make_shared<PlayerController>());
		_warrior->GetComponent<AbilitySlot>()->SetController(_warrior->GetComponent<PlayerController>());
		_warrior->SetSpwanPosition(spawnPos);
		_warrior->GetTransform()->SetLocalRotation(Vec3(0, ::XMConvertToRadians(105.f), 0));
		_warrior->Start();

		Add(_warrior);
		AddShadow(_warrior);

		_camera->GetCamera()->SetTargetTransform(_warrior->GetTransform());
	}
	//Npc
	{
		_MagniBronzebeard = make_shared<MagniBronzebeard>();
		_MagniBronzebeard->Awake();
		shared_ptr<AIController> _aiCon = make_shared<AIController>();
		_aiCon->SetAIType(AIType::PlayableUnit);
		_MagniBronzebeard->AddComponent(_aiCon);
		_MagniBronzebeard->AddComponent(make_shared<CharacterInfo>());
		_MagniBronzebeard->GetTransform()->SetLocalScale(Vec3(0.2f, 0.2f, 0.2f));
		_MagniBronzebeard->GetTransform()->SetLocalPosition(Vec3(10, 25, 0));
		_MagniBronzebeard->Start();

		Add(_MagniBronzebeard);
		//AddShadow(_MagniBronzebeard);
	}


	shared_ptr<Sounds> bgm = MANAGER_RESOURCES()->GetResource<Sounds>(L"Lobby");
	if (bgm == nullptr) {
	shared_ptr<Sounds> bgm = make_shared<Sounds>();
	wstring bgmpath = RESOURCES_ADDR_SOUND;
	bgmpath += L"Scene/Lobby.mp3";
	bgm->Load(bgmpath);
	MANAGER_RESOURCES()->AddResource<Sounds>(L"Lobby", bgm);
	//auto chs = bgm->Play(true);
	//}
	//else {
	//	bgm->Play(true);
	}
	auto _playerAbSlot = _warrior->GetComponent<AbilitySlot>();
	MANAGER_IMGUI()->SetAbilitySlot(_playerAbSlot);
	SpawnManager::GetInstance().Init();
}
void BaseScene::Start()
{
	Scene::Start();
}

void BaseScene::Update()
{
	MANAGER_SOUND()->Update();
	MANAGER_SHADOW()->StartShadow();
	_terrain->GetMeshRenderer()->SetPass(1);
	_terrain->GetMeshRenderer()->ShadowUpdate();
	_terrain->GetMeshRenderer()->SetPass(0);

	Scene::ShadowUpdate();
	MANAGER_SHADOW()->EndShadow();

	{
		sendInfo = ClientPacketHandler::Instance().GetUserInfo();
		sendInfo._pos = _warrior->GetTransform()->GetPosition();
		sendInfo._Rotate = _warrior->GetTransform()->GetLocalRotation();
		sendInfo._jumpFlag = *_warrior->GetComponent<PlayerController>()->GetJumpState();
		sendInfo._animState = *_warrior->GetComponent<PlayerController>()->GetCurrentUnitState();
		sendInfo._spawnMapType = SpawnManager::GetInstance().GetSpawnMapType();

		//SendBuffer
		_sendBuffer = ClientPacketHandler::Instance().Make_USER_INFO(sendInfo, sendInfo._name);
	}

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
	}
	latestMessageSize = MANAGER_IMGUI()->GetLatestMessages().size();


	Scene::Update();

	quadTreeTerrain->Frame((*_frustom->frustomBox.get()));
	quadTreeTerrainOutLine->Frame((*_frustom->frustomBox.get()));
	//skyBox->Update();
	MANAGER_INDICATOR()->Frame();

	shared_ptr<Scene> scene = make_shared<DungeonScene>();
	scene->SetSceneName(L"DungeonScene");

	//Change
	{
		int size = MANAGER_IMGUI()->GetChangeSceneQueueSize();
		if (size > 0)
		{
			wstring name = MANAGER_SCENE()->GetCurrentScene()->GetSceneName();
			SpawnManager::GetInstance().Reset(name);
			SpawnManager::GetInstance().EraseSpawnerMap(name);
			MANAGER_SCENE()->ChangeScene(scene);
		}
	}
}

void BaseScene::LateUpdate()
{

	_skydome->Update();
	Scene::LateUpdate();
	quadTreeTerrain->Update();
	quadTreeTerrainOutLine->Update();
	MANAGER_SOUND()->Update();
	MANAGER_INDICATOR()->Render();
	MANATER_PARTICLE()->Render();
}
