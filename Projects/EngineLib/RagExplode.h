#pragma once
class RagExplode :public ParticleObj
{
private:
	shared_ptr<Mesh> particleMesh;
	ComPtr<ID3DX11EffectShaderResourceVariable> noiseSRV;
	ComPtr<ID3DX11EffectShaderResourceVariable> smokeSRV;
	shared_ptr<Texture> noiseTexture;
	shared_ptr<Texture> smokeTexture;

public:
	virtual void Update() override;
	virtual void LateUpdate() override;

	virtual void OnDestroy(shared_ptr<ParticleInstance>& instance) override;
public:
	RagExplode();
	~RagExplode();
};

