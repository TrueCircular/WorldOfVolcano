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

#include "engine/WarriorRoar.h"
#include "engine/Clap.h"
#include "engine/Smoke1.h"
#include "engine/Smoke2.h"
#include "engine/Smoke3.h"
#include "engine/FireStorm.h"
#include "engine/FireBall.h"
#include "engine/Polar.h"
#include "engine/MagicCircle.h"
#include "engine/LineSpark.h"
#include "engine/ShineHelper.h"
#include "engine/Exlode.h"
void BaseScene::Init()
{
	//리소스 매니저 초기화
	MANAGER_RESOURCES()->Init();
	{
		_shader = make_shared<Shader>(L"Instancing.fx");
		MANAGER_RESOURCES()->AddResource(L"Default", _shader);
		wstring dTex = RESOURCES_ADDR_TEXTURE;
		dTex += L"Effect/noise.png";
		_dissolve = MANAGER_RESOURCES()->GetOrAddTexture(L"Dissolve", dTex);
	}

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
		fog_obj->GetTransform()->SetLocalScale(Vec3(5, 5, 5));
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

	//SplatterDesc spDesc{};
	//spDesc.texPath[0] = wstring(RESOURCES_ADDR_TEXTURE) + L"burningsteppsash01.png";
	//spDesc.texPath[1] = wstring(RESOURCES_ADDR_TEXTURE) + L"burningsteppsashcracks.png";
	//spDesc.texPath[2] = wstring(RESOURCES_ADDR_TEXTURE) + L"burningsteppscharcoal01.png";
	//spDesc.texName[0] = L"Splat1Dungeon";
	//spDesc.texName[1] = L"Splat2Dungeon";
	//spDesc.texName[2] = L"Splat3Dungeon";
	//spDesc.alphaPath = wstring(RESOURCES_ADDR_TEXTURE) + L"dungeon1alpha.bmp";
	//spDesc.alphaName = L"SplatAlphaDungeon";
	//splatter = make_shared<LayerSplatter>();
	//splatter->Set(spDesc, MANAGER_RESOURCES()->GetResource<Shader>(L"HeightMapShaderDungeon"));

	quadTreeTerrain->AddSplatter(splatter);
	spDesc.alphaPath = wstring(RESOURCES_ADDR_TEXTURE) + L"testalphaOutLine.bmp";
	spDesc.alphaName = L"SplatAlphaBaseOutLine";
	splatterOutLine = make_shared<LayerSplatter>();
	splatterOutLine->Set(spDesc, MANAGER_RESOURCES()->GetResource<Shader>(L"HeightMapShaderDungeon"));
	quadTreeTerrainOutLine->AddSplatter(splatterOutLine);

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


	shared_ptr<Sounds> bgm = MANAGER_RESOURCES()->GetResource<Sounds>(L"Lobby");
	if (bgm == nullptr) {
		shared_ptr<Sounds> bgm = make_shared<Sounds>();
		wstring bgmpath = RESOURCES_ADDR_SOUND;
		bgmpath += L"Scene/Lobby.mp3";
		bgm->SetVolume(0.5);
		bgm->Load(bgmpath);
		MANAGER_RESOURCES()->AddResource<Sounds>(L"Lobby", bgm);
		auto chs = bgm->Play(true);
		}
		else {
			bgm->Play(true);
	}
	MANATER_PARTICLE()->ClearList();
	shared_ptr<WarriorRoar> roar = make_shared<WarriorRoar>();
	auto _waranimator = _warrior->GetChildByName(L"Model")->GetModelAnimator();
	roar->SetAnimator(_waranimator);
	shared_ptr<Clap> clap = make_shared<Clap>();
	shared_ptr<Smoke1> smoke1 = make_shared<Smoke1>();
	shared_ptr<Smoke2> smoke2 = make_shared<Smoke2>();
	shared_ptr<Smoke3> smoke3 = make_shared<Smoke3>();
	shared_ptr<FireStorm> storm = make_shared<FireStorm>();
	shared_ptr<FireBall> fireBall = make_shared<FireBall>();
	shared_ptr<Polar> polar = make_shared<Polar>();
	shared_ptr<MagicCircle> magicCircle = make_shared<MagicCircle>();
	shared_ptr<LineSpark> lineSpark = make_shared<LineSpark>();
	shared_ptr<Exlode> explode = make_shared<Exlode>();
	tempTargetTrans = make_shared<Transform>();
	tempTargetTrans->SetLocalPosition(Vec3(0, 80, 0));
	shared_ptr<ShineHelper> sparkHelper = make_shared<ShineHelper>();
	MANATER_PARTICLE()->AddManagingParticle(L"Explode", explode);
	MANATER_PARTICLE()->AddManagingParticle(L"SparkHelper", sparkHelper);
	MANATER_PARTICLE()->AddManagingParticle(L"WarriorRoar", roar);
	MANATER_PARTICLE()->AddManagingParticle(L"Clap", clap);
	MANATER_PARTICLE()->AddManagingParticle(L"Smoke1", smoke1);
	MANATER_PARTICLE()->AddManagingParticle(L"Smoke3", smoke3);
	MANATER_PARTICLE()->AddManagingParticle(L"Smoke2", smoke2);
	MANATER_PARTICLE()->AddManagingParticle(L"FireStorm", storm);
	MANATER_PARTICLE()->AddManagingParticle(L"FireBall", fireBall);
	MANATER_PARTICLE()->AddManagingParticle(L"Polar", polar);
	MANATER_PARTICLE()->AddManagingParticle(L"MagicCircle", magicCircle);
	MANATER_PARTICLE()->AddManagingParticle(L"LineSpark", lineSpark);
	//{
	//shared_ptr<Transform> pos = make_shared<Transform>();
	//pos->SetParent(_warrior->GetTransform());
	//pos->SetLocalPosition(Vec3(-1, 8,3));
	//pos->SetLocalRotation(Vec3(::XMConvertToRadians(90), ::XMConvertToRadians(90),::XMConvertToRadians(-90)));
	//pos->SetLocalScale(Vec3(1.5, 1.5, 1.5));
	//shared_ptr<ParticleInstance> instancedata = make_shared<ParticleInstance>(5, pos, nullptr, 0, true);
	//polar->AddParticle(instancedata);
	//} 
	//{
	//	shared_ptr<Transform> pos2 = make_shared<Transform>();
	//	pos2->SetParent(_warrior->GetTransform());
	//	pos2->SetLocalPosition(Vec3(0,0.2,0));
	//	pos2->SetLocalScale(Vec3(20,20,20));
	//	shared_ptr<ParticleInstance> instancedata = make_shared<ParticleInstance>(5, pos2, nullptr, 0, true);
	//	magicCircle->AddParticle(instancedata);
	//}
	SpawnManager::GetInstance().Init();
}
void BaseScene::Start()
{
	Scene::Start();
}

void BaseScene::Update()
{
	quadTreeTerrain->Frame((*frustom->frustomBox.get()));
	quadTreeTerrainOutLine->Frame((*frustom->frustomBox.get()));
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
		//sendInfo._spawnMapType = SpawnManager::GetInstance().GetSpawnMapId();

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
				_sendBuffer = ClientPacketHandler::Instance().Make_BATTLE(sendInfo, targetId);
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
	if (MANAGER_INPUT()->GetButtonDown(KEY_TYPE::E)) {
		auto roarParticle = MANATER_PARTICLE()->GetParticleFromName(L"WarriorRoar");
		shared_ptr<Transform> pos = make_shared<Transform>();
		pos->SetParent(_warrior->GetChildByName(L"Model")->GetTransform());
		shared_ptr<ParticleInstance>  instancedata = make_shared<ParticleInstance>(1.2,pos, nullptr, 0);
		auto _tweenDesc = _warrior->GetChildByName(L"Model")->GetModelAnimator()->GetTweenDesc();
		roarParticle->AddParticle(instancedata,_tweenDesc);
		auto clapParticle = MANATER_PARTICLE()->GetParticleFromName(L"Clap");
		shared_ptr<Transform> pos2 = make_shared<Transform>();
		pos2->SetLocalPosition(_warrior->GetTransform()->GetLocalPosition());
		pos2->SetScale(Vec3(100,100,100));
		shared_ptr<ParticleInstance>  instancedata2 = make_shared<ParticleInstance>(1.6, pos2, nullptr, 0);
		clapParticle->AddParticle(instancedata2);
		auto helper = MANATER_PARTICLE()->GetParticleFromName(L"SparkHelper");
		helper->AddParticle(instancedata2);
		//for (int i = 0; i < 5; ++i) {
		//	auto SparkParticle = MANATER_PARTICLE()->GetParticleFromName(L"LineSpark");
		//	shared_ptr<Transform> pos3 = make_shared<Transform>();
		//	Vec3 refpos = _warrior->GetTransform()->GetLocalPosition();
		//	float x = Utils::Randstep(-30, 30);
		//	float y = Utils::Randstep(0, 30);
		//	float z = Utils::Randstep(-30, 30);
		//	pos3->SetScale(Vec3(10, 10, 10));
		//	pos3->SetLocalPosition(Vec3(refpos.x+x, refpos.y+y, refpos.z+z));
		//	shared_ptr<ParticleInstance>  instancedata2 = make_shared<ParticleInstance>(1.6, pos3, nullptr, 0);
		//	SparkParticle->AddParticle(instancedata2);
		//}

	}	
	if (MANAGER_INPUT()->GetButtonDown(KEY_TYPE::C)) {
		//auto clapParticle = MANATER_PARTICLE()->GetParticleFromName(L"Smoke1");
		//shared_ptr<Transform> pos2 = make_shared<Transform>();
		//pos2->SetLocalPosition(_warrior->GetTransform()->GetLocalPosition());
		//pos2->SetScale(Vec3(100, 100, 100));
		//shared_ptr<ParticleInstance>  instancedata2 = make_shared<ParticleInstance>(3, pos2, nullptr, 0);
		//clapParticle->AddParticle(instancedata2);
		auto clapParticle = MANATER_PARTICLE()->GetParticleFromName(L"Explode");
		shared_ptr<Transform> pos2 = make_shared<Transform>();
		pos2->SetLocalPosition(_warrior->GetTransform()->GetLocalPosition());
		pos2->SetScale(Vec3(100, 100, 100));
		shared_ptr<ParticleInstance>  instancedata2 = make_shared<ParticleInstance>(3, pos2, nullptr, 0);
		clapParticle->AddParticle(instancedata2);
	}
	if (MANAGER_INPUT()->GetButtonDown(KEY_TYPE::A)) {
		auto clapParticle = MANATER_PARTICLE()->GetParticleFromName(L"Smoke2");
		shared_ptr<Transform> pos2 = make_shared<Transform>();
		pos2->SetLocalPosition(_warrior->GetTransform()->GetLocalPosition());
		pos2->SetScale(Vec3(100, 100, 100));
		shared_ptr<ParticleInstance>  instancedata2 = make_shared<ParticleInstance>(3, pos2, nullptr, 0);
		clapParticle->AddParticle(instancedata2);

	}
	if (MANAGER_INPUT()->GetButtonDown(KEY_TYPE::D)) {
		auto clapParticle = MANATER_PARTICLE()->GetParticleFromName(L"Smoke3");
		shared_ptr<Transform> pos2 = make_shared<Transform>();
		pos2->SetLocalPosition(_warrior->GetTransform()->GetLocalPosition());
		pos2->SetScale(Vec3(100, 100, 100));
		shared_ptr<ParticleInstance>  instancedata2 = make_shared<ParticleInstance>(3, pos2, nullptr, 0);
		clapParticle->AddParticle(instancedata2);

	}
	if (MANAGER_INPUT()->GetButtonDown(KEY_TYPE::W)) {
		auto clapParticle = MANATER_PARTICLE()->GetParticleFromName(L"FireStorm");
		shared_ptr<Transform> pos2 = make_shared<Transform>();
		pos2->SetLocalPosition(_warrior->GetTransform()->GetLocalPosition());
		pos2->SetScale(Vec3(1, 1, 1));
		shared_ptr<ParticleInstance>  instancedata2 = make_shared<ParticleInstance>(3, pos2, nullptr, 0);
		clapParticle->AddParticle(instancedata2);

	}	
	if (MANAGER_INPUT()->GetButtonDown(KEY_TYPE::Z)) {
		auto clapParticle = MANATER_PARTICLE()->GetParticleFromName(L"FireBall");
		shared_ptr<Transform> pos2 = make_shared<Transform>();
		pos2->SetLocalPosition(_warrior->GetTransform()->GetLocalPosition());
		pos2->SetLocalRotation(_warrior->GetTransform()->GetLocalRotation());
		pos2->SetScale(Vec3(1, 1, 1));
		shared_ptr<ParticleInstance>  instancedata2 = make_shared<ParticleInstance>(3, pos2, tempTargetTrans, 100,true);
		instancedata2->parentTransform = _warrior->GetTransform();
		clapParticle->AddParticle(instancedata2);

	}

	MANATER_PARTICLE()->Update();
	if (MANAGER_INPUT()->GetButtonDown(KEY_TYPE::Q))
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
	quadTreeTerrainOutLine->Update();

	MANAGER_SOUND()->Update();
	DamageIndicator::GetInstance().Render();
	MANATER_PARTICLE()->Render();
}
