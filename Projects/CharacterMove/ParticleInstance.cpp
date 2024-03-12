#include "pch.h"
#include "ParticleInstance.h"

ParticleInstancingBuffer::ParticleInstancingBuffer()
{
	CreateBuffer(MAX_MESH_INSTANCE);
}

ParticleInstancingBuffer::~ParticleInstancingBuffer()
{

}

void ParticleInstancingBuffer::ClearData()
{
	_data.clear();
}

void ParticleInstancingBuffer::AddData(ParticleInstanceData& data)
{
	_data.push_back(data);
}

void ParticleInstancingBuffer::PushData()
{
	const uint32 dataCount = GetCount();
	if (dataCount > _maxCount)
		CreateBuffer(dataCount);

	D3D11_MAPPED_SUBRESOURCE subResource;

	DC()->Map(_instanceBuffer->GetBuffer().Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
	{
		::memcpy(subResource.pData, _data.data(), sizeof(ParticleInstanceData) * dataCount);
	}
	DC()->Unmap(_instanceBuffer->GetBuffer().Get(), 0);

	_instanceBuffer->PushData();
}

void ParticleInstancingBuffer::CreateBuffer(uint32 maxCount /*= MAX_MESH_INSTANCE*/)
{
	_maxCount = maxCount;
	_instanceBuffer = make_shared<VertexBuffer>();

	vector<ParticleInstanceData> temp(maxCount);
	_instanceBuffer->CreateVertexBuffer(temp, /*slot*/1, /*cpuWrite*/true);
}