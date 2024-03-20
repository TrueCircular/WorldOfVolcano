#pragma once
#include "ParticleInstance.h"
#include "ParticleRenderer.h"

class ColorDesc;

class ParticleObj
{
protected:
	wstring name;
	vector<shared_ptr<ParticleInstance>> instanceList;
	shared_ptr<ParticleInstancingBuffer> instanceBuffer;
	shared_ptr<ParticleMeshRenderer> meshRenderer;
	shared_ptr<ParticleStaticRenderer> staticRenderer;
	shared_ptr<ParticleAnimRenderer> animRenderer;
	shared_ptr<Shader> shader;


	ColorDesc _colorDesc;
	shared_ptr<ConstantBuffer<ColorDesc>> colorData;
	ComPtr<ID3DX11EffectConstantBuffer> colorBuffer;
	UINT instanceCounter=0;

public:
	ParticleObj() {};
	~ParticleObj() {};
	UINT GetCountofInstance() { return instanceCounter; };
public:
	virtual void AddParticle(shared_ptr<ParticleInstance> data);
	virtual void AddParticle(shared_ptr<ParticleInstance> data, shared_ptr<TweenDesc> desc) {};
	virtual void DeleteInstance(shared_ptr<ParticleInstance>& data);
	
	virtual void Update();
	virtual void LateUpdate();
//	virtual void ResetParticle();
	virtual void OnDestroy(shared_ptr<ParticleInstance>& instance)=0;
};

