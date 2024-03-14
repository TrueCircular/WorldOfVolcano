#include "pch.h"
#include "TestScene.h"
#include "CameraMove.h"

void TestScene::Init()
{
	//리소스 매니저 초기화
	shared_ptr<Shader> _shader;
	MANAGER_RESOURCES()->Init();
	{
		_shader = make_shared<Shader>(L"Instancing.fx");
		MANAGER_RESOURCES()->AddResource(L"Default", _shader);
	}

	//랜더 매니저 초기화
//	MANAGER_RENDERER()->Init(_shader);
	MANAGER_SOUND()->Init();
	//light
	{
		auto light = make_shared<GameObject>();
		light->AddComponent(make_shared<Light>());
		LightDesc lightDesc;
		lightDesc.ambient = Vec4(0.4f);
		lightDesc.diffuse = Vec4(1.f);
		lightDesc.specular = Vec4(0.1f);
		lightDesc.direction = Vec3(0.0f, -1.0f, 0.f);
		//		lightDesc.direction = Vec3(0, 0.0f, 1.f);
		light->GetLight()->SetLightDesc(lightDesc);
		MANAGER_SCENE()->GetCurrentScene()->Add(light);
		//		MANAGER_RENDERER()->PushLightData(lightDesc);
	}

	//Camera
	{

		frustom = make_shared<FrustomCamera>();
		_camera = make_shared<GameObject>();
		_camera->Awake();
		_camera->GetTransform()->SetLocalPosition(Vec3(0.f, 0.f, 0));
		_camera->AddComponent(make_shared<Camera>());
//		_camera->GetCamera()->SetCameraType(CameraType::Debug);
		_camera->GetCamera()->Init(Vec3(0, 0, -100.f), CameraType::Debug, ProjectionType::Perspective, 100.f);
		_camera->GetCamera()->SetCameraToTargetOffset(Vec3(0, 0, 0));
		_camera->Start();
		_camera->SetName(L"Camera");
		_camera->AddComponent(make_shared<CameraMove>());
		_camera->AddComponent(frustom);
		MANAGER_SCENE()->GetCurrentScene()->Add(_camera);
	}
	effShader = make_shared<Shader>(L"storm.fx");
	effTex = make_shared<Texture>();
	effTex->Load(L"../../Resources/Texture/Effect/T_ky_noise17.PNG");
	world = make_shared<Transform>();
	world->SetLocalScale(Vec3(10, 10, 10));
	world->SetLocalPosition(Vec3(0, 0, 0));
	effShader2 = make_shared<Shader>(L"FirePillar.fx");
	pillarTex1 = make_shared<Texture>();
	pillarTex1->Load(L"../../Resources/Texture/Effect/T_ky_maskRGB5.PNG");
	pillarTex2= make_shared<Texture>();
	pillarTex2->Load(L"../../Resources/Texture/Effect/T_ky_flare3.PNG");
	world2 = make_shared<Transform>();
	world2->SetLocalScale(Vec3(10, 10, 10));
	world2->SetLocalPosition(Vec3(10, 0, 0));
	//HeightPlainInfo heightMapDesc;
	//heightMapDesc.heightFilename = L"HeightMapBase";
	//heightMapDesc.heightFilePath = wstring(RESOURCES_ADDR_TEXTURE) + L"test.bmp";
	//heightMapDesc.shaderFilePath = L"ShadowSplattingMapping.fx";
	////heightMapDesc.shaderFilePath = L"TerrainMapping.fx";
	//heightMapDesc.shaderFilename = L"HeightMapShaderBase";
	//heightMapDesc.textureFilename = L"HeightMapTextureBase";
	//heightMapDesc.textureFilePath = wstring(RESOURCES_ADDR_TEXTURE) + L"020.bmp";
	//heightMapDesc.meshKey = L"TerrainMeshBase";
	//heightMapDesc.distance = 1;
	//heightMapDesc.row = 253;
	//heightMapDesc.col = 253;
	//
	//_terrain = make_shared<Terrain>(heightMapDesc);
	//_terrain->Awake();
	//_terrain->AddComponent(make_shared<MeshRenderer>());
	//_terrain->Start();
	//quadTreeTerrain = make_shared<QuadTreeTerrain>();
	//quadTreeTerrain->Set(_terrain, 3);
	//quadTreeTerrain->Start();
}
void TestScene::Start()
{
	Scene::Start();
}

void TestScene::Update()
{
	Scene::Update();
	//quadTreeTerrain->Frame((*frustom->frustomBox.get()));
	//quadTreeTerrain->Update();

	{
	world->UpdateTransform();
	auto _materialBuffer = make_shared<ConstantBuffer<ColorDesc>>();
	_materialBuffer->CreateConstantBuffer();
	auto data = effShader->GetConstantBuffer("ColorBuffer");

	ColorDesc _desc;
	_desc.baseColor = Vec4(1, 0, 0, 1);
	_desc.subColor = Vec4(0.8, 0.2, 0.2, 1);
	_materialBuffer->CopyData(_desc);
	data->SetConstantBuffer(_materialBuffer->GetBuffer().Get());

	auto _fresnelBuffer = make_shared<ConstantBuffer<FresnelDesc>>();
	_fresnelBuffer->CreateConstantBuffer();
	auto fresneldata = effShader->GetConstantBuffer("FresnelBuffer");

	FresnelDesc _fresneldesc;
	_fresneldesc.eyePos = MANAGER_SCENE()->GetCurrentScene()->GetCamera()->GetCamera()->GetCameraPosition();
	_fresneldesc.eyeLook = MANAGER_SCENE()->GetCurrentScene()->GetCamera()->GetCamera()->GetCameraLookVector();
	_fresnelBuffer->CopyData(_fresneldesc);
	fresneldata->SetConstantBuffer(_fresnelBuffer->GetBuffer().Get());

	effShader->PushTransformData(TransformDesc{ world->GetWorldMatrix() });
	effShader->PushGlobalData(Camera::S_MatView, Camera::S_MatProjection);
	auto light = MANAGER_SCENE()->GetCurrentScene()->GetLight()->GetLight()->GetLightDesc();
	light.direction = _camera->GetTransform()->GetLocalPosition();
	light.direction.Normalize();
	effShader->PushLightData(light);

	auto texmap1 = effShader->GetSRV("HightLightMap");
	auto texmap2 = effShader->GetSRV("Ky_PrimNoiseMap");
	auto texmap3 = effShader->GetSRV("Ky_NoiseMap");
	currenttime += MANAGER_TIME()->GetDeltaTime();
	if (currenttime > maxtime)currenttime -= maxtime;
	auto times = effShader->GetScalar("time")->SetFloat(currenttime);
	texmap1->SetResource(effTex->GetTexture().Get());
	texmap2->SetResource(effTex->GetTexture().Get());
	texmap3->SetResource(effTex->GetTexture().Get());
	auto _stride = MANAGER_RESOURCES()->GetResource<Mesh>(L"Sphere")->GetVertexBuffer()->GetStride();
	auto _offset = MANAGER_RESOURCES()->GetResource<Mesh>(L"Sphere")->GetVertexBuffer()->GetOffset();

	DC()->IASetVertexBuffers(0, 1, MANAGER_RESOURCES()->GetResource<Mesh>(L"Sphere")->GetVertexBuffer()->GetBuffer().GetAddressOf(), &_stride, &_offset);
	DC()->IASetIndexBuffer(MANAGER_RESOURCES()->GetResource<Mesh>(L"Sphere")->GetIndexBuffer()->GetBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

	effShader->DrawIndexed(0, 0, MANAGER_RESOURCES()->GetResource<Mesh>(L"Sphere")->GetIndexBuffer()->GetCount(), 0, 0);
	}

	{
		world2->UpdateTransform();
		auto _materialBuffer = make_shared<ConstantBuffer<ColorDesc>>();
		_materialBuffer->CreateConstantBuffer();
		auto data = effShader2->GetConstantBuffer("ColorBuffer");

		ColorDesc _desc;
		_desc.baseColor = Vec4(1,0,0,1);
		_desc.subColor = Vec4(1,1,0, 1);
		_materialBuffer->CopyData(_desc);
		data->SetConstantBuffer(_materialBuffer->GetBuffer().Get());

		effShader2->PushTransformData(TransformDesc{ world2->GetWorldMatrix() });
		effShader2->PushGlobalData(Camera::S_MatView, Camera::S_MatProjection);
		auto light = MANAGER_SCENE()->GetCurrentScene()->GetLight()->GetLight()->GetLightDesc();
		light.direction = _camera->GetTransform()->GetLocalPosition();
		light.direction.Normalize();
		effShader2->PushLightData(light);

		auto texmap1 = effShader2->GetSRV("FireNoise");
		auto texmap2 = effShader2->GetSRV("FireMask");
		auto times = effShader2->GetScalar("time")->SetFloat(currenttime);
		texmap1->SetResource(pillarTex1->GetTexture().Get());
		texmap2->SetResource(pillarTex2->GetTexture().Get());
		auto _stride = MANAGER_RESOURCES()->GetResource<Mesh>(L"Quad")->GetVertexBuffer()->GetStride();
		auto _offset = MANAGER_RESOURCES()->GetResource<Mesh>(L"Quad")->GetVertexBuffer()->GetOffset();

		DC()->IASetVertexBuffers(0, 1, MANAGER_RESOURCES()->GetResource<Mesh>(L"Quad")->GetVertexBuffer()->GetBuffer().GetAddressOf(), &_stride, &_offset);
		DC()->IASetIndexBuffer(MANAGER_RESOURCES()->GetResource<Mesh>(L"Quad")->GetIndexBuffer()->GetBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

		effShader2->DrawIndexed(0, 0, MANAGER_RESOURCES()->GetResource<Mesh>(L"Quad")->GetIndexBuffer()->GetCount(), 0, 0);
	}
}

void TestScene::LateUpdate()
{
	Scene::LateUpdate();
}