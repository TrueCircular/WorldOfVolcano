#pragma once
#include "ParticleInstance.h"

class ParticleRenderer
{
protected:
	shared_ptr<Shader> shader;
	shared_ptr<ParticleInstancingBuffer> buffer;
public:
	void SetShader(shared_ptr<Shader> _shader) { shader = _shader; };
	void SetBuffer(shared_ptr<ParticleInstancingBuffer> _buffer) { buffer = _buffer; };
	virtual void Render(vector<ParticleInstance>& data)=0;
};

class ParticleMeshRenderer :public ParticleRenderer {
private:
	shared_ptr<Mesh> _mesh;
public:
	virtual void Render(vector<ParticleInstance>& data) override;
	void SetMesh(shared_ptr<Mesh> mesh);
};
class ParticleStaticRenderer : public ParticleRenderer {
private:
	shared_ptr<Model> _model;
public:
	virtual void Render(vector<ParticleInstance>& data) override;
	void SetModel(shared_ptr<Model> model);

};
class ParticleAnimRenderer : public ParticleRenderer {
private:
	shared_ptr<ModelAnimator> _animator;
public:
	virtual void Render(vector<ParticleInstance>& data) override;
	void SetAnimator(shared_ptr<ModelAnimator> animator);

};

