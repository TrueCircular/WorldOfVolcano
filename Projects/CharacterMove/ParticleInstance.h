#pragma once
#define MAX_PARTICLE_INSTANCE 500
class VertexBuffer;

struct ParticleInstanceData {
	Matrix world;
	float duration;
	float currentime = 0;;
};

struct ParticleInstance
{
	ParticleInstanceData data;
	bool isTargeting = false;
	bool isLoop = false;
	bool isDestroy = false;
	shared_ptr<Transform> targetTransform;
	shared_ptr<Transform> particleTransform;
	float speed;
	ParticleInstance(float duration, shared_ptr<Transform> pos, shared_ptr<Transform> target, float speedVel, bool loop = false) {
		data.duration = duration;
		particleTransform = pos;
		if (target) {
			targetTransform = target;
			isTargeting = true;
		}
		else {
			targetTransform = nullptr;
		}
		speed = speedVel;
		isLoop = loop;
	};

};

class ParticleInstancingBuffer
{
public:
	ParticleInstancingBuffer();
	~ParticleInstancingBuffer();

private:
	void CreateBuffer(uint32 maxCount = MAX_PARTICLE_INSTANCE);

public:
	void ClearData();
	void AddData(ParticleInstanceData& data);

	void PushData();

public:
	uint32						GetCount() { return static_cast<uint32>(_data.size()); }
	shared_ptr<VertexBuffer>	GetBuffer() { return _instanceBuffer; }

private:
	shared_ptr<VertexBuffer>	_instanceBuffer;
	uint32						_maxCount = 0;
	vector<ParticleInstanceData>		_data;
};