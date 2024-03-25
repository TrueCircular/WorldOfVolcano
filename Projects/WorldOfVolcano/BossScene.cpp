#include "pch.h"
#include "BossScene.h"

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
#include "engine/BaronGeddon.h"
#include "engine/SphereCollider.h"
#include "ObjectExporter.h"
#include "Demo.h"

#include "engine\PlayerSoundController.h"


void BossScene::Init()
{
	//light
	{
		auto light = make_shared<GameObject>();
		light->AddComponent(make_shared<Light>());
		LightDesc lightDesc;
		lightDesc.ambient = Vec4(0.4f);
		lightDesc.diffuse = Vec4(1.f);
		lightDesc.specular = Vec4(0.1f);
		lightDesc.direction = Vec3(0.0f, -1.0f, 0.0f);
		light->GetLight()->SetLightDesc(lightDesc);
		MANAGER_SCENE()->GetCurrentScene()->Add(light);
	}

	DamageIndicator::GetInstance().Init();

	ObjectExporter exporter;
	exporter.OpenFile(L"../../Resources/Assets/dungeonboss.dat");
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

	HeightPlainInfo heightMapDesc;
	heightMapDesc.heightFilename = L"HeightMapBossDungeon";
	heightMapDesc.heightFilePath = wstring(RESOURCES_ADDR_TEXTURE) + L"BossDungeon.bmp";
	heightMapDesc.shaderFilePath = L"SplattingMapping.fx";
	//heightMapDesc.shaderFilePath = L"TerrainMapping.fx";
	heightMapDesc.shaderFilename = L"HeightMapShaderDungeon";
	heightMapDesc.textureFilename = L"HeightMapTextureBossDungeon";
	heightMapDesc.textureFilePath = wstring(RESOURCES_ADDR_TEXTURE) + L"Bosstile1.png";
	heightMapDesc.meshKey = L"TerrainMeshBossDungeon";
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
	spDesc.texPath[0] = wstring(RESOURCES_ADDR_TEXTURE) + L"Bosstile2.png";
	spDesc.texPath[1] = wstring(RESOURCES_ADDR_TEXTURE) + L"Bosstile3.png";
	spDesc.texPath[2] = wstring(RESOURCES_ADDR_TEXTURE) + L"Bosstile4.png";
	spDesc.texName[0] = L"Splat1BossDungeon";
	spDesc.texName[1] = L"Splat2BossDungeon";
	spDesc.texName[2] = L"Splat3BossDungeon";
	spDesc.alphaPath = wstring(RESOURCES_ADDR_TEXTURE) + L"BossDungeonAlpha.bmp";
	spDesc.alphaName = L"SplatAlphaBossDungeon";
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
		_childCamera->GetCamera()->Init(Vec3(0, 100.f, -100.f), CameraType::Target, ProjectionType::Perspective, 125.f);
		_childCamera->GetCamera()->SetCameraToTargetOffset(Vec3(0, 10, 0));
		MANAGER_SCENE()->GetCurrentScene()->Add(_childCamera);
	}
	//Character
	{
		_warrior = make_shared<Warrior>();
		_warrior->Awake();
		_childCamera->GetCamera()->SetTargetTransform(_warrior->GetTransform());
		_warrior->SetCharacterController(make_shared<PlayerController>());
		_warrior->SetSpwanPosition(Vec3(-44.f, 0.f, 277.f));
		_warrior->GetTransform()->SetLocalPosition(Vec3(-44.f, 0.f, 277.f));
		_warrior->GetTransform()->SetLocalRotation(Vec3(0, ::XMConvertToRadians(105.f), 0));
		_warrior->Start();

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

		//_warrior->GetComponent<PlayerController>()->SetSoundController(soundController);

		MANAGER_SOUND()->SetTransForm(_warrior->GetTransform());
	}

	//monster
	//{
	//	auto height = make_shared<HeightGetter>();
	//	height->Set(MANAGER_SCENE()->GetCurrentScene()->GetCurrentTerrain().get());
	//	Vec3 spwanPos = Vec3(103, 0, 240);
	//	spwanPos.y = height->GetHeight(spwanPos);


	//	auto baronGeddon = make_shared<BaronGeddon>();
	//	baronGeddon->Awake();
	//	baronGeddon->SetCharacterController(make_shared<AIController>(), AIType::EnemyUnit);
	//	baronGeddon->SetSpwanPosition(spwanPos);
	//	baronGeddon->Start();
	//	baronGeddon->GetTransform()->SetPosition(spwanPos);

	//	Add(baronGeddon);
	//}


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
void BossScene::Start()
{
	Scene::Start();
}

void BossScene::Update()
{
	quadTreeTerrain->Frame((*frustom->frustomBox.get()));
	MANAGER_SOUND()->Update();

	sendInfo = ClientPacketHandler::Instance().GetUserInfo();
	sendInfo._pos = _warrior->GetTransform()->GetPosition();
	sendInfo._Rotate = _warrior->GetTransform()->GetLocalRotation();
	sendInfo._jumpFlag = *_warrior->GetComponent<PlayerController>()->GetJumpState();
	sendInfo._animState = *_warrior->GetComponent<PlayerController>()->GetCurrentUnitState();
	sendInfo._spawnMapType = SpawnManager::GetInstance().GetSpawnMapType();

	_sendBuffer = ClientPacketHandler::Instance().Make_USER_INFO(sendInfo, sendInfo._name);
	SpawnManager::GetInstance().Update();

#pragma region Client Thread
	//12����1�� = 83.33ms
	//30����1�� = 33.33ms
	//60����1�� = 16.67ms

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

	//ä��
	if (MANAGER_IMGUI()->GetLatestMessages().size() > 0 && (MANAGER_IMGUI()->GetLatestMessages().size() != latestMessageSize))
	{
		string text = MANAGER_IMGUI()->GetLatestMessages().back();
		const char* newMessage = text.c_str();
		MESSAGE message;
		std::strncpy(message._messageBox, newMessage, sizeof(message._messageBox) - 1);
		message._messageBox[sizeof(message._messageBox) - 1] = '\0'; // Null ���� �߰�
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
	DamageIndicator::GetInstance().Frame();

	{
		Vec3 pos = _warrior->GetTransform()->GetPosition();

		wstring Pstring = L"Player Pos X:";
		Pstring += ::to_wstring(pos.x);
		Pstring += L" Y:";
		Pstring += ::to_wstring(pos.y);
		Pstring += L" Z:";
		Pstring += ::to_wstring(pos.z);
		Pstring += L"\n";

		OutputDebugString(Pstring.c_str());
	}

	shared_ptr<Scene> scene = make_shared<BaseScene>();
	scene->SetSceneName(L"BaseScene");

	if (MANAGER_INPUT()->GetButton(KEY_TYPE::Q))
	{
		wstring name = MANAGER_SCENE()->GetCurrentScene()->GetSceneName();
		SpawnManager::GetInstance().Reset(name);
		SpawnManager::GetInstance().EraseSpawnerMap(name);
		MANAGER_SCENE()->ChangeScene(scene);
	}
}

void BossScene::LateUpdate()
{
	Scene::LateUpdate();
	quadTreeTerrain->Update();

	DamageIndicator::GetInstance().Render();
}
