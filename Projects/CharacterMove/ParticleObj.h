#pragma once
#include "ParticleInstance.h"
#include "ParticleRenderer.h"


class ParticleObj
{
protected:
	wstring name;
	vector<ParticleInstance> instanceList;
	shared_ptr<ParticleMeshRenderer> meshRenderer;
	shared_ptr<ParticleStaticRenderer> staticRenderer;
	shared_ptr<ParticleAnimRenderer> animRenderer;
	shared_ptr<Shader> shader;


	ColorDesc _colorDesc;
	shared_ptr<ConstantBuffer<ColorDesc>> colorBuffer;
	UINT instanceCounter=0;

public:
	ParticleObj() {};
	~ParticleObj() {};
	UINT GetCountofInstance() { return instanceCounter; };
public:
	virtual void AddParticle(ParticleInstance& data);
	virtual void DeleteInstance(ParticleInstance& data);
	
	virtual void Update();
	virtual void LateUpdate()=0;

	virtual void OnDestroy()=0;
};

