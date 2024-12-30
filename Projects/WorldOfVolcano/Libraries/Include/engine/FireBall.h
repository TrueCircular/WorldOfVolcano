#pragma once
class FireBall :public ParticleObj
{
private:
	shared_ptr<Model> particleModel;
	//shared_ptr<
	
	ComPtr<ID3DX11EffectShaderResourceVariable> noiseSRV;
	ComPtr<ID3DX11EffectShaderResourceVariable> maskSRV;
	shared_ptr<Texture> noiseTexture;
	shared_ptr<Texture> maskTexture;


public:
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void OnDestroy(shared_ptr<ParticleInstance>& instance) override;
	virtual void AddParticle(shared_ptr<ParticleInstance> data) override;
public:
	FireBall();
	~FireBall();
};

