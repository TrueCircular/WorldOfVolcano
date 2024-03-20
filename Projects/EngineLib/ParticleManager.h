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
	void Render();
	void AddManagingParticle(wstring name, shared_ptr<ParticleObj> ParticleObj);
	shared_ptr<ParticleObj> GetParticleFromName(wstring name);
private:
	map<wstring,shared_ptr<ParticleObj>>particleList;

};

