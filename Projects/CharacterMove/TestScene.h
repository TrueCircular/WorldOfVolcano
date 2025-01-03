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

class ColorDesc;
class FresnelDesc;

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

	shared_ptr<Shader> effShader4;
	shared_ptr<Texture> quakeTex;
	shared_ptr<Transform> world4;

	shared_ptr<Shader> effShader5;
	shared_ptr<Texture> polarTex;
	shared_ptr<Transform> world5;

	shared_ptr<Shader> effShader6;
	shared_ptr<Texture> smoke1Tex1;
	shared_ptr<Texture> smoke1Tex2;
	shared_ptr<Transform> world6;

	shared_ptr<Shader> effShader7;
	shared_ptr<Texture> smoke2Tex1;
	shared_ptr<Texture> smoke2Tex2;
	shared_ptr<Transform> world7;


	shared_ptr<Shader> effShader8;
	shared_ptr<Texture> smoke3Tex;
	shared_ptr<Transform> world8;

	shared_ptr<Shader> effShader9;
	shared_ptr<Texture> lineSparkTex;
	shared_ptr<Transform> world9;

//	float maxtime = 15.0f;
	float maxtime = 3.0f;
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



