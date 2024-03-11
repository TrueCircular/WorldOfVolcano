#include "pch.h"
#include "ParticleManager.h"
//
//
//ParticleManager::ParticleManager()
//{
//}
//
//ParticleManager::~ParticleManager()
//{
//}
//
//ParticleManager* ParticleManager::_instance = nullptr;
//
//
//ParticleManager* ParticleManager::GetInstance()
//{
//
//	if (_instance == nullptr)
//		_instance = new ParticleManager();
//
//	return _instance;
//
//}
//
//void ParticleManager::Render(vector<shared_ptr<GameObject>>& gameObjects)
//{
//	ClearData();
//
//	RenderMeshRenderer(gameObjects);
//	RenderModelRenderer(gameObjects);
//	RenderAnimRenderer(gameObjects);
//}
//
//void ParticleManager::RenderMeshRenderer(vector<shared_ptr<GameObject>>& gameObjects)
//{
//	map<InstanceID, vector<shared_ptr<GameObject>>> cache;
//
//	for (shared_ptr<GameObject>& gameObject : gameObjects)
//	{
//		if (gameObject->GetMeshRenderer() == nullptr)
//			continue;
//
//		const InstanceID instanceId = gameObject->GetMeshRenderer()->GetInstanceID();
//		cache[instanceId].push_back(gameObject);
//	}
//
//	for (auto& pair : cache)
//	{
//		const vector<shared_ptr<GameObject>>& vec = pair.second;
//
//		//if (vec.size() == 1)
//		//{
//		//	vec[0]->GetMeshRenderer()->RenderSingle();
//		//}
//		//else
//		{
//			const InstanceID instanceId = pair.first;
//
//			for (int32 i = 0; i < vec.size(); i++)
//			{
//				const shared_ptr<GameObject>& gameObject = vec[i];
//				InstancingData data;
//				data.world = gameObject->GetTransform()->GetWorldMatrix();
//
//				AddData(instanceId, data);
//			}
//
//			shared_ptr<InstancingBuffer>& buffer = _buffers[instanceId];
//			vec[0]->GetMeshRenderer()->RenderInstancing(buffer);
//		}
//	}
//}
//
//
//void ParticleManager::RenderModelRenderer(vector<shared_ptr<GameObject>>& gameObjects)
//{
//	map<InstanceID, vector<shared_ptr<GameObject>>> cache;
//
//	for (shared_ptr<GameObject>& gameObject : gameObjects)
//	{
//		if (gameObject->GetModelRenderer() == nullptr)
//			continue;
//
//		const InstanceID instanceId = gameObject->GetModelRenderer()->GetInstanceID();
//		cache[instanceId].push_back(gameObject);
//	}
//
//	for (auto& pair : cache)
//	{
//		const vector<shared_ptr<GameObject>>& vec = pair.second;
//
//		//if (vec.size() == 1)
//		//{
//		//	vec[0]->GetMeshRenderer()->RenderSingle();
//		//}
//		//else
//		{
//			const InstanceID instanceId = pair.first;
//
//			for (int32 i = 0; i < vec.size(); i++)
//			{
//				const shared_ptr<GameObject>& gameObject = vec[i];
//				InstancingData data;
//				data.world = gameObject->GetTransform()->GetWorldMatrix();
//
//				AddData(instanceId, data);
//			}
//
//			shared_ptr<InstancingBuffer>& buffer = _buffers[instanceId];
//			MANAGER_SHADOW()->SetShadow(vec[0]->GetModelRenderer()->GetShader());
//			vec[0]->GetModelRenderer()->RenderInstancing(buffer);
//		}
//	}
//}
//
//void ParticleManager::RenderAnimRenderer(vector<shared_ptr<GameObject>>& gameObjects)
//{
//	map<InstanceID, vector<shared_ptr<GameObject>>> cache;
//
//	for (shared_ptr<GameObject>& gameObject : gameObjects)
//	{
//		if (gameObject->GetModelAnimator() == nullptr)
//			continue;
//
//		const InstanceID instanceId = gameObject->GetModelAnimator()->GetInstanceID();
//		cache[instanceId].push_back(gameObject);
//	}
//
//	for (shared_ptr<GameObject>& gameObject : gameObjects)
//	{
//		const auto& temp = gameObject->GetChildByName(L"Model");
//		if (temp)
//		{
//			const InstanceID instanceId = temp->GetModelAnimator()->GetInstanceID();
//			cache[instanceId].push_back(gameObject);
//		}
//	}
//
//	for (auto& pair : cache)
//	{
//		shared_ptr<InstancedTweenDesc> tweenDesc = make_shared<InstancedTweenDesc>();
//
//		const vector<shared_ptr<GameObject>>& vec = pair.second;
//
//		//if (vec.size() == 1)
//		//{
//		//	vec[0]->GetModelAnimator()->RenderSingle();
//		//}
//		//else
//		{
//			const InstanceID instanceId = pair.first;
//
//			for (int32 i = 0; i < vec.size(); i++)
//			{
//				const shared_ptr<GameObject>& gameObject = vec[i];
//				InstancingData data;
//				data.world = gameObject->GetTransform()->GetWorldMatrix();
//
//
//				// INSTANCING
//				if (gameObject->GetModelAnimator() == nullptr)
//				{
//					gameObject->GetChildByName(L"Model")->GetModelAnimator()->UpdateTweenData();
//					tweenDesc->tweens[i] = *gameObject->GetChildByName(L"Model")->GetModelAnimator()->GetTweenDesc();
//					data.world = gameObject->GetChildByName(L"Model")->GetTransform()->GetWorldMatrix();
//
//				}
//				else
//				{
//					gameObject->GetModelAnimator()->UpdateTweenData();
//					tweenDesc->tweens[i] = *gameObject->GetModelAnimator()->GetTweenDesc();
//
//				}
//				AddData(instanceId, data);
//
//			}
//
//			if (vec[0]->GetModelAnimator() == nullptr)
//			{
//				vec[0]->GetChildByName(L"Model")->GetModelAnimator()->GetShader()->PushTweenData(*tweenDesc.get());
//				shared_ptr<InstancingBuffer>& buffer = _buffers[instanceId];
//				MANAGER_SHADOW()->SetShadow(vec[0]->GetChildByName(L"Model")->GetModelAnimator()->GetShader());
//				vec[0]->GetChildByName(L"Model")->GetModelAnimator()->RenderInstancing(buffer);
//			}
//			else
//			{
//				vec[0]->GetModelAnimator()->GetShader()->PushTweenData(*tweenDesc.get());
//				shared_ptr<InstancingBuffer>& buffer = _buffers[instanceId];
//
//				MANAGER_SHADOW()->SetShadow(vec[0]->GetModelAnimator()->GetShader());
//
//				vec[0]->GetModelAnimator()->RenderInstancing(buffer);
//			}
//
//		}
//	}
//}
