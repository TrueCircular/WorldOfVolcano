#pragma once
#include "ParticleInstance.h"

class ParticleRenderer
{
	shared_ptr<ParticleInstancingBuffer> buffer;
public:
	virtual void Render(vector<ParticleInstance>& data)=0;
};

class ParticleMeshRenderer :public ParticleRenderer {
private:
	shared_ptr<Mesh> mesh;
public:
	virtual void Render(vector<ParticleInstance>& data) override;
};
class ParticleStaticRenderer : public ParticleRenderer {
private:
	shared_ptr<Model> model;
public:
	virtual void Render(vector<ParticleInstance>& data) override;

};
class ParticleAnimRenderer : public ParticleRenderer {
private:
	shared_ptr<ModelAnimator> animator;
public:
	virtual void Render(vector<ParticleInstance>& data) override;

};

