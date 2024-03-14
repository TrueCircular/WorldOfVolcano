#pragma once
#include "engine/IExecute.h"
#include "BaseScene.h"
#include "DungeonScene.h"
#include "MainScene.h"
#include "TestScene.h"

class Demo : public IExecute
{
public:
	virtual void Init() override;
	virtual void Update() override;
	virtual void Render() override;
private:
	
};

extern ClientServiceRef _service;

