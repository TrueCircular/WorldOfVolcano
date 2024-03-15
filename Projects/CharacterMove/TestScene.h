#pragma once
#include "QuadTreeBlock.h"
#include "Skybox.h"
//Character
#include "Character.h"
#include "Player.h"
#include "ClientPacketHandler.h"
#include "SpawnManager.h"
//Server
#include "ServerSession.h"
#include "ThreadManager.h"
#include "Service.h"

class TestScene : public Scene
{

	shared_ptr<GameObject> _obj;
	shared_ptr<Shader> effShader;
	shared_ptr<Texture> effTex;
	shared_ptr<Transform> world;
	shared_ptr<FrustomCamera> frustom;

	shared_ptr<Shader> effShader2;
	shared_ptr<Texture> pillarTex1;
	shared_ptr<Texture> pillarTex2;
	shared_ptr<Transform> world2;

	shared_ptr<Shader> effShader3;
	shared_ptr<Texture> circleTex1;
	shared_ptr<Texture> circleTex2;
	shared_ptr<Transform> world3;

	float maxtime = 15.0f;
	float currenttime=0;
	shared_ptr<Terrain> _terrain;
	shared_ptr<QuadTreeTerrain> quadTreeTerrain;
	shared_ptr<LayerSplatter> splatter;

	shared_ptr<GameObject> _camera;
public:
	virtual void Init();
	virtual void Start();
	virtual void Update();
	virtual void LateUpdate();
};



