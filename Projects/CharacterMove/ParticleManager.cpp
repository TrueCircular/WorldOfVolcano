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
}

void ParticleManager::Render(vector<shared_ptr<GameObject>>& gameObjects)
{

}

void ParticleManager::AddManagingParticle(shared_ptr<ParticleObj> ParticleObj)
{
}
