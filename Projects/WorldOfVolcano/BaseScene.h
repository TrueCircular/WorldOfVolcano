#pragma once
#include "QuadTreeBlock.h"
#include "SkyDome.h"
//Character
#include "Character.h"
#include "Player.h"
#include "ClientPacketHandler.h"
#include "SpawnManager.h"
//Server
#include "ServerSession.h"
#include "ThreadManager.h"
#include "Service.h"

#pragma region Forward Declaration
class FrustomCamera;
class Unit;

class BaseScene : public Scene
{
public:
	BaseScene() {}
	virtual ~BaseScene() {}
private:
	//Camera
	shared_ptr<GameObject> _camera;
	shared_ptr<FrustomCamera> _frustom;
	//Character
	shared_ptr<Unit> _warrior;
	shared_ptr<Unit> _MagniBronzebeard;
	shared_ptr<Unit> _coreHound;
	//Terrain and Terrain Object
	shared_ptr<Terrain> _terrain;
	shared_ptr<Terrain> _terrainOutLine;
	shared_ptr<QuadTreeTerrain> quadTreeTerrain;
	shared_ptr<QuadTreeTerrain> quadTreeTerrainOutLine;
	shared_ptr<LayerSplatter> splatter;
	shared_ptr<LayerSplatter> splatterOutLine;
	shared_ptr<SkyDome> _skydome;
	//Spawn Pos
	Vec3 spawnPos = Vec3(0, 25, 0);
	//Shader
	shared_ptr<Shader>	_shader;
	shared_ptr<Shader> _terShader;
	//client
	float _threadTimer = 0.0f;
	SendBufferRef _sendBuffer;
	Player_INFO sendInfo; //플레이어 인포
	int latestMessageSize = 0;
	//test
	shared_ptr<Texture> _dissolve;
	bool _isdisv = false;
public:
	virtual void Init();
	virtual void Start();
	virtual void Update();
	virtual void LateUpdate();
};

