#pragma once
class Clap :public ParticleObj
{
private:
	shared_ptr<Mesh> particleMesh;
	ComPtr<ID3DX11EffectShaderResourceVariable> quakeSRV;
	shared_ptr<Texture> quakeTexture;

public:
	virtual void Update() override;
	virtual void LateUpdate() override;

	virtual void OnDestroy(shared_ptr<ParticleInstance>& instance) override;
public:
	Clap();
	~Clap();
};

