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

#pragma region Forward Declaration
class FrustomCamera;
class Unit;

class DungeonScene : public Scene
{
public:
	DungeonScene() {}
	virtual ~DungeonScene() {}
private:
	//Camera
	shared_ptr<GameObject> _camera;
	shared_ptr<FrustomCamera> _frustom;
	//Character
	shared_ptr<Unit> _warrior;
	shared_ptr<Unit> _coreHound;
	shared_ptr<Unit> _baronGeddon;
	//Terrain and Terrain Object
	shared_ptr<Terrain>			_terrain;
	shared_ptr<QuadTreeTerrain> _quadTreeTerrain;
	shared_ptr<LayerSplatter>	_splatter;
	shared_ptr<Skybox> skyBox;
	//Spawn Pos
	Vec3 spawnPos = Vec3(-374, 25, 338);
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


