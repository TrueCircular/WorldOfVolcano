#pragma once
#include "ParticleInstance.h"
#include "ParticleRenderer.h"


class ParticleObj
{
private:
	wstring name;
	vector<ParticleInstance> instanceList;
	shared_ptr<ParticleMeshRenderer> meshRenderer;
	shared_ptr<ParticleStaticRenderer> staticRenderer;
	shared_ptr<ParticleAnimRenderer> animRenderer;
	shared_ptr<Shader> shader;
	UINT instanceCounter=0;

public:
	ParticleObj() {};
	~ParticleObj() {};
	virtual void AddParticle(ParticleInstance& data);

	virtual void Update()=0;
	virtual void LateUpdate()=0;

	virtual void DeleteInstance(ParticleInstance& data)=0;
	virtual void OnDestroy()=0;
};

