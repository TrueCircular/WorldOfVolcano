#pragma once
#define MAX_PARTICLE_INSTANCE 500
class VertexBuffer;

struct ParticleInstanceData {
	Matrix world;
	float currentime = 0;
};

struct ParticleInstance
{
	ParticleInstanceData data;
	bool isTargeting = false;
	bool isLoop = false;
	bool isDestroy = false;
	shared_ptr<Transform> targetTransform;
	shared_ptr<Transform> particleTransform;
	shared_ptr<Transform> parentTransform;
	float speed=1.0f;
	float duration=0.0f;
	ParticleInstance(float _duration, shared_ptr<Transform> pos, shared_ptr<Transform> target, float speedVel, bool loop = false) {
		duration = _duration;
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
public:
	void Reset() {
		data.currentime = 0.0f;
		data.world = Matrix::Identity;
		isDestroy = false;
	}
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