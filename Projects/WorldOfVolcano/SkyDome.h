#pragma once

struct SkyDomeDesc {

	std::wstring SkyDomeTexPath = { L"", };
	std::wstring SkyDomeBlendPath = { L"", };
	std::wstring shaderPath = L"";

};
class SkyDome :public GameObject
{
	SkyDomeDesc* _desc=nullptr;
	shared_ptr<Texture> skyImage;
	shared_ptr<Texture> skyBlend;
	shared_ptr<Shader> shader;
	shared_ptr<Mesh> mesh;
	ComPtr<ID3DX11EffectShaderResourceVariable> srvImageEff;
	ComPtr<ID3DX11EffectShaderResourceVariable> srvBlendEff;
public:
	SkyDome() {};
	~SkyDome() {};
public:
	void Set(SkyDomeDesc* desc) { _desc = desc; };
//	void SetMesh(shared_ptr<Mesh> _mesh) { mesh = _mesh; };
	virtual void Start() override;
	virtual void Update() override;
};
