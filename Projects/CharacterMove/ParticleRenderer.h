#pragma once
#include "ParticleInstance.h"

class ParticleRenderer
{
	shared_ptr<ParticleInstancingBuffer> buffer;
public:
	virtual void Render()=0;
};

class ParticleMeshRenderer :public ParticleRenderer {
private:

public:
	virtual void Render() override;
};
class ParticleStaticRenderer : public ParticleRenderer {
private:

public:
	virtual void Render() override;

};
class ParticleAnimRenderer : public ParticleRenderer {
private:

public:
	virtual void Render() override;

};

