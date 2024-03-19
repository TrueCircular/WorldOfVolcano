#include "pch.h"
#include "ParticleRenderer.h"

void ParticleMeshRenderer::Render(vector<ParticleInstance>& data)
{

	shader->PushGlobalData(Camera::S_MatView, Camera::S_MatProjection);
	auto lightObj = MANAGER_SCENE()->GetCurrentScene()->GetLight();
	if (lightObj)
	{
		shader->PushLightData(lightObj->GetLight()->GetLightDesc());
	}
	shader->PushGlobalData(Camera::S_MatView, Camera::S_MatProjection);


	for (auto& pdata : data)
	{

		ParticleInstanceData insdata;
		

		buffer->AddData(pdata.data);
	

	}

	// IA
	_mesh->GetVertexBuffer()->PushData();
	_mesh->GetIndexBuffer()->PushData();
	buffer->PushData();

	shader->DrawIndexedInstanced(0, _pass, _mesh->GetIndexBuffer()->GetCount(), buffer->GetCount());
}


void ParticleStaticRenderer::Render(vector<ParticleInstance>& data)
{
	if (_model == nullptr)
		return;

	BoneDesc boneDesc;

	const uint32 boneCount = _model->GetBoneCount();
	for (uint32 i = 0; i < boneCount; i++)
	{
		shared_ptr<ModelBone> bone = _model->GetBoneByIndex(i);
		boneDesc.transforms[i] = bone->transform;
	}
	shader->PushBoneData(boneDesc);

	shader->PushGlobalData(Camera::S_MatView, Camera::S_MatProjection);
	auto lightObj = MANAGER_SCENE()->GetCurrentScene()->GetLight();
	if (lightObj)
	{
		shader->PushLightData(lightObj->GetLight()->GetLightDesc());
	}
	shader->PushGlobalData(Camera::S_MatView, Camera::S_MatProjection);
	

	for (auto pData : data)
	{
		ParticleInstanceData insdata;


		buffer->AddData(pData.data);
	}

	const auto& meshes = _model->GetMeshes();
	for (auto& mesh : meshes)
	{

		//Bone Index
		shader->GetScalar("BoneIndex")->SetInt(mesh->boneIndex);

		mesh->vertexBuffer->PushData();
		mesh->indexBuffer->PushData();
		buffer->PushData();

		shader->DrawIndexedInstanced(0, _pass, mesh->indexBuffer->GetCount(), buffer->GetCount());
	}

}

//DONOT USE SINGLE MUST USE WITH OTHER RealPlaying Characters
void ParticleAnimRenderer::Render(vector<ParticleInstance>& data)
{
	if (_animator == nullptr)
		return;

	for (auto& pData : data)
	{
		buffer->AddData(pData.data);
	}
	// GlobalData
	shader->PushGlobalData(Camera::S_MatView, Camera::S_MatProjection);

	// Light
	auto lightObj = MANAGER_SCENE()->GetCurrentScene()->GetLight();
	if (lightObj)
		shader->PushLightData(lightObj->GetLight()->GetLightDesc());

	// SRV를 통해 정보 전달
	shader->GetSRV("TransformMap")->SetResource(_animator->GetTransformSRV());


	// Bones
	BoneDesc boneDesc;

	const uint32 boneCount = _animator->GetModel()->GetBoneCount();
	for (uint32 i = 0; i < boneCount; i++)
	{
		shared_ptr<ModelBone> bone = _animator->GetModel()->GetBoneByIndex(i);
		boneDesc.transforms[i] = bone->transform;
	}
	shader->PushBoneData(boneDesc);

	const auto& meshes = _animator->GetModel()->GetMeshes();
	for (auto& mesh : meshes)
	{
		if (mesh->material) {
			auto mat = mesh->material;
			auto originShader = mat->GetShader();
			mat->SetShader(shader);
			mat->Update();
			mat->SetShader(originShader);
		}
		

		// BoneIndex
		shader->GetScalar("BoneIndex")->SetInt(mesh->boneIndex);

		mesh->vertexBuffer->PushData();
		mesh->indexBuffer->PushData();

		buffer->PushData();
		shader->DrawIndexedInstanced(0, _pass, mesh->indexBuffer->GetCount(), buffer->GetCount());
	}
}
