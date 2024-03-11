#pragma once
#include "ParticleInstance.h"

struct ParticleInstance
{
	ParticleInstanceData data;
	bool isTargeting;
	bool isLoop;
	bool isDestroy;
	weak_ptr<Transform> targetTransform;
	shared_ptr<Transform> particleTransform;
	float speed;
};

class ParticleObj
{
private:
	wstring name;
	vector<ParticleInstance> instanceList;


public:
	virtual void Awake();
	virtual void Start();
	virtual void FixedUpdate();
	virtual void Update();
	virtual void LateUpdate();
};

