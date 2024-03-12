#pragma once
#include "ParticleObj.h"

class ParticleManager
{
	ParticleManager();
	static ParticleManager* _instance;
public:
	~ParticleManager();
	static ParticleManager* GetInstance();
	void Update();
	void Render(vector<shared_ptr<GameObject>>& ParticleObj);
	void AddManagingParticle(shared_ptr<ParticleObj> ParticleObj);
private:
	vector<shared_ptr<ParticleObj>> particleList;

};

