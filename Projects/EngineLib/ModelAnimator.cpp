#include "pch.h"
#include "ModelAnimator.h"
#include <float.h>
#include "EquipmentSlot.h"

ModelAnimator::ModelAnimator() : Super(ComponentType::ModelAnimator)
{
	_tweenDesc = make_shared<TweenDesc>();
}

ModelAnimator::ModelAnimator(shared_ptr<Shader> shader) : Super(ComponentType::ModelAnimator), _shader(shader)
{
	_tweenDesc = make_shared<TweenDesc>();
}

ModelAnimator::~ModelAnimator()
{
}

void ModelAnimator::CreateTexture()
{
	if (_model->GetAnimationCount() == 0)
		return;

	_animTransforms.resize(_model->GetAnimationCount());
	for (uint32 i = 0; i < _model->GetAnimationCount(); i++)
		CreateAnimationTransform(i);

	// Creature Texture
	{
		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
		desc.Width = MAX_MODEL_TRANSFORMS * 4;
		desc.Height = MAX_MODEL_KEYFRAMES;
		desc.ArraySize = _model->GetAnimationCount();
		desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT; // 16바이트
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.MipLevels = 1;
		desc.SampleDesc.Count = 1;

		const uint32 dataSize = MAX_MODEL_TRANSFORMS * sizeof(Matrix);
		const uint32 pageSize = dataSize * MAX_MODEL_KEYFRAMES;
		void* mallocPtr = ::malloc(pageSize * _model->GetAnimationCount());

		// 파편화된 데이터를 조립한다.
		for (uint32 c = 0; c < _model->GetAnimationCount(); c++)
		{
			uint32 startOffset = c * pageSize;

			BYTE* pageStartPtr = reinterpret_cast<BYTE*>(mallocPtr) + startOffset;

			for (uint32 f = 0; f < MAX_MODEL_KEYFRAMES; f++)
			{
				void* ptr = pageStartPtr + dataSize * f;
				::memcpy(ptr, _animTransforms[c].transforms[f].data(), dataSize);
			}
		}

		// 리소스 만들기
		vector<D3D11_SUBRESOURCE_DATA> subResources(_model->GetAnimationCount());

		for (uint32 c = 0; c < _model->GetAnimationCount(); c++)
		{
			void* ptr = (BYTE*)mallocPtr + c * pageSize;
			subResources[c].pSysMem = ptr;
			subResources[c].SysMemPitch = dataSize;
			subResources[c].SysMemSlicePitch = pageSize;
		}

		HRESULT hr = DEVICE()->CreateTexture2D(&desc, subResources.data(), _texture.GetAddressOf());
		CHECK(hr);

		::free(mallocPtr);
	}

	// Create SRV
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
		desc.Texture2DArray.MipLevels = 1;
		desc.Texture2DArray.ArraySize = _model->GetAnimationCount();

		HRESULT hr = DEVICE()->CreateShaderResourceView(_texture.Get(), &desc, _srv.GetAddressOf());
		CHECK(hr);
	}
}

void ModelAnimator::CreateAnimationTransform(uint32 index)
{
	vector<Matrix> tempAnimBoneTransforms(MAX_MODEL_TRANSFORMS, Matrix::Identity);

	shared_ptr<ModelAnimation> animation = _model->GetAnimationByIndex(index);

	for (uint32 f = 0; f < animation->frameCount; f++)
	{
		for (uint32 b = 0; b < _model->GetBoneCount(); b++)
		{
			shared_ptr<ModelBone> bone = _model->GetBoneByIndex(b);

			Matrix matAnimation;

			shared_ptr<ModelKeyframe> frame = animation->GetKeyframe(bone->name);

			if (frame != nullptr)
			{
				ModelKeyframeData& data = frame->transforms[f];

				Matrix S, R, T;
				S = Matrix::CreateScale(data.scale.x, data.scale.y, data.scale.z);
				R = Matrix::CreateFromQuaternion(data.rotation);
				T = Matrix::CreateTranslation(data.translation.x, data.translation.y, data.translation.z);
				matAnimation = S * R * T;
			}
			else
			{
				matAnimation = Matrix::Identity;
			}

			Matrix toRootMatrix = bone->transform;
			Matrix invGlobal = toRootMatrix.Invert();
			int32 parentIndex = bone->parentIndex;

			Matrix matParent = Matrix::Identity;
			if (parentIndex >= 0)
				matParent = tempAnimBoneTransforms[parentIndex];

			tempAnimBoneTransforms[b] = matAnimation * matParent;

			// 결론
			_animTransforms[index].transforms[f][b] = invGlobal * tempAnimBoneTransforms[b];
		}
	}
}

vector<AnimTransform>& ModelAnimator::GetAnimTransform()
{
	return  _animTransforms;
}

bool ModelAnimator::SetCurrentAnimation(wstring animName)
{
	int num = 0;

	for (const auto& anim : _anims)
	{
		if (anim->name == animName)
		{
			_currentAnim = anim;
			_tweenDesc->current.animIndex = num;
			return true;
		}

		num++;
	}

	return false;
}

void ModelAnimator::UpdateTweenData()
{
	if (_isPlay)
	{
		if (_model == nullptr || _shader == nullptr)
			return;

		if (_texture == nullptr)
			CreateTexture();

		//루프 애니메이션
		if (_isLoop)
		{
			//현재 애니메이션
			{
				if (_currentAnim)
				{


					_tweenDesc->current.sumTime += MANAGER_TIME()->GetDeltaTime();
					_timePerFrame = 1 / (_currentAnim->frameRate * _tweenDesc->current.speed);

					if (_tweenDesc->current.sumTime > _timePerFrame + FLT_EPSILON)
					{
						_tweenDesc->current.sumTime = 0;
						_tweenDesc->current.currentFrame = (_tweenDesc->current.currentFrame + 1) % _currentAnim->frameCount;
						_tweenDesc->current.nextFrame = (_tweenDesc->current.currentFrame + 1) % _currentAnim->frameCount;
					}

					if (((_tweenDesc->current.nextFrame + 1) % _currentAnim->frameCount) == 0)
					{
						_isFrameEnd = true;
					}

					_tweenDesc->current.ratio = (_tweenDesc->current.sumTime / _timePerFrame);
				}
			}
			//다음 애니메이션 예약 시
			if (_tweenDesc->next.animIndex >= 0)
			{
				_tweenDesc->tweenSumTime += MANAGER_TIME()->GetDeltaTime();
				_tweenDesc->tweenRatio = _tweenDesc->tweenSumTime / _tweenDesc->tweenDuration;

				if (_tweenDesc->tweenRatio > 1.f)
				{
					_tweenDesc->current = _tweenDesc->next;
					_currentAnim = _nextAnim;
					_nextAnim = nullptr;
					_tweenDesc->ClearNextAnim();
				}
				else
				{
					if (_nextAnim)
					{
						_tweenDesc->next.sumTime += MANAGER_TIME()->GetDeltaTime();

						float timeperFrame = 1.f / (_nextAnim->frameRate * _tweenDesc->next.speed);

						if (_tweenDesc->next.ratio > 1.f + FLT_EPSILON)
						{
							_tweenDesc->next.sumTime = 0;
							_tweenDesc->next.currentFrame = (_tweenDesc->next.currentFrame + 1) % _nextAnim->frameCount;
							_tweenDesc->next.nextFrame = (_tweenDesc->next.currentFrame + 1) % _nextAnim->frameCount;
						}

						_tweenDesc->next.ratio = _tweenDesc->next.sumTime / timeperFrame;
					}
				}
			}
			UpdateEquipmentTransform();
		}
		//논 루프 애니메이션
		else
		{
			//현재 애니메이션
			{
				if (_currentAnim)
				{


					_tweenDesc->current.sumTime += MANAGER_TIME()->GetDeltaTime();
					_timePerFrame = 1 / (_currentAnim->frameRate * _tweenDesc->current.speed);

					if (_tweenDesc->current.sumTime > _timePerFrame + FLT_EPSILON)
					{
						_tweenDesc->current.sumTime = 0;
						_tweenDesc->current.currentFrame = (_tweenDesc->current.currentFrame + 1) % _currentAnim->frameCount;
						_tweenDesc->current.nextFrame = (_tweenDesc->current.currentFrame + 1) % _currentAnim->frameCount;
					}

					if (((_tweenDesc->current.nextFrame + 1) % _currentAnim->frameCount) == 0)
					{
						_tweenDesc->current.currentFrame = _currentAnim->frameCount - 1;
						_tweenDesc->current.nextFrame = _tweenDesc->current.currentFrame;
						_isPlay = false; 
						_isFrameEnd = true; 
					}

					_tweenDesc->current.ratio = (_tweenDesc->current.sumTime / _timePerFrame);
				}
			}
			//다음 애니메이션 예약 시
			if (_tweenDesc->next.animIndex >= 0)
			{
				_tweenDesc->tweenSumTime += MANAGER_TIME()->GetDeltaTime();
				_tweenDesc->tweenRatio = _tweenDesc->tweenSumTime / _tweenDesc->tweenDuration;

				if (_tweenDesc->tweenRatio > 1.f)
				{
					_tweenDesc->current = _tweenDesc->next;
					_currentAnim = _nextAnim;
					_nextAnim = nullptr;
					_tweenDesc->ClearNextAnim();
				}
				else
				{
					if (_nextAnim)
					{
						_tweenDesc->next.sumTime += MANAGER_TIME()->GetDeltaTime();

						float timeperFrame = 1.f / (_nextAnim->frameRate * _tweenDesc->next.speed);

						if (_tweenDesc->next.ratio > 1.f + FLT_EPSILON)
						{
							_tweenDesc->next.sumTime = 0;
							_tweenDesc->next.currentFrame = (_tweenDesc->next.currentFrame + 1) % _nextAnim->frameCount;
							_tweenDesc->next.nextFrame = (_tweenDesc->next.currentFrame + 1) % _nextAnim->frameCount;
						}

						_tweenDesc->next.ratio = _tweenDesc->next.sumTime / timeperFrame;
					}
				}
			}
			UpdateEquipmentTransform();
		}

		// SRV를 통해 정보 전달
		_shader->GetSRV("TransformMap")->SetResource(_srv.Get());
	}
}

bool ModelAnimator::SetNextAnimation(wstring animName)
{
	int num = 0;
	for (const auto& anim : _anims)
	{
		if (anim->name == animName)
		{
			_nextAnim = anim;
			_tweenDesc->next.animIndex = num;

			if (_currentAnim != nullptr)
			{
				if (_currentAnim->name == L"Stand" && _nextAnim->name == L"Damaged")
				{
					_tweenDesc->tweenDuration = 1.f / _nextAnim->duration;
				}
				else if (_currentAnim->name == L"Damaged" && _nextAnim->name == L"Stand")
				{
					_tweenDesc->tweenDuration = 1.f / _nextAnim->duration;
				}
				else if (_currentAnim->name == L"Attack" && _nextAnim->name == L"Battle")
				{
					_tweenDesc->tweenDuration = 1.f / _nextAnim->duration;
				}
				else if (_currentAnim->name == L"Battle" && _nextAnim->name == L"Attack")
				{
					_tweenDesc->tweenDuration = 1.f / _nextAnim->duration;
				}
				else if (_currentAnim->name == L"Attack1" && _nextAnim->name == L"Battle")
				{
					_tweenDesc->tweenDuration = 1.f / _nextAnim->duration;
				}
				else if (_currentAnim->name == L"Battle" && _nextAnim->name == L"Attack1")
				{
					_tweenDesc->tweenDuration = 1.f / _nextAnim->duration;
				}
				else if (_currentAnim->name == L"Attack2" && _nextAnim->name == L"Battle")
				{
					_tweenDesc->tweenDuration = 1.f / _nextAnim->duration;
				}
				else if (_currentAnim->name == L"Battle" && _nextAnim->name == L"Attack2")
				{
					_tweenDesc->tweenDuration = 1.f / _nextAnim->duration;
				}
				else
				{
					_tweenDesc->tweenDuration = 0.18f;
				}
			}
	
			return true;
		}
		num++;
	}

	return false;
}
void ModelAnimator::SetModel(shared_ptr<Model> model)
{
	_model = model;

	const auto& materials = _model->GetMaterials();
	for (auto& material : materials)
	{
		material->SetShader(_shader);
	}
}

void ModelAnimator::RenderInstancing(shared_ptr<class InstancingBuffer>& buffer)
{
	if (_model == nullptr)
		return;
	if (_texture == nullptr)
		CreateTexture();

	// GlobalData
	_shader->PushGlobalData(Camera::S_MatView, Camera::S_MatProjection);

	// Light
	auto lightObj = MANAGER_SCENE()->GetCurrentScene()->GetLight();
	if (lightObj)
		_shader->PushLightData(lightObj->GetLight()->GetLightDesc());

	// SRV를 통해 정보 전달
	_shader->GetSRV("TransformMap")->SetResource(_srv.Get());


	// Bones
	BoneDesc boneDesc;

	const uint32 boneCount = _model->GetBoneCount();
	for (uint32 i = 0; i < boneCount; i++)
	{
		shared_ptr<ModelBone> bone = _model->GetBoneByIndex(i);
		boneDesc.transforms[i] = bone->transform;
	}
	_shader->PushBoneData(boneDesc);

	const auto& meshes = _model->GetMeshes();
	for (auto& mesh : meshes)
	{
		if (mesh->material)
			mesh->material->Update();

		// BoneIndex
		_shader->GetScalar("BoneIndex")->SetInt(mesh->boneIndex);

		mesh->vertexBuffer->PushData();
		mesh->indexBuffer->PushData();

		buffer->PushData();
		_shader->DrawIndexedInstanced(0, _pass, mesh->indexBuffer->GetCount(), buffer->GetCount());
	}
}

InstanceID ModelAnimator::GetInstanceID()
{
	return make_pair((uint64)_model.get(), (uint64)_shader.get());
}

void ModelAnimator::Start()
{
	assert(_model != nullptr);
	assert(_shader != nullptr);

	if (_model)
	{
		if (_anims.size() > 0)
		{
			_anims.clear();
		}

		_equipmentBoneIndexList = _model->GetEquipmentBoneIndex();

		uint32 count = _model->GetAnimationCount();
		if (count > 0)
		{
			for (uint32 i = 0; i < count; i++)
			{
				_anims.push_back(_model->GetAnimationByIndex(i));
			}
		}
	}
}

void ModelAnimator::ShadowUpdate() {


	// 애니메이션 현재 프레임 정보
	_shader->PushTweenData(*_tweenDesc);

	// SRV를 통해 정보 전달
	_shader->GetSRV("TransformMap")->SetResource(_srv.Get());

	//Render
	{
		//Bone
		BoneDesc boneDesc;

		const uint32 boneCount = _model->GetBoneCount();
		for (uint32 i = 0; i < boneCount; i++)
		{
			shared_ptr<ModelBone> bone = _model->GetBoneByIndex(i);
			boneDesc.transforms[i] = bone->transform;
		}

		_shader->PushBoneData(boneDesc);
		_shader->PushGlobalData(MANAGER_SHADOW()->GetShadowDesc().shadowView, MANAGER_SHADOW()->GetShadowDesc().shadowProj);

		auto lightObj = MANAGER_SCENE()->GetCurrentScene()->GetLight();
		if (lightObj)
			_shader->PushLightData(lightObj->GetLight()->GetLightDesc());

		auto world = GetTransform()->GetWorldMatrix();
		_shader->PushTransformData(TransformDesc{ world });

		const auto& meshes = _model->GetMeshes();
		for (auto& mesh : meshes)
		{
			if (mesh->material)
				mesh->material->Update();

			//Bone Index
			_shader->GetScalar("BoneIndex")->SetInt(mesh->boneIndex);

			uint32 stride = mesh->vertexBuffer->GetStride();
			uint32 offset = mesh->vertexBuffer->GetOffset();
			//_shader->PushGlobalData(Camera::S_MatView, Camera::S_MatProjection);

			DC()->IASetVertexBuffers(0, 1, mesh->vertexBuffer->GetBuffer().GetAddressOf(),
				&stride, &offset);
			DC()->IASetIndexBuffer(mesh->indexBuffer->GetBuffer().Get(),
				DXGI_FORMAT_R32_UINT, 0);
			DC()->OMSetBlendState(nullptr, 0, -1);

			_shader->DrawIndexed(0, _pass, mesh->indexBuffer->GetCount(), 0, 0);
		}
	}

	float fWidthLength = 513 * 513;
	float fHeightLength = 513 * 513;
	float viewdis = sqrt(fWidthLength + fHeightLength);
	//Global

}

void ModelAnimator::UpdateEquipmentTransform()
{
	auto equipmentSlot = GetGameObject()->GetParent()->GetComponent<EquipmentSlot>();

	if (equipmentSlot == nullptr)
		return;

	//Helm
	{
		auto helm = equipmentSlot->GetEquipmentItem(0);

		if (helm != nullptr)
		{
			int currentIndex = _tweenDesc->current.animIndex;
			uint32 currentFrame = _tweenDesc->current.currentFrame;
			uint32 currentNextFrame = _tweenDesc->current.nextFrame;
			int nextIndex = _tweenDesc->next.animIndex;
			uint32 nextFrame = _tweenDesc->next.currentFrame;
			uint32 nextNextFrame = _tweenDesc->next.nextFrame;
			int boneIndex = _equipmentBoneIndexList.HelmIndex;

			Matrix currentItemMat = _animTransforms[currentIndex].transforms[currentFrame][boneIndex];
			currentItemMat = _model->GetBoneByIndex(boneIndex)->parent->transform * currentItemMat;
			Matrix currentNextItemMat = _animTransforms[currentIndex].transforms[currentNextFrame][boneIndex];
			currentNextItemMat = _model->GetBoneByIndex(boneIndex)->parent->transform * currentNextItemMat;

			Matrix interpolatedTransform = Matrix::Lerp(currentItemMat, currentNextItemMat, _tweenDesc->current.ratio);

			if (_nextAnim != nullptr)
			{
				Matrix nextItemMat = _animTransforms[nextIndex].transforms[nextFrame][boneIndex];
				nextItemMat = _model->GetBoneByIndex(boneIndex)->parent->transform * nextItemMat;
				Matrix nextNextItemMat = _animTransforms[nextIndex].transforms[nextNextFrame][boneIndex];
				nextNextItemMat = _model->GetBoneByIndex(boneIndex)->parent->transform * nextNextItemMat;
				Matrix nextFinalMat = Matrix::Lerp(nextItemMat, nextNextItemMat, _tweenDesc->next.ratio);

				// 현재와 넥스트 트랜스폼 사이를 보간
				interpolatedTransform = Matrix::Lerp(interpolatedTransform, nextFinalMat, _tweenDesc->tweenRatio);
			}

			helm->GetTransform()->SetLocalMatrix(interpolatedTransform);
		}
	}
	//LeftShoulder
	{
		auto LeftShoulder = equipmentSlot->GetEquipmentItem(1);

		if (LeftShoulder != nullptr)
		{
			int currentIndex = _tweenDesc->current.animIndex;
			uint32 currentFrame = _tweenDesc->current.currentFrame;
			uint32 currentNextFrame = _tweenDesc->current.nextFrame;
			int nextIndex = _tweenDesc->next.animIndex;
			uint32 nextFrame = _tweenDesc->next.currentFrame;
			uint32 nextNextFrame = _tweenDesc->next.nextFrame;
			int boneIndex = _equipmentBoneIndexList.LeftShoulderIndex;

			Matrix currentItemMat = _animTransforms[currentIndex].transforms[currentFrame][boneIndex];
			currentItemMat = _model->GetBoneByIndex(boneIndex)->parent->transform * currentItemMat;
			Matrix currentNextItemMat = _animTransforms[currentIndex].transforms[currentNextFrame][boneIndex];
			currentNextItemMat = _model->GetBoneByIndex(boneIndex)->parent->transform * currentNextItemMat;

			Matrix interpolatedTransform = Matrix::Lerp(currentItemMat, currentNextItemMat, _tweenDesc->current.ratio);

			if (_nextAnim != nullptr)
			{
				Matrix nextItemMat = _animTransforms[nextIndex].transforms[nextFrame][boneIndex];
				nextItemMat = _model->GetBoneByIndex(boneIndex)->parent->transform * nextItemMat;
				Matrix nextNextItemMat = _animTransforms[nextIndex].transforms[nextNextFrame][boneIndex];
				nextNextItemMat = _model->GetBoneByIndex(boneIndex)->parent->transform * nextNextItemMat;
				Matrix nextFinalMat = Matrix::Lerp(nextItemMat, nextNextItemMat, _tweenDesc->next.ratio);

				// 현재와 넥스트 트랜스폼 사이를 보간
				interpolatedTransform = Matrix::Lerp(interpolatedTransform, nextFinalMat, _tweenDesc->tweenRatio);
			}

			LeftShoulder->GetTransform()->SetLocalMatrix(interpolatedTransform);
		}
	}
	//RightShoulder
	{
		auto RightShoulder = equipmentSlot->GetEquipmentItem(2);

		if (RightShoulder != nullptr)
		{
			int currentIndex = _tweenDesc->current.animIndex;
			uint32 currentFrame = _tweenDesc->current.currentFrame;
			uint32 currentNextFrame = _tweenDesc->current.nextFrame;
			int nextIndex = _tweenDesc->next.animIndex;
			uint32 nextFrame = _tweenDesc->next.currentFrame;
			uint32 nextNextFrame = _tweenDesc->next.nextFrame;
			int boneIndex = _equipmentBoneIndexList.RightShoulderIndex;

			Matrix currentItemMat = _animTransforms[currentIndex].transforms[currentFrame][boneIndex];
			currentItemMat = _model->GetBoneByIndex(boneIndex)->parent->transform * currentItemMat;
			Matrix currentNextItemMat = _animTransforms[currentIndex].transforms[currentNextFrame][boneIndex];
			currentNextItemMat = _model->GetBoneByIndex(boneIndex)->parent->transform * currentNextItemMat;

			Matrix interpolatedTransform = Matrix::Lerp(currentItemMat, currentNextItemMat, _tweenDesc->current.ratio);

			if (_nextAnim != nullptr)
			{
				Matrix nextItemMat = _animTransforms[nextIndex].transforms[nextFrame][boneIndex];
				nextItemMat = _model->GetBoneByIndex(boneIndex)->parent->transform * nextItemMat;
				Matrix nextNextItemMat = _animTransforms[nextIndex].transforms[nextNextFrame][boneIndex];
				nextNextItemMat = _model->GetBoneByIndex(boneIndex)->parent->transform * nextNextItemMat;
				Matrix nextFinalMat = Matrix::Lerp(nextItemMat, nextNextItemMat, _tweenDesc->next.ratio);

				// 현재와 넥스트 트랜스폼 사이를 보간
				interpolatedTransform = Matrix::Lerp(interpolatedTransform, nextFinalMat, _tweenDesc->tweenRatio);
			}

			RightShoulder->GetTransform()->SetLocalMatrix(interpolatedTransform);
		}
	}
	//Belt
	{
		auto Belt = equipmentSlot->GetEquipmentItem(3);

		if (Belt != nullptr)
		{
			int currentIndex = _tweenDesc->current.animIndex;
			uint32 currentFrame = _tweenDesc->current.currentFrame;
			uint32 currentNextFrame = _tweenDesc->current.nextFrame;
			int nextIndex = _tweenDesc->next.animIndex;
			uint32 nextFrame = _tweenDesc->next.currentFrame;
			uint32 nextNextFrame = _tweenDesc->next.nextFrame;
			int boneIndex = _equipmentBoneIndexList.BeltIndex;

			Matrix currentItemMat = _animTransforms[currentIndex].transforms[currentFrame][boneIndex];
			currentItemMat = _model->GetBoneByIndex(boneIndex)->parent->transform * currentItemMat;
			Matrix currentNextItemMat = _animTransforms[currentIndex].transforms[currentNextFrame][boneIndex];
			currentNextItemMat = _model->GetBoneByIndex(boneIndex)->parent->transform * currentNextItemMat;

			Matrix interpolatedTransform = Matrix::Lerp(currentItemMat, currentNextItemMat, _tweenDesc->current.ratio);

			if (_nextAnim != nullptr)
			{
				Matrix nextItemMat = _animTransforms[nextIndex].transforms[nextFrame][boneIndex];
				nextItemMat = _model->GetBoneByIndex(boneIndex)->parent->transform * nextItemMat;
				Matrix nextNextItemMat = _animTransforms[nextIndex].transforms[nextNextFrame][boneIndex];
				nextNextItemMat = _model->GetBoneByIndex(boneIndex)->parent->transform * nextNextItemMat;
				Matrix nextFinalMat = Matrix::Lerp(nextItemMat, nextNextItemMat, _tweenDesc->next.ratio);

				// 현재와 넥스트 트랜스폼 사이를 보간
				interpolatedTransform = Matrix::Lerp(interpolatedTransform, nextFinalMat, _tweenDesc->tweenRatio);
			}

			Belt->GetTransform()->SetLocalMatrix(interpolatedTransform);
		}
	}
	//Weapon
	{
		auto weapon = equipmentSlot->GetEquipmentItem(4);

		if (weapon != nullptr)
		{
			int currentIndex = _tweenDesc->current.animIndex;
			uint32 currentFrame = _tweenDesc->current.currentFrame;
			uint32 currentNextFrame = _tweenDesc->current.nextFrame;
			int nextIndex = _tweenDesc->next.animIndex;
			uint32 nextFrame = _tweenDesc->next.currentFrame;
			uint32 nextNextFrame = _tweenDesc->next.nextFrame;
			int boneIndex = _equipmentBoneIndexList.WeaponIndex;

			Matrix currentItemMat = _animTransforms[currentIndex].transforms[currentFrame][boneIndex];
			currentItemMat = _model->GetBoneByIndex(boneIndex)->parent->transform * currentItemMat;
			Matrix currentNextItemMat = _animTransforms[currentIndex].transforms[currentNextFrame][boneIndex];
			currentNextItemMat = _model->GetBoneByIndex(boneIndex)->parent->transform * currentNextItemMat;

			Matrix interpolatedTransform = Matrix::Lerp(currentItemMat, currentNextItemMat, _tweenDesc->current.ratio);

			if (_nextAnim != nullptr)
			{
				Matrix nextItemMat = _animTransforms[nextIndex].transforms[nextFrame][boneIndex];
				nextItemMat = _model->GetBoneByIndex(boneIndex)->parent->transform * nextItemMat;
				Matrix nextNextItemMat = _animTransforms[nextIndex].transforms[nextNextFrame][boneIndex];
				nextNextItemMat = _model->GetBoneByIndex(boneIndex)->parent->transform * nextNextItemMat;
				Matrix nextFinalMat = Matrix::Lerp(nextItemMat, nextNextItemMat, _tweenDesc->next.ratio);

				// 현재와 넥스트 트랜스폼 사이를 보간
				interpolatedTransform = Matrix::Lerp(interpolatedTransform, nextFinalMat, _tweenDesc->tweenRatio);
			}

			weapon->GetTransform()->SetLocalMatrix(interpolatedTransform);
		}
	}
	//Shield
	{
		auto Shield = equipmentSlot->GetEquipmentItem(5);

		if (Shield != nullptr)
		{
			int currentIndex = _tweenDesc->current.animIndex;
			uint32 currentFrame = _tweenDesc->current.currentFrame;
			uint32 currentNextFrame = _tweenDesc->current.nextFrame;
			int nextIndex = _tweenDesc->next.animIndex;
			uint32 nextFrame = _tweenDesc->next.currentFrame;
			uint32 nextNextFrame = _tweenDesc->next.nextFrame;
			int boneIndex = _equipmentBoneIndexList.ShieldIndex;

			Matrix currentItemMat = _animTransforms[currentIndex].transforms[currentFrame][boneIndex];
			currentItemMat = _model->GetBoneByIndex(boneIndex)->parent->transform * currentItemMat;
			Matrix currentNextItemMat = _animTransforms[currentIndex].transforms[currentNextFrame][boneIndex];
			currentNextItemMat = _model->GetBoneByIndex(boneIndex)->parent->transform * currentNextItemMat;

			Matrix interpolatedTransform = Matrix::Lerp(currentItemMat, currentNextItemMat, _tweenDesc->current.ratio);

			if (_nextAnim != nullptr)
			{
				Matrix nextItemMat = _animTransforms[nextIndex].transforms[nextFrame][boneIndex];
				nextItemMat = _model->GetBoneByIndex(boneIndex)->parent->transform * nextItemMat;
				Matrix nextNextItemMat = _animTransforms[nextIndex].transforms[nextNextFrame][boneIndex];
				nextNextItemMat = _model->GetBoneByIndex(boneIndex)->parent->transform * nextNextItemMat;
				Matrix nextFinalMat = Matrix::Lerp(nextItemMat, nextNextItemMat, _tweenDesc->next.ratio);

				// 현재와 넥스트 트랜스폼 사이를 보간
				interpolatedTransform = Matrix::Lerp(interpolatedTransform, nextFinalMat, _tweenDesc->tweenRatio);
			}

			Shield->GetTransform()->SetLocalMatrix(interpolatedTransform);
		}
	}
}

void ModelAnimator::Update()
{
}

void ModelAnimator::RenderInstancingShadow(shared_ptr<class InstancingBuffer>& buffer, ShadowViewDesc& desc)
{
	if (_model == nullptr)
		return;

	//Bone
	BoneDesc boneDesc;

	const uint32 boneCount = _model->GetBoneCount();
	for (uint32 i = 0; i < boneCount; i++)
	{
		shared_ptr<ModelBone> bone = _model->GetBoneByIndex(i);
		boneDesc.transforms[i] = bone->transform;
	}
	_shader->PushBoneData(boneDesc);
	_shader->PushGlobalData(desc.shadowView, desc.shadowProj);

	auto lightObj = MANAGER_SCENE()->GetCurrentScene()->GetLight();
	if (lightObj)
		_shader->PushLightData(lightObj->GetLight()->GetLightDesc());

	const auto& meshes = _model->GetMeshes();
	for (auto& mesh : meshes)
	{
		//Bone Index
		_shader->GetScalar("BoneIndex")->SetInt(mesh->boneIndex);

		mesh->vertexBuffer->PushData();
		mesh->indexBuffer->PushData();
		buffer->PushData();

		_shader->DrawIndexedInstanced(0, _pass, mesh->indexBuffer->GetCount(), buffer->GetCount());
	}
}
