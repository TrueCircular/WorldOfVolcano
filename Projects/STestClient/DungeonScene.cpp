#include "pch.h"
#include "DungeonScene.h"
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

#include "engine\PlayerSoundController.h"


void DungeonScene::Init()
{
	//리소스 매니저 초기화
	MANAGER_RESOURCES()->Init();

	//사운드 매니저 초기화
	MANAGER_SOUND()->Init();

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

	DamageIndicator::GetInstance().Init();
	DamageIndicator::GetInstance().SetCamera(_childCamera);

	ObjectExporter exporter;
	exporter.OpenFile(L"../../Resources/Assets/dungeon1fix.dat");
	for (int i = 0; i < exporter._structureList.size(); ++i) {
		Add(exporter._structureList[i]);
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

	HeightPlainInfo heightMapDesc;
	heightMapDesc.heightFilename = L"HeightMapDungeon";
	heightMapDesc.heightFilePath = wstring(RESOURCES_ADDR_TEXTURE) + L"dungeon1.bmp";
	heightMapDesc.shaderFilePath = L"SplattingMapping.fx";
	//heightMapDesc.shaderFilePath = L"TerrainMapping.fx";
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
	quadTreeTerrain = make_shared<QuadTreeTerrain>();
	quadTreeTerrain->Set(_terrain, 3);
	quadTreeTerrain->Start();

	SplatterDesc spDesc{};
	spDesc.texPath[0] = wstring(RESOURCES_ADDR_TEXTURE) + L"burningsteppsash01.png";
	spDesc.texPath[1] = wstring(RESOURCES_ADDR_TEXTURE) + L"burningsteppsashcracks.png";
	spDesc.texPath[2] = wstring(RESOURCES_ADDR_TEXTURE) + L"burningsteppscharcoal01.png";
	spDesc.texName[0] = L"Splat1Dungeon";
	spDesc.texName[1] = L"Splat2Dungeon";
	spDesc.texName[2] = L"Splat3Dungeon";
	spDesc.alphaPath = wstring(RESOURCES_ADDR_TEXTURE) + L"dungeon1alpha.bmp";
	spDesc.alphaName = L"SplatAlphaDungeon";
	splatter = make_shared<LayerSplatter>();
	splatter->Set(spDesc, MANAGER_RESOURCES()->GetResource<Shader>(L"HeightMapShaderDungeon"));

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

		shared_ptr< PlayerSoundController> soundController = make_shared<PlayerSoundController>();

		soundController->Set(_warrior->GetTransform());
		shared_ptr<Sounds> bgm2 = MANAGER_RESOURCES()->GetResource<Sounds>(L"Warrior_Attack");
		if (bgm2 == nullptr) {
			bgm2 = make_shared<Sounds>();
			wstring bgmpath = RESOURCES_ADDR_SOUND;
			bgmpath += L"Character/Playable/Warrior/Warrior_Attack1.mp3";
			bgm2->Load(bgmpath);
			MANAGER_RESOURCES()->AddResource<Sounds>(L"Warrior_Attack", bgm2);
		}
		soundController->SetSound(PlayerAnimType::Attack1, bgm2);

		bgm2 = MANAGER_RESOURCES()->GetResource<Sounds>(L"Warrior_Attack2");
		if (bgm2 == nullptr) {
			bgm2 = make_shared<Sounds>();
			wstring bgmpath = RESOURCES_ADDR_SOUND;
			bgmpath += L"Character/Playable/Warrior/Warrior_Attack2.mp3";
			bgm2->Load(bgmpath);
			MANAGER_RESOURCES()->AddResource<Sounds>(L"Warrior_Attack2", bgm2);
		}
		soundController->SetSound(PlayerAnimType::Attack2, bgm2);

		bgm2 = MANAGER_RESOURCES()->GetResource<Sounds>(L"Warrior_Damaged");
		if (bgm2 == nullptr) {
			bgm2 = make_shared<Sounds>();
			wstring bgmpath = RESOURCES_ADDR_SOUND;
			bgmpath += L"Character/Playable/Warrior/Warrior_Damaged.mp3";
			bgm2->Load(bgmpath);
			MANAGER_RESOURCES()->AddResource<Sounds>(L"Warrior_Damaged", bgm2);
		}
		soundController->SetSound(PlayerAnimType::Damaged, bgm2);

		bgm2 = MANAGER_RESOURCES()->GetResource<Sounds>(L"Warrior_Death");
		if (bgm2 == nullptr) {
			bgm2 = make_shared<Sounds>();
			wstring bgmpath = RESOURCES_ADDR_SOUND;
			bgmpath += L"Character/Playable/Warrior/Warrior_Death.mp3";
			bgm2->Load(bgmpath);
			MANAGER_RESOURCES()->AddResource<Sounds>(L"Warrior_Death", bgm2);
		}
		soundController->SetSound(PlayerAnimType::Death, bgm2);

		_warrior->GetComponent<PlayerController>()->SetSoundController(soundController);

		MANAGER_SOUND()->SetTransForm(_warrior->GetTransform());
	}


	shared_ptr<Sounds> bgm = MANAGER_RESOURCES()->GetResource<Sounds>(L"fireland");
	if (bgm == nullptr) {
		shared_ptr<Sounds> bgm = make_shared<Sounds>();
		wstring bgmpath = RESOURCES_ADDR_SOUND;
		bgmpath += L"Scene/Dungeon.mp3";
		bgm->Load(bgmpath);
		MANAGER_RESOURCES()->AddResource<Sounds>(L"fireland", bgm);
		bgm->Play(true);
	}
	else {
		bgm->Play(true);
	}
///	bool isplaynsd;
//	chs->isPlaying(&isplaynsd);

	SpawnManager::GetInstance().Init();
}
void DungeonScene::Start()
{
	Scene::Start();
}

void DungeonScene::Update()
{
	quadTreeTerrain->Frame((*frustom->frustomBox.get()));
	MANAGER_SOUND()->Update();

	{
		sendInfo = ClientPacketHandler::Instance().GetUserInfo();
		sendInfo._pos = _warrior->GetTransform()->GetPosition();
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

	//DamageIndiCatorBox box;
	//box.damage = 444;
	//box.pos = _warrior->GetTransform()->GetLocalPosition();
	//box.textDuration = 20; sexkinghfghfghrttrrtherethrteh
	//DamageIndicator::GetInstance().Add(box);

	Scene::Update();
	skyBox->Update();
	DamageIndicator::GetInstance().Frame();

	shared_ptr<Scene> scene = make_shared<BaseScene>();
	scene->SetSceneName(L"BaseScene");

	/*if ()
	{
		wstring name = MANAGER_SCENE()->GetCurrentScene()->GetSceneName();
		SpawnManager::GetInstance().Reset(name);
		SpawnManager::GetInstance().EraseSpawnerMap(name);
		MANAGER_SCENE()->ChangeScene(scene);
	}*/
}

void DungeonScene::LateUpdate()
{
	Scene::LateUpdate();
	quadTreeTerrain->Update();

	DamageIndicator::GetInstance().Render();
}
