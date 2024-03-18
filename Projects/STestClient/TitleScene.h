#pragma once
#include "engine\\FrustomCamera.h"


class TitleScene : public Scene
{
private:
	shared_ptr<FrustomCamera> _frustom;
	//camera
	shared_ptr<GameObject> _childCamera;
	//Shader
	shared_ptr<Shader>	_shader;
	//obj
	shared_ptr<GameObject> _titleObj;
	//plane
	shared_ptr<Plain> _titlePlane;
	//tex
	shared_ptr<Texture> _mainTexture;
public:
	virtual void Init();
	virtual void Start();
	virtual void Update();
	virtual void LateUpdate();
};

