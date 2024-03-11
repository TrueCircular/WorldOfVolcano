#pragma once
#include "ParticleObj.h"

class ParticleManager
{
	ParticleManager();
	~ParticleManager();
	static ParticleManager* _instance;
public:
	static ParticleManager* GetInstance();
public:
	void Render(vector<shared_ptr<GameObject>>& ParticleObj);
private:
	void RenderMeshRenderer(vector<shared_ptr<GameObject>>& ParticleObj);
	void RenderModelRenderer(vector<shared_ptr<GameObject>>& ParticleObj);
	void RenderAnimRenderer(vector<shared_ptr<GameObject>>& ParticleObj);
private:
	void AddManagingParticle(shared_ptr<ParticleObj> ParticleObj);
private:
	vector<shared_ptr<ParticleObj>> particleList;

};

