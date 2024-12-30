#pragma once
class Polar :public ParticleObj
{
private:
	shared_ptr<Model> particleModel;
	//shared_ptr<

	ComPtr<ID3DX11EffectShaderResourceVariable> maskSRV;
	shared_ptr<Texture> maskTexture;


public:
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void OnDestroy(shared_ptr<ParticleInstance>& instance) override;
public:
	Polar();
	~Polar();
};

