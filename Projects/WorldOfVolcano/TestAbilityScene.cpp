#include "pch.h"
#include "TestAbilityScene.h"

//Scene Include


//Script Include
#include "LavaFlow.h"
#include "StruectedLavaSprite.h"
#include "LayerSplatter.h"
#include "ObjectExporter.h"

//Engine Include
#include "engine/FrustomCamera.h"
#include "engine/Unit.h"
#include "engine/Warrior.h"
#include "engine/BaronGeddon.h"
#include "engine/SphereCollider.h"
#include "engine/HeightGetter.h"
#include "engine/PlayerController.h"
#include "engine/AIController.h"
#include "engine/Utils.h"
#include "engine/StrategyFactory.h"

void TestAbilityScene::Init()
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
			_warrior = make_shared<Warrior>();
			_warrior->Awake();
			_warrior->SetCharacterController(make_shared<PlayerController>());
			_warrior->SetSpwanPosition(Vec3(-44.f, 0.f, 277.f));
			_warrior->GetTransform()->SetLocalRotation(Vec3(0, ::XMConvertToRadians(105.f), 0));
			_warrior->Start();

			Add(_warrior);

			_camera->GetCamera()->SetTargetTransform(_warrior->GetTransform());
		}

		//Enemy
		{
			//baronGeddon
			{
				auto height = make_shared<HeightGetter>();
				height->Set(MANAGER_SCENE()->GetCurrentScene()->GetCurrentTerrain().get());
				Vec3 spwanPos = Vec3(103, 0, 240);
				spwanPos.y = height->GetHeight(spwanPos);

				auto baronGeddon = make_shared<BaronGeddon>();
				baronGeddon->Awake();
				baronGeddon->SetCharacterController(make_shared<AIController>(), AIType::EnemyUnit);
				baronGeddon->GetComponent<AIController>()->SetFsmStrategyList(StrategyFactory::GetStrategyList<BaronGeddon>());
				baronGeddon->SetSpwanPosition(spwanPos);
				baronGeddon->Start();

				Add(baronGeddon);
			}
		}
	}

	//Background Sound
	{
		shared_ptr<Sounds> bgm = MANAGER_RESOURCES()->GetResource<Sounds>(L"DungeonBGM");
		if (bgm == nullptr) {
			shared_ptr<Sounds> bgm = make_shared<Sounds>();
			wstring bgmpath = RESOURCES_ADDR_SOUND;
			bgmpath += L"Scene/Dungeon.mp3";
			bgm->Load(bgmpath);
			bgm->SetVolume(0.5f);
			MANAGER_RESOURCES()->AddResource<Sounds>(L"DungeonBGM", bgm);
			bgm->Play(true);
		}
		else {
			bgm->Play(true);
		}
	}

}

void TestAbilityScene::Start()
{
	Scene::Start();
}

void TestAbilityScene::FixedUpdate()
{
	Scene::FixedUpdate();
}

void TestAbilityScene::Update()
{
	_quadTreeTerrain->Frame((*_frustom->frustomBox.get()));

	Scene::Update();

	MANAGER_SOUND()->Update();
}

void TestAbilityScene::LateUpdate()
{
	Scene::LateUpdate();

	_quadTreeTerrain->Update();
}
