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

	shared_ptr<GameObject> _target;
	float					_damage = 0.f;
public:
	ParticleObj() {};
	~ParticleObj() {};
	UINT GetCountofInstance() { return instanceCounter; };
public:
	void SetTargetObject(const shared_ptr<GameObject>& targetObj) { _target = targetObj; }
	void SetEffectDamage(float damage) { _damage = damage; }
public:
	virtual void Clear();
	virtual void AddParticle(shared_ptr<ParticleInstance> data);
	virtual void AddParticle(shared_ptr<ParticleInstance> data, shared_ptr<TweenDesc> desc) {};
	virtual void DeleteInstance(shared_ptr<ParticleInstance>& data);

	virtual void Update();
	virtual void LateUpdate();
//	virtual void ResetParticle();
	virtual void OnDestroy(shared_ptr<ParticleInstance>& instance)=0;
};

