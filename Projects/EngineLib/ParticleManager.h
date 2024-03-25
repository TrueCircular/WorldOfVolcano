#pragma once
#include "ParticleObj.h"

class ParticleManager
{
	ParticleManager();
	static ParticleManager* _instance;

public:
	~ParticleManager();
	static ParticleManager* GetInstance();
private:
	void AddDefaultParticle();
public:
	void Init();
	void Update();
	void Render();
	void AddManagingParticle(wstring name, shared_ptr<ParticleObj> ParticleObj);
	void Clear();
	void ClearList();
	shared_ptr<ParticleObj> GetParticleFromName(wstring name);
private:
	map<wstring,shared_ptr<ParticleObj>>particleList;

};

