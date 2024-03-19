#pragma once
class MagicCircle :public ParticleObj
{
private:
	shared_ptr<Mesh> particleMesh;
	ComPtr<ID3DX11EffectShaderResourceVariable> circleSRV;
	ComPtr<ID3DX11EffectShaderResourceVariable> NoiseSRV;
	shared_ptr<Texture> circleTexture;

public:
	virtual void Update() override;
	virtual void LateUpdate() override;

	virtual void OnDestroy(ParticleInstance& instance) override;
public:
	MagicCircle();
	~MagicCircle();
};


