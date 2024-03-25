#include "pch.h"
#include "SkyDome.h"
void SkyDome::Start()
{

	if (_desc == nullptr) {
		return;
	}
	skyImage = MANAGER_RESOURCES()->LoadResource<Texture>(L"SkyDome1", _desc->SkyDomeTexPath.c_str());
	skyBlend = MANAGER_RESOURCES()->LoadResource<Texture>(L"SkyDome2", _desc->SkyDomeBlendPath.c_str());
	shader = make_shared<Shader>(_desc->shaderPath);

	GetOrAddTransform()->SetLocalPosition(Vec3(0, 0, 0));
	GetOrAddTransform()->SetLocalScale(Vec3(15000, 15000, 15000));
	mesh = MANAGER_RESOURCES()->GetResource<Mesh>(L"Sphere");
	srvImageEff = shader->GetSRV("ImageMap");
	srvBlendEff = shader->GetSRV("BlendMap");
}

void SkyDome::Update()
{
	GameObject::Update();

	//Global
	srvImageEff->SetResource(skyImage->GetTexture().Get());
	srvBlendEff->SetResource(skyBlend->GetTexture().Get());
	shader->PushTransformData(TransformDesc{ GetTransform()->GetWorldMatrix() });
	Matrix mat = Camera::S_MatView;
	mat._41 = 0;
	mat._42 = 0;
	mat._43 = 0;
	shader->PushGlobalData(mat, Camera::S_MatProjection);


	UINT _stride = mesh->GetVertexBuffer()->GetStride();
	UINT _offset = mesh->GetVertexBuffer()->GetOffset();

	DC()->IASetVertexBuffers(0, 1, mesh->GetVertexBuffer()->GetBuffer().GetAddressOf(), &_stride, &_offset);
	DC()->IASetIndexBuffer(mesh->GetIndexBuffer()->GetBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

	shader->DrawIndexed(0, 0, mesh->GetIndexBuffer()->GetCount(), 0, 0);
}
