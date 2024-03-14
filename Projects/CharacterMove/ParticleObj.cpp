#include "pch.h"
#include "ParticleObj.h"

void ParticleObj::AddParticle(ParticleInstance& data)
{
	instanceList.push_back(data);
	++instanceCounter;
}

void ParticleObj::DeleteInstance(ParticleInstance& data)
{
	for (int i = 0; i < instanceList.size();++i) {
		if (instanceList[i].isDestroy) {
			auto iter = instanceList.begin() + i;
			instanceList.erase(iter);
			--i;
			--instanceCounter;
		}
	}
}

void ParticleObj::Update()
{

	for (auto c : instanceList) {
		c.data.currentime+= MANAGER_TIME()->GetDeltaTime();
		if (c.data.currentime > c.duration) {
			if (c.isLoop) {
				c.data.currentime -= c.duration;
			}
			else {
				c.isDestroy = true;
			}
		}
		
	}
}

void ParticleObj::LateUpdate()
{
	if (colorData) {
		colorData->CopyData(_colorDesc);
		if (colorBuffer)
			colorBuffer->SetConstantBuffer(colorData->GetBuffer().Get());
	}
}
