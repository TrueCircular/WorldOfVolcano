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
		_camera->GetTransform()->SetLocalPosition(Vec3(0.f, 0.f, -100.f));
		_camera->AddComponent(make_shared<Camera>());
		_camera->GetCamera()->SetCameraType(CameraType::Debug);
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
	world->SetLocalPosition(Vec3(0, 100, 0));

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
struct TimeDesc {
	float time;
};
struct ColorDesc {
	Vec4 baseColor;
	Vec4 subColor;
};
void TestScene::Update()
{
	Scene::Update();

	//quadTreeTerrain->Frame((*frustom->frustomBox.get()));
	//quadTreeTerrain->Update();


	world->UpdateTransform();
	auto _materialBuffer = make_shared<ConstantBuffer<ColorDesc>>();
	_materialBuffer->CreateConstantBuffer();
	auto data = effShader->GetConstantBuffer("ColorBuffer");

	ColorDesc _desc;
	_desc.baseColor = Vec4(1, 0, 0, 1);
	_desc.subColor = Vec4(0.8, 0.2, 0.2, 1);
	_materialBuffer->CopyData(_desc);
	data->SetConstantBuffer(_materialBuffer->GetBuffer().Get());

	//auto _timeBuffer = make_shared<ConstantBuffer<TimeDesc>>();
	//_timeBuffer->CreateConstantBuffer();
	//auto timedata = effShader->GetConstantBuffer("TimeBuffer");
	//
	//TimeDesc _timedesc;
	//_timedesc.time = MANAGER_TIME()->GetDeltaTime();
	//_timeBuffer->CopyData(_timedesc);
	//timedata->SetConstantBuffer(_timeBuffer->GetBuffer().Get());
	effShader->PushTransformData(TransformDesc{ world->GetWorldMatrix()});
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
	auto _stride = MANAGER_RESOURCES()->GetResource<Mesh>(L"Sphere") ->GetVertexBuffer()->GetStride();
	auto _offset = MANAGER_RESOURCES()->GetResource<Mesh>(L"Sphere") ->GetVertexBuffer()->GetOffset();

	DC()->IASetVertexBuffers(0, 1, MANAGER_RESOURCES()->GetResource<Mesh>(L"Sphere")->GetVertexBuffer()->GetBuffer().GetAddressOf(), &_stride, &_offset);
	DC()->IASetIndexBuffer(MANAGER_RESOURCES()->GetResource<Mesh>(L"Sphere")->GetIndexBuffer()->GetBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);

	effShader->DrawIndexed(0, 0, MANAGER_RESOURCES()->GetResource<Mesh>(L"Sphere")->GetIndexBuffer()->GetCount(), 0, 0);
}

void TestScene::LateUpdate()
{
	Scene::LateUpdate();
}
/*


cbuffer cbObjectChangesEveryFrame: register(b1)
{
	// Object
	matrix				g_matNormal;
	// Global
	float4				cb_vLightVector;
	//float g_MeshColor.w	ref_at_norm_incidence = 1.33f;
	float4				cb_vEyePos;
	float4				cb_vEyeDir;
};



float3 Reflect( float3 incident, float3 normal)
{
	float cosI = dot(normal, incident);
	return incident - 2 * cosI * normal;
}

float3 Refract( float3 incident,  float3 normal,  float etaRatio )
{
	float cosI = dot( incident, normal);
	float cosT2 = 1.0f - etaRatio * etaRatio *( 1.0f - cosI * cosI );
	float3 T = etaRatio * incident + ((etaRatio * cosI - sqrt( abs(cosT2))) * normal );
	return T;
}

float ComputeFresnel(float3 vReflect, float3 vNormal, float F0)
{
	float cosAngle = 1-saturate(dot(vReflect, vNormal));
	float result = pow( cosAngle, 5.0f);
	result = saturate(mad(result, 1-saturate(F0), F0));
	return result;
}
	float t = m_Timer.GetElapsedTime() * XM_PI;
	m_pMainCamera->Update(g_fSecPerFrame);
	m_matWorld = *m_pMainCamera->GetWorldMatrix();


	if (I_Input.KeyCheck(DIK_7)== KEY_UP)
	{
		m_dwPixelShader = 0;
	}
	if (I_Input.KeyCheck(DIK_8) == KEY_UP)
	{
		m_dwPixelShader = 1;
	}
	if (I_Input.KeyCheck(DIK_9) == KEY_UP)
	{
		m_dwPixelShader = 2;
	}

	m_mSphereWorld =  *m_pMainCamera->GetWorldMatrix();
	m_mSphereWorld._11 = 10.0f;
	m_mSphereWorld._22 = 10.0f;
	m_mSphereWorld._33 = 10.0f;

	TMatrix mTranslate, mRotation;
	D3DXMatrixTranslation( &mTranslate, 0.0f, 50.0f, 50.0f );
	D3DXMatrixRotationX( &mRotation, 0 );
	D3DXMatrixMultiply( &m_mLightWorld, &mTranslate, &mRotation );

	m_vLightPosition.x = m_mLightWorld._41;
	m_vLightPosition.y = m_mLightWorld._42;
	m_vLightPosition.z = m_mLightWorld._43;

	D3DXVec3Normalize( &m_vLightVector, &m_vLightPosition);
	m_vLightVector *= -1.0f;

	TMatrix matNormalTransform;
	D3DXMatrixInverse(&matNormalTransform, NULL, &m_mSphereWorld );
	D3DXMatrixTranspose( &matNormalTransform, &matNormalTransform );

	D3D11_MAPPED_SUBRESOURCE MappedFaceDest;
	if( SUCCEEDED( m_pImmediateContext->Map( (ID3D11Resource*)m_pConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedFaceDest ) ) )
	{
		CB_ENVIRONMENT* pConstData = (CB_ENVIRONMENT*)MappedFaceDest.pData;

		//pConstData->matWorld		= m_mSphereWorld;
		pConstData->matNormal		= matNormalTransform;
		pConstData->vLightVector	= TVector4(m_vLightVector.x,
										m_vLightVector.y,
										m_vLightVector.z, 1.33f);
		pConstData->vEyePos			= TVector4(m_pMainCamera->GetEyePt()->x,
													m_pMainCamera->GetEyePt()->y,
													m_pMainCamera->GetEyePt()->z, 100.0f	);
		pConstData->vEyeDir			= TVector4(m_pMainCamera->GetLookVector()->x,
													m_pMainCamera->GetLookVector()->y,
													m_pMainCamera->GetLookVector()->z, 100.0f) ;
		m_pImmediateContext->Unmap( m_pConstantBuffer.Get(), 0 );
	}


		if( m_SkyObj.Create(GetDevice(), L"../../data/shader/SkyObj.hlsl", L"../../data/sky/grassenvmap1024.dds" ) == false )
	{
		MessageBox( 0, _T("m_pSkyObj->Create"), _T("Fatal error"), MB_OK );
		return 0;
	}
	m_SkyObj.SetSO(GetDevice(), GetContext());
	//--------------------------------------------------------------------------------------
	// 구 오브젝트 생성
	//--------------------------------------------------------------------------------------
	if( m_SphereObj.Create(GetDevice(), L"EnvMap.hlsl", L"../../data/map/basecolor.jpg" ) == false )
	{
		MessageBox( 0, _T("m_pSphereObj->Create"), _T("Fatal error"), MB_OK );
		return 0;
	}
	m_pConstantBuffer.Attach(DX::CreateConstantBuffer(m_pd3dDevice, &m_cbEnvironment, 1, sizeof(CB_ENVIRONMENT),true));
	FRESNEL_PS.Attach(DX::LoadPixelShaderFile(m_pd3dDevice, L"EnvMap.hlsl", "FRESNEL_PS"));
	REFRACTION_PS.Attach(DX::LoadPixelShaderFile(m_pd3dDevice, L"EnvMap.hlsl", "REFRACTION_PS"));
	//--------------------------------------------------------------------------------------
	// 월드  행렬
	//--------------------------------------------------------------------------------------
	D3DXMatrixIdentity( &m_mSphereWorld );
	D3DXMatrixIdentity( &m_matSkyWorld );
	m_matSkyWorld._11 = 10.0f;
	m_matSkyWorld._22 = 10.0f;
	m_matSkyWorld._33 = 10.0f;
	//--------------------------------------------------------------------------------------
	// 카메라  행렬
	//--------------------------------------------------------------------------------------
	m_pMainCamera = make_shared<TModelViewCamera>();
	m_pMainCamera->SetViewMatrix(TVector3(0.0f, 100.0f, -100.0f), TVector3(0.0f, 0.0f, 0.0f));

	float fAspectRatio = m_iWindowWidth / (FLOAT)m_iWindowHeight;
	m_pMainCamera->SetProjMatrix(XM_PI / 4, fAspectRatio, 0.1f, 500.0f);
	m_pMainCamera->SetWindow(m_iWindowWidth, m_iWindowHeight);
	return true;
}


*/