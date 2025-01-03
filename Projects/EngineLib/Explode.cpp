#include "pch.h"
#include "Explode.h"
#include "Utils.h"
void Explode::Update()
{

	for (int i = 0; i < instanceList.size(); ++i) {

		if (instanceList[i]->particleTransform) {
			instanceList[i]->particleTransform->UpdateTransform();
			auto particle1 = MANATER_PARTICLE()->GetParticleFromName(L"Smoke1");
			auto particle2 = MANATER_PARTICLE()->GetParticleFromName(L"Smoke2");
			auto particle3 = MANATER_PARTICLE()->GetParticleFromName(L"Smoke3");
			if (particle1) {
				for (int j = 0; j < 4; ++j) {
					shared_ptr<Transform> particletargetTransform = make_shared<Transform>();
					shared_ptr<Transform> particleTransform = make_shared<Transform>();
					Vec3 particlePos=instanceList[i]->particleTransform->GetPosition();
					Vec3 particleScale = instanceList[i]->particleTransform->GetScale();
					Vec3 velocity;
					velocity.x = Utils::Randstep(-1, 1);
					velocity.y = Utils::Randstep(-1, 1);
					velocity.z = Utils::Randstep(-1, 1);
					particleTransform->SetLocalPosition(particlePos);
					particleTransform->SetLocalScale(Vec3(6, 6, 6));
					if(velocity.x<0){
						particlePos.x *= -1;
					}
					particlePos.x *= 100;
					if (velocity.y < 0) {
						particlePos.y *= -1;
					}
					particlePos.y *= 100;
					if (velocity.z < 0) {
						particlePos.z *= -1;
					}
					particlePos.z *= 100;

					particletargetTransform->SetLocalPosition(particlePos);
					particletargetTransform->SetLocalScale(particleScale);
					shared_ptr<ParticleInstance> particles;
					particles = make_shared<ParticleInstance>(0.3,particleTransform, particletargetTransform,10);
					particle1->AddParticle(particles);
					
				}
			}
			if (particle2) {

				for (int j = 0; j < 5; ++j) {
					shared_ptr<Transform> particletargetTransform = make_shared<Transform>();
					shared_ptr<Transform> particleTransform = make_shared<Transform>();
					Vec3 particlePos = instanceList[i]->particleTransform->GetPosition();
					Vec3 particleScale = instanceList[i]->particleTransform->GetScale();
					Vec3 velocity;
					velocity.x = Utils::Randstep(-1, 1);
					velocity.y = Utils::Randstep(-1, 1);
					velocity.z = Utils::Randstep(-1, 1);

					particlePos.x += velocity.x * 5;
					particlePos.y += velocity.y * 5;
					particlePos.z += velocity.z * 5;
					particleTransform->SetLocalPosition(particlePos);
					particleTransform->SetLocalScale(Vec3(3,3,3));
					if (velocity.x < 0) {
						particlePos.x *= -1;
					}
					particlePos.x *= 100;

					if (velocity.y < 0) {
						particlePos.y *= -1;
					}

					particlePos.y *= 100;
					if (velocity.z < 0) {
						particlePos.z *= -1;
					}

					particlePos.z *= 100;
					particletargetTransform->SetLocalPosition(particlePos);
					particletargetTransform->SetLocalScale(particleScale);
					shared_ptr<ParticleInstance> particles;
					particles = make_shared<ParticleInstance>(0.4, particleTransform, particletargetTransform, 5);
					particle2->AddParticle(particles);
				}
			}
			if (particle3) {

				for (int j = 0; j < 1; ++j) {
					shared_ptr<Transform> particleTransform = make_shared<Transform>();
					Vec3 particlePos = instanceList[i]->particleTransform->GetPosition();
					Vec3 particleScale = instanceList[i]->particleTransform->GetScale();
					particleTransform->SetLocalPosition(particlePos);
					particleTransform->SetLocalScale(Vec3(3,3,3));
					shared_ptr<ParticleInstance> particles;
					particles = make_shared<ParticleInstance>(0.5, particleTransform, nullptr, 0);
					particle3->AddParticle(particles);
				}
			}
		}


		auto iter = instanceList.begin() + i;
		instanceList.erase(iter);
		instanceCounter--;
		--i;
		continue;
	}
}

void Explode::OnDestroy(shared_ptr<ParticleInstance>& instance)
{
}
