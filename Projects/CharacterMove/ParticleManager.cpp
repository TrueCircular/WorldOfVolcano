#include "pch.h"
#include "ParticleManager.h"


ParticleManager::ParticleManager()
{
}

ParticleManager::~ParticleManager()
{
}

ParticleManager* ParticleManager::_instance = nullptr;


ParticleManager* ParticleManager::GetInstance()
{

	if (_instance == nullptr)
		_instance = new ParticleManager();

	return _instance;

}

void ParticleManager::Update()
{
	for (auto c : particleList) {
		if (c.second->GetCountofInstance() != 0) {
			c.second->Update();
		}
	}
}

void ParticleManager::Render()
{
	for (auto c : particleList) {
		if(c.second->GetCountofInstance()!=0){
			c.second->LateUpdate();
		}
	}
}

void ParticleManager::AddManagingParticle(wstring name,shared_ptr<ParticleObj> ParticleObj)
{
	auto iter = particleList.find(name);
	if (iter != particleList.end()) {

	}
	else {
		particleList.insert(make_pair(name.c_str(), ParticleObj));
	}
}

shared_ptr<ParticleObj> ParticleManager::GetParticleFromName(wstring name)
{
	auto iter = particleList.find(name);
	if (iter != particleList.end()) {
		return iter->second;
	}
	return nullptr;
}
