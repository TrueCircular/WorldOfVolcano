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
	effShader = make_shared<Shader>(L"StormLegacy.fx");
	effTex = make_shared<Texture>();
	effTex->Load(L"../../Resources/Texture/Effect/T_ky_noise17.PNG");
	world = make_shared<Transform>();
	world->SetLocalScale(Vec3(10, 10, 10));
	world->SetLocalPosition(Vec3(0, 0, 0));
	effShader2 = make_shared<Shader>(L"FirePillarLegacy.fx");
	pillarTex1 = make_shared<Texture>();
	pillarTex1->Load(L"../../Resources/Texture/Effect/T_ky_maskRGB5.PNG");
	pillarTex2= make_shared<Texture>();
	pillarTex2->Load(L"../../Resources/Texture/Effect/T_ky_flare3.PNG");
	world2 = make_shared<Transform>();
	world2->SetLocalScale(Vec3(10, 10, 10));
	world2->SetLocalPosition(Vec3(10, 0, 0));

	effShader3 = make_shared<Shader>(L"MagicCircleLegacy.fx");
	circleTex1 = make_shared<Texture>();
	circleTex1->Load(L"../../Resources/Texture/Effect/T_ky_magicCircle020.PNG");
	//circleTex2 = make_shared<Texture>();
	//circleTex2->Load(L"../../Resources/Texture/Effect/T_ky_flare3.PNG");
	world3 = make_shared<Transform>();
	world3->SetLocalScale(Vec3(10, 10, 10));
	world3->SetLocalPosition(Vec3(-10, 0, 0));

	effShader4 = make_shared<Shader>(L"EarthCrackLegacy.fx");
	quakeTex = make_shared<Texture>();
	quakeTex->Load(L"../../Resources/Texture/Effect/T_Earth_Crack.PNG");
	world4 = make_shared<Transform>();
	world4->SetLocalScale(Vec3(10, 10, 10));
	world4->SetLocalPosition(Vec3(20, 0, 0));

	effShader5 = make_shared<Shader>(L"PolarLegacy.fx");
	polarTex = make_shared<Texture>();
	polarTex->Load(L"../../Resources/Texture/Effect/T_ky_decoLinesB.PNG");
	world5 = make_shared<Transform>();
	world5->SetLocalScale(Vec3(10, 10, 10));
	world5->SetLocalPosition(Vec3(0, 10, 0));

	effShader6 = make_shared<Shader>(L"SmokeLegacy.fx");
	smoke1Tex1 = make_shared<Texture>();
	smoke1Tex1->Load(L"../../Resources/Texture/Effect/T_ky_noise6.PNG");
	smoke1Tex2 = make_shared<Texture>();
	smoke1Tex2->Load(L"../../Resources/Texture/Effect/T_ky_smoke1.PNG");
	world6 = make_shared<Transform>();
	world6->SetLocalScale(Vec3(10, 10, 10));
	world6->SetLocalPosition(Vec3(-10, 10, 0));

	effShader7 = make_shared<Shader>(L"Smoke2Legacy.fx");
	smoke2Tex1 = make_shared<Texture>();
	smoke2Tex1->Load(L"../../Resources/Texture/Effect/T_ky_dist_noise.PNG");
	smoke2Tex2 = make_shared<Texture>();
	smoke2Tex2->Load(L"../../Resources/Texture/Effect/T_ky_smoke_kai.PNG");
	world7 = make_shared<Transform>();
	world7->SetLocalScale(Vec3(10, 10, 10));
	world7->SetLocalPosition(Vec3(-20, 10, 0));

	effShader8 = make_shared<Shader>(L"Smoke3Legacy.fx");
	smoke3Tex = make_shared<Texture>();
	smoke3Tex->Load(L"../../Resources/Texture/Effect/T_ky_circleDust.PNG");
	world8 = make_shared<Transform>();
	world8->SetLocalScale(Vec3(10, 10, 10));
	world8->SetLocalPosition(Vec3(-20, 0, 0));

	effShader9 = make_shared<Shader>(L"LineSparkLegacy.fx");
	lineSparkTex = make_shared<Texture>();
	lineSparkTex->Load(L"../../Resources/Texture/Effect/T_Sparks.PNG");
	world9= make_shared<Transform>();
	world9->SetLocalScale(Vec3(10, 10, 10));
	world9->SetLocalPosition(Vec3(10, 10, 0));


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
	_desc.baseColor = Vec4(1, 0.2, 0.2, 1);
	_desc.subColor = Vec4(0.8, 0.3, 0.3, 1);
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

	{
		world3->UpdateTransform();
		auto _materialBuffer = make_shared<ConstantBuffer<ColorDesc>>();
		_materialBuffer->CreateConstantBuffer();
		auto data = effShader3->GetConstantBuffer("ColorBuffer");

		ColorDesc _desc;
		_desc.baseColor = Vec4(1, 0.2, 0.2, 1);
		_desc.subColor = Vec4(1, 1, 0, 1);
		_materialBuffer->CopyData(_desc);
		data->SetConstantBuffer(_materialBuffer->GetBuffer().Get());

		effShader3->PushTransformData(TransformDesc{ world3->GetWorldMatrix() });
		effShader3->PushGlobalData(Camera::S_MatView, Camera::S_MatProjection);
		auto light = MANAGER_SCENE()->GetCurrentScene()->GetLight()->GetLight()->GetLightDesc();
		light.direction = _camera->GetTransform()->GetLocalPosition();
		light.direction.Normalize();
		effShader3->PushLightData(light);

		auto texmap1 = effShader3->GetSRV("NoiseImage");
		auto texmap2 = effShader3->GetSRV("SpellImage");
		auto times = effShader3->GetScalar("time")->SetFloat(currenttime);
		texmap1->SetResource(circleTex1->GetTexture().Get());
		texmap2->SetResource(circleTex1->GetTexture().Get());
		auto _stride = MANAGER_RESOURCES()->GetResource<Mesh>(L"Quad")->GetVertexBuffer()->GetStride();
		auto _offset = MANAGER_RESOURCES()->GetResource<Mesh>(L"Quad")->GetVertexBuffer()->GetOffset();

		DC()->IASetVertexBuffers(0, 1, MANAGER_RESOURCES()->GetResource<Mesh>(L"Quad")->GetVertexBuffer()->GetBuffer().GetAddressOf(), &_stride, &_offset);
		DC()->IASetIndexBuffer(MANAGER_RESOURCES()->GetResource<Mesh>(L"Quad")->GetIndexBuffer()->GetBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

		effShader3->DrawIndexed(0, 0, MANAGER_RESOURCES()->GetResource<Mesh>(L"Quad")->GetIndexBuffer()->GetCount(), 0, 0);
	}

	{
		world4->UpdateTransform();
		auto _materialBuffer = make_shared<ConstantBuffer<ColorDesc>>();
		_materialBuffer->CreateConstantBuffer();
		auto data = effShader4->GetConstantBuffer("ColorBuffer");

		ColorDesc _desc;
		_desc.baseColor = Vec4(1, 1, 0.2, 1);
		_desc.subColor = Vec4(1.0, 1.0, 0.0, 1);
		_materialBuffer->CopyData(_desc);
		data->SetConstantBuffer(_materialBuffer->GetBuffer().Get());

		effShader4->PushTransformData(TransformDesc{ world4->GetWorldMatrix() });
		effShader4->PushGlobalData(Camera::S_MatView, Camera::S_MatProjection);

		auto texmap1 = effShader4->GetSRV("EffectMap");
		auto times = effShader4->GetScalar("time")->SetFloat(currenttime);
		auto ctimes = effShader4->GetScalar("duration")->SetFloat(maxtime);
		texmap1->SetResource(quakeTex->GetTexture().Get());
		auto _stride = MANAGER_RESOURCES()->GetResource<Mesh>(L"Quad")->GetVertexBuffer()->GetStride();
		auto _offset = MANAGER_RESOURCES()->GetResource<Mesh>(L"Quad")->GetVertexBuffer()->GetOffset();

		DC()->IASetVertexBuffers(0, 1, MANAGER_RESOURCES()->GetResource<Mesh>(L"Quad")->GetVertexBuffer()->GetBuffer().GetAddressOf(), &_stride, &_offset);
		DC()->IASetIndexBuffer(MANAGER_RESOURCES()->GetResource<Mesh>(L"Quad")->GetIndexBuffer()->GetBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

		effShader4->DrawIndexed(0, 0, MANAGER_RESOURCES()->GetResource<Mesh>(L"Quad")->GetIndexBuffer()->GetCount(), 0, 0);
	}
	{
		world5->UpdateTransform();
		auto _materialBuffer = make_shared<ConstantBuffer<ColorDesc>>();
		_materialBuffer->CreateConstantBuffer();
		auto data = effShader5->GetConstantBuffer("ColorBuffer");

		ColorDesc _desc;
		_desc.baseColor = Vec4(1, 1, 1, 1);
		_desc.subColor = Vec4(1.0, 1.0, 0.0, 1);
		_materialBuffer->CopyData(_desc);
		data->SetConstantBuffer(_materialBuffer->GetBuffer().Get());

		effShader5->PushTransformData(TransformDesc{ world5->GetWorldMatrix() });
		effShader5->PushGlobalData(Camera::S_MatView, Camera::S_MatProjection);

		auto texmap1 = effShader5->GetSRV("NoiseMap");
		auto times = effShader5->GetScalar("time")->SetFloat(currenttime);
		texmap1->SetResource(polarTex->GetTexture().Get());
		auto _stride = MANAGER_RESOURCES()->GetResource<Mesh>(L"Quad")->GetVertexBuffer()->GetStride();
		auto _offset = MANAGER_RESOURCES()->GetResource<Mesh>(L"Quad")->GetVertexBuffer()->GetOffset();

		DC()->IASetVertexBuffers(0, 1, MANAGER_RESOURCES()->GetResource<Mesh>(L"Quad")->GetVertexBuffer()->GetBuffer().GetAddressOf(), &_stride, &_offset);
		DC()->IASetIndexBuffer(MANAGER_RESOURCES()->GetResource<Mesh>(L"Quad")->GetIndexBuffer()->GetBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

		effShader5->DrawIndexed(0, 0, MANAGER_RESOURCES()->GetResource<Mesh>(L"Quad")->GetIndexBuffer()->GetCount(), 0, 0);
	}
	{
		world6->UpdateTransform();
		auto _materialBuffer = make_shared<ConstantBuffer<ColorDesc>>();
		_materialBuffer->CreateConstantBuffer();
		auto data = effShader6->GetConstantBuffer("ColorBuffer");

		ColorDesc _desc;
		_desc.baseColor = Vec4(0, 0.2, 0.8, 1);
		_desc.subColor = Vec4(0.0, 0.0, 0.0, 1);
		_materialBuffer->CopyData(_desc);
		data->SetConstantBuffer(_materialBuffer->GetBuffer().Get());

		effShader6->PushTransformData(TransformDesc{ world6->GetWorldMatrix() });
		effShader6->PushGlobalData(Camera::S_MatView, Camera::S_MatProjection);

		auto texmap1 = effShader6->GetSRV("NoiseMap");
		auto texmap2 = effShader6->GetSRV("MaskMap");
		auto times = effShader6->GetScalar("time")->SetFloat(currenttime);
		texmap1->SetResource(smoke1Tex1->GetTexture().Get());
		texmap2->SetResource(smoke1Tex2->GetTexture().Get());
		auto _stride = MANAGER_RESOURCES()->GetResource<Mesh>(L"Quad")->GetVertexBuffer()->GetStride();
		auto _offset = MANAGER_RESOURCES()->GetResource<Mesh>(L"Quad")->GetVertexBuffer()->GetOffset();

		DC()->IASetVertexBuffers(0, 1, MANAGER_RESOURCES()->GetResource<Mesh>(L"Quad")->GetVertexBuffer()->GetBuffer().GetAddressOf(), &_stride, &_offset);
		DC()->IASetIndexBuffer(MANAGER_RESOURCES()->GetResource<Mesh>(L"Quad")->GetIndexBuffer()->GetBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

		effShader6->DrawIndexed(0, 0, MANAGER_RESOURCES()->GetResource<Mesh>(L"Quad")->GetIndexBuffer()->GetCount(), 0, 0);
	}

	{
		world7->UpdateTransform();
		auto _materialBuffer = make_shared<ConstantBuffer<ColorDesc>>();
		_materialBuffer->CreateConstantBuffer();
		auto data = effShader7->GetConstantBuffer("ColorBuffer");

		ColorDesc _desc;
		_desc.baseColor = Vec4(1, 1, 1, 1);
		_desc.subColor = Vec4(1.0, 1.0, 1, 1);
		_materialBuffer->CopyData(_desc);
		data->SetConstantBuffer(_materialBuffer->GetBuffer().Get());

		effShader7->PushTransformData(TransformDesc{ world7->GetWorldMatrix() });
		effShader7->PushGlobalData(Camera::S_MatView, Camera::S_MatProjection);

		auto texmap1 = effShader7->GetSRV("NoiseMap");
		auto texmap2 = effShader7->GetSRV("MaskMap");
		auto times = effShader7->GetScalar("time")->SetFloat(currenttime);
		texmap1->SetResource(smoke2Tex1->GetTexture().Get());
		texmap2->SetResource(smoke2Tex2->GetTexture().Get());
		auto _stride = MANAGER_RESOURCES()->GetResource<Mesh>(L"Quad")->GetVertexBuffer()->GetStride();
		auto _offset = MANAGER_RESOURCES()->GetResource<Mesh>(L"Quad")->GetVertexBuffer()->GetOffset();

		DC()->IASetVertexBuffers(0, 1, MANAGER_RESOURCES()->GetResource<Mesh>(L"Quad")->GetVertexBuffer()->GetBuffer().GetAddressOf(), &_stride, &_offset);
		DC()->IASetIndexBuffer(MANAGER_RESOURCES()->GetResource<Mesh>(L"Quad")->GetIndexBuffer()->GetBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

		effShader7->DrawIndexed(0, 0, MANAGER_RESOURCES()->GetResource<Mesh>(L"Quad")->GetIndexBuffer()->GetCount(), 0, 0);
	}
	{
		world8->UpdateTransform();
		auto _materialBuffer = make_shared<ConstantBuffer<ColorDesc>>();
		_materialBuffer->CreateConstantBuffer();
		auto data = effShader8->GetConstantBuffer("ColorBuffer");

		ColorDesc _desc;
		_desc.baseColor = Vec4(1, 1, 1, 1);
		_desc.subColor = Vec4(1.0, 1.0, 1, 1);
		_materialBuffer->CopyData(_desc);
		data->SetConstantBuffer(_materialBuffer->GetBuffer().Get());

		effShader8->PushTransformData(TransformDesc{ world8->GetWorldMatrix() });
		effShader8->PushGlobalData(Camera::S_MatView, Camera::S_MatProjection);

		auto texmap1 = effShader8->GetSRV("MaskMap");
		auto times = effShader8->GetScalar("time")->SetFloat(currenttime);
		texmap1->SetResource(smoke3Tex->GetTexture().Get());
		auto _stride = MANAGER_RESOURCES()->GetResource<Mesh>(L"Quad")->GetVertexBuffer()->GetStride();
		auto _offset = MANAGER_RESOURCES()->GetResource<Mesh>(L"Quad")->GetVertexBuffer()->GetOffset();

		DC()->IASetVertexBuffers(0, 1, MANAGER_RESOURCES()->GetResource<Mesh>(L"Quad")->GetVertexBuffer()->GetBuffer().GetAddressOf(), &_stride, &_offset);
		DC()->IASetIndexBuffer(MANAGER_RESOURCES()->GetResource<Mesh>(L"Quad")->GetIndexBuffer()->GetBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

		effShader8->DrawIndexed(0, 0, MANAGER_RESOURCES()->GetResource<Mesh>(L"Quad")->GetIndexBuffer()->GetCount(), 0, 0);
	}
	{
		world9->UpdateTransform();
		auto _materialBuffer = make_shared<ConstantBuffer<ColorDesc>>();
		_materialBuffer->CreateConstantBuffer();
		auto data = effShader9->GetConstantBuffer("ColorBuffer");

		ColorDesc _desc;
		_desc.baseColor = Vec4(1, 0.8, 0.4, 1);
		_desc.subColor = Vec4(1.0, 1.0, 1, 1);
		_materialBuffer->CopyData(_desc);
		data->SetConstantBuffer(_materialBuffer->GetBuffer().Get());

		effShader9->PushTransformData(TransformDesc{ world9->GetWorldMatrix() });
		effShader9->PushGlobalData(Camera::S_MatView, Camera::S_MatProjection);

		auto texmap1 = effShader9->GetSRV("EffectMap");
		auto times = effShader9->GetScalar("time")->SetFloat(currenttime);
		auto ctimes = effShader9->GetScalar("duration")->SetFloat(maxtime);
		texmap1->SetResource(lineSparkTex->GetTexture().Get());
		auto _stride = MANAGER_RESOURCES()->GetResource<Mesh>(L"Quad")->GetVertexBuffer()->GetStride();
		auto _offset = MANAGER_RESOURCES()->GetResource<Mesh>(L"Quad")->GetVertexBuffer()->GetOffset();

		DC()->IASetVertexBuffers(0, 1, MANAGER_RESOURCES()->GetResource<Mesh>(L"Quad")->GetVertexBuffer()->GetBuffer().GetAddressOf(), &_stride, &_offset);
		DC()->IASetIndexBuffer(MANAGER_RESOURCES()->GetResource<Mesh>(L"Quad")->GetIndexBuffer()->GetBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

		effShader9->DrawIndexed(0, 0, MANAGER_RESOURCES()->GetResource<Mesh>(L"Quad")->GetIndexBuffer()->GetCount(), 0, 0);
	}


}

void TestScene::LateUpdate()
{
	Scene::LateUpdate();
}