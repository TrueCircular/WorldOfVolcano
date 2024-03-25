#include "pch.h"
#include "ParticleManager.h"
//Particles
#include "WarriorRoar.h"
#include "Clap.h"
#include "Smoke1.h"
#include "Smoke2.h"
#include "Smoke3.h"
#include "FireStorm.h"
#include "FireBall.h"
#include "Polar.h"
#include "MagicCircle.h"
#include "LineSpark.h"
#include "ShineHelper.h"
#include "Exlode.h"

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

void ParticleManager::AddDefaultParticle()
{
	shared_ptr<WarriorRoar> warriorRoar = make_shared<WarriorRoar>();
	shared_ptr<Clap> clap = make_shared<Clap>();
	shared_ptr<Smoke1> smoke1 = make_shared<Smoke1>();
	shared_ptr<Smoke2> smoke2 = make_shared<Smoke2>();
	shared_ptr<Smoke3> smoke3 = make_shared<Smoke3>();
	shared_ptr<FireStorm> storm = make_shared<FireStorm>();
	shared_ptr<FireBall> fireBall = make_shared<FireBall>();
	shared_ptr<Polar> polar = make_shared<Polar>();
	shared_ptr<MagicCircle> magicCircle = make_shared<MagicCircle>();
	shared_ptr<LineSpark> lineSpark = make_shared<LineSpark>();
	shared_ptr<Exlode> explode = make_shared<Exlode>();
	shared_ptr<ShineHelper> sparkHelper = make_shared<ShineHelper>();

	AddManagingParticle(L"WarriorRoar", warriorRoar);
	AddManagingParticle(L"Explode", explode);
	AddManagingParticle(L"SparkHelper", sparkHelper);
	AddManagingParticle(L"Clap", clap);
	AddManagingParticle(L"Smoke1", smoke1);
	AddManagingParticle(L"Smoke3", smoke3);
	AddManagingParticle(L"Smoke2", smoke2);
	AddManagingParticle(L"FireStorm", storm);
	AddManagingParticle(L"FireBall", fireBall);
	AddManagingParticle(L"Polar", polar);
	AddManagingParticle(L"MagicCircle", magicCircle);
	AddManagingParticle(L"LineSpark", lineSpark);
}

void ParticleManager::Init()
{
	AddDefaultParticle();
}

void ParticleManager::Update()
{
	for (auto& c : particleList) {
		if (c.second->GetCountofInstance() != 0) {
			c.second->Update();
		}
	}
}

void ParticleManager::Render()
{
	for (auto& c : particleList) {
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

void ParticleManager::Clear()
{
	for (auto& c : particleList) {
		c.second->Clear();
	}
}

void ParticleManager::ClearList()
{
	for (auto& c : particleList) {
		c.second->Clear();
	}
	particleList.clear();
}

shared_ptr<ParticleObj> ParticleManager::GetParticleFromName(wstring name)
{
	auto iter = particleList.find(name);
	if (iter != particleList.end()) {
		return iter->second;
	}
	return nullptr;
}
