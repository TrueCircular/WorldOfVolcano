#include "pch.h"
#include "BaseScene.h"
#include "CameraMove.h"
#include "LavaFlow.h"
#include "StruectedLavaSprite.h"
#include "LayerSplatter.h"
#include "engine\Utils.h"
#include "CameraMove.h"
#include "engine\HeightGetter.h"
#include "engine\Utils.h"
#include "engine/Warrior.h"
#include "engine/CoreHound.h"
#include "engine/SphereCollider.h"
#include "ObjectExporter.h"
#include "Demo.h"

#include "DungeonScene.h"
#include "BaseScene.h"
#include "MainScene.h"

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

	DamageIndicator::GetInstance().Init();
	DamageIndicator::GetInstance().SetCamera(_childCamera);

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

	auto fog_obj = make_shared<GameObject>();
	//Character
	{
		shared_ptr<Model> model = make_shared<Model>();
		{
			wstring MeshAdr = RESOURCES_ADDR_MESH_STATIC;
			MeshAdr += L"fog";
			MeshAdr += L"/";
			MeshAdr += L"fog";
			MeshAdr += L".mesh";

			wstring MaterialAdr = RESOURCES_ADDR_TEXTURE_STATIC;
			MaterialAdr += L"fog";
			MaterialAdr += L"/";
			MaterialAdr += L"fog";
			MaterialAdr += L".xml";

			model->ReadModel(MeshAdr);
			model->ReadMaterial(MaterialAdr);
		}
		const auto& shader = MANAGER_RESOURCES()->GetResource<Shader>(L"Default");
		shared_ptr<ModelRenderer> tempModelRenderer = make_shared<ModelRenderer>(shader);
		{
			tempModelRenderer->SetModel(model);
			tempModelRenderer->SetPass(8);
		}
		fog_obj->AddComponent(tempModelRenderer);
		fog_obj->GetOrAddTransform()->SetLocalPosition(Vec3(0, 0, 0));
	}
	fog_obj->Awake();
	Add(fog_obj);


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
	quadTreeTerrain->Set(_terrain, 3);
	quadTreeTerrain->Start();

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

	//Camera
	{

		frustom = make_shared<FrustomCamera>();
		_childCamera = make_shared<GameObject>();
		_childCamera->Awake();
		_childCamera->AddComponent(make_shared<Camera>());
		_childCamera->AddComponent(frustom);
		_childCamera->Start();
		_childCamera->SetName(L"Camera");
		_childCamera->GetCamera()->Init(Vec3(0, 100.f, -100.f), CameraType::Target, ProjectionType::Perspective, 100.f);
		_childCamera->GetCamera()->SetCameraToTargetOffset(Vec3(0, 10, 0));
		MANAGER_SCENE()->GetCurrentScene()->Add(_childCamera);
	}
	//Character
	{
		_warrior = make_shared<Warrior>();
		_warrior->Awake();
		_childCamera->GetCamera()->SetTargetTransform(_warrior->GetTransform());
		_warrior->AddComponent(make_shared<PlayerController>());
		_warrior->Start();
		_warrior->GetTransform()->SetLocalPosition(spawnPos);
		Add(_warrior);
		AddShadow(_warrior);

		MANAGER_SOUND()->SetTransForm(_warrior->GetTransform());
	}
	//Npc
	{
		_MagniBronzebeard = make_shared<MagniBronzebeard>();
		_MagniBronzebeard->Awake();
		shared_ptr<AIController> _aiCon = make_shared<AIController>();
		_aiCon->SetAIType(AIType::PlayableUnit);
		_MagniBronzebeard->AddComponent(_aiCon);
		_MagniBronzebeard->AddComponent(make_shared<CharacterInfo>());
		_MagniBronzebeard->Start();
		_MagniBronzebeard->GetTransform()->SetLocalScale(Vec3(0.2f, 0.2f, 0.2f));
		_MagniBronzebeard->GetTransform()->SetLocalPosition(spawnPos);
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

	SpawnManager::GetInstance().Init();
}
void BaseScene::Start()
{
	Scene::Start();
}

void BaseScene::Update()
{
	quadTreeTerrain->Frame((*frustom->frustomBox.get()));
	MANAGER_SOUND()->Update();
	MANAGER_SHADOW()->StartShadow();
	_terrain->GetMeshRenderer()->SetPass(1);
	_terrain->GetMeshRenderer()->ShadowUpdate();
	_terrain->GetMeshRenderer()->SetPass(0);

	Scene::ShadowUpdate();
	MANAGER_SHADOW()->EndShadow();

	{
		sendInfo._uid = ClientPacketHandler::Instance().GetUserInfo()._uid;
		sendInfo._pos = _warrior->GetTransform()->GetPosition();
		sendInfo._isOnline = true;
		sendInfo._Rotate = _warrior->GetTransform()->GetLocalRotation();
		sendInfo._jumpFlag = *_warrior->GetComponent<PlayerController>()->GetJumpState();
		sendInfo._isAttack = _warrior->GetComponent<PlayerController>()->IsAttack();
		sendInfo._isBattle = _warrior->GetComponent<PlayerController>()->IsBattle();
		sendInfo._animState = *_warrior->GetComponent<PlayerController>()->GetCurrentUnitState();
		sendInfo._spawnMapType = SpawnManager::GetInstance().GetSpawnMapType();

		//Alive
		if (sendInfo._isAlive == false)
		{
			_warrior->GetComponent<PlayerController>()->NotifyPlayerAlive(false);
			MANAGER_IMGUI()->NotifyPlayerAlive(false);
		}

		//Rebirth
		{
			int size = MANAGER_IMGUI()->GetAttackQueueSize();
			if (size > 0)
			{
				sendInfo._isAlive = true;
				sendInfo._hp = sendInfo._maxHp;
				sendInfo._pos = spawnPos;
				_warrior->GetTransform()->SetLocalPosition(spawnPos);
				_warrior->GetComponent<PlayerController>()->NotifyPlayerAlive(true);
				MANAGER_IMGUI()->NotifyPlayerAlive(true);
			}
		}

		//Attack1
		{
			int size = _warrior->GetComponent<PlayerController>()->GetAttackQueueSize();
			if (size > 0)
			{
				uint32 targetId = _warrior->GetComponent<PlayerController>()->GetPickedInfo()._instanceId;
				SkillType skillType = _warrior->GetComponent<PlayerController>()->GetFrontAttackQueue();
				_sendBuffer = ClientPacketHandler::Instance().Make_BATTLE(sendInfo, targetId, skillType);
				_service->Broadcast(_sendBuffer);
			}
		}

		//SendBuffer
		_sendBuffer = ClientPacketHandler::Instance().Make_USER_INFO(sendInfo);
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

	//skyBox->Update();
	DamageIndicator::GetInstance().Frame();

	shared_ptr<Scene> scene = make_shared<DungeonScene>();
	scene->SetSceneName(L"DungeonScene");

	if (MANAGER_INPUT()->GetButton(KEY_TYPE::Q))
	{
		wstring name = MANAGER_SCENE()->GetCurrentScene()->GetSceneName();
		SpawnManager::GetInstance().Reset(name);
		SpawnManager::GetInstance().EraseSpawnerMap(name);
		MANAGER_SCENE()->ChangeScene(scene);
	}
}

void BaseScene::LateUpdate()
{

	Scene::LateUpdate();
	quadTreeTerrain->Update();

	DamageIndicator::GetInstance().Render();
}
