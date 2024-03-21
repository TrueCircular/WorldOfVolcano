#pragma once
#include "QuadTreeBlock.h"

#pragma region Forward Declaration
class FrustomCamera;
class Unit;
#pragma endregion

class TestAbilityScene : public Scene
{
public:
	TestAbilityScene() {}
	virtual ~TestAbilityScene() {}
private:
	//Camera
	shared_ptr<GameObject>		_camera;
	shared_ptr<FrustomCamera>	_frustom;

	//Character
	shared_ptr<Unit> _warrior;
	shared_ptr<Unit> _baronGeddon;

	//Terrain and Terrain Object
	shared_ptr<Terrain>			_terrain;
	shared_ptr<QuadTreeTerrain> _quadTreeTerrain;
	shared_ptr<LayerSplatter>	_splatter;
public:
	virtual void Init() override;
	virtual void Start() override;
	virtual void FixedUpdate() override;
	virtual void Update() override;
	virtual void LateUpdate() override;
};

