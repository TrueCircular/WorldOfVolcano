#include "pch.h"
#include "ParticleObj.h"

void ParticleObj::AddParticle(ParticleInstance& data)
{
	instanceList.push_back(data);
}

void ParticleObj::DeleteInstance(ParticleInstance& data)
{
	for (int i = 0; i < instanceList.size();++i) {
		if (instanceList[i].isDestroy) {
			auto iter = instanceList.begin() + i;
			instanceList.erase(iter);
			--i;
		}
	}
}

void ParticleObj::Update()
{
	colorBuffer->
}
