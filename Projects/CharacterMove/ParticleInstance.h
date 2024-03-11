#pragma once
#define MAX_PARTICLE_INSTANCE 500
class VertexBuffer;

struct ParticleInstanceData {
	Matrix world;
	float duration;
	float currentime;
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

	void	SetID(uint64 instanceId) { _instanceId = instanceId; }
	uint64	GetID() { return _instanceId; }

private:
	uint64						_instanceId = 0;
	shared_ptr<VertexBuffer>	_instanceBuffer;
	uint32						_maxCount = 0;
	vector<ParticleInstanceData>		_data;
};