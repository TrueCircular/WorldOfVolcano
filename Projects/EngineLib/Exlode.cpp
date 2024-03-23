#include "pch.h"
#include "Exlode.h"
#include "Utils.h"
void Exlode::Update()
{

	for (int i = 0; i < instanceList.size(); ++i) {

		if (instanceList[i]->particleTransform) {
			instanceList[i]->particleTransform->UpdateTransform();
			auto particle1 = MANATER_PARTICLE()->GetParticleFromName(L"Smoke1");
			auto particle2 = MANATER_PARTICLE()->GetParticleFromName(L"Smoke2");
			auto particle3 = MANATER_PARTICLE()->GetParticleFromName(L"Smoke3");
			if (particle1) {
				for (int i = 0; i < 2; ++i) {
					shared_ptr<Transform> particletargetTransform = make_shared<Transform>();
					shared_ptr<Transform> particleTransform = make_shared<Transform>();
					Vec3 particlePos=instanceList[i]->particleTransform->GetPosition();
					Vec3 velocity;
					velocity.x = Utils::Randstep(-1, 1);
					velocity.y = Utils::Randstep(-1, 1);
					particlePos.x += velocity.x* 5 ;
					particlePos.y += velocity.y* 5;
					particleTransform->SetLocalPosition(particlePos);
					particlePos.x *= 5;
					particlePos.y *= 5;
					particletargetTransform->SetLocalPosition(particlePos);
					shared_ptr<ParticleInstance> particles;
					particles = make_shared<ParticleInstance>(1.2,particleTransform, particletargetTransform,50);
					particle1->AddParticle(particles);
					
				}
			}
			if (particle2) {

				for (int i = 0; i < 3; ++i) {
					shared_ptr<Transform> particletargetTransform = make_shared<Transform>();
					shared_ptr<Transform> particleTransform = make_shared<Transform>();
					Vec3 particlePos = instanceList[i]->particleTransform->GetPosition();
					Vec3 velocity;
					velocity.x = Utils::Randstep(-1, 1);
					velocity.y = Utils::Randstep(-1, 1);
					particlePos.x += velocity.x * 5;
					particlePos.y += velocity.y * 5;
					particleTransform->SetLocalPosition(particlePos);
					particlePos.x *= 5;
					particlePos.y *= 5;
					particletargetTransform->SetLocalPosition(particlePos);
					shared_ptr<ParticleInstance> particles;
					particles = make_shared<ParticleInstance>(1.2, particleTransform, particletargetTransform, 50);
					particle2->AddParticle(particles);
				}
			}
			if (particle3) {

				for (int i = 0; i < 1; ++i) {
					shared_ptr<Transform> particleTransform = make_shared<Transform>();
					Vec3 particlePos = instanceList[i]->particleTransform->GetPosition();
					particleTransform->SetLocalPosition(particlePos);
					shared_ptr<ParticleInstance> particles;
					particles = make_shared<ParticleInstance>(1.4, particleTransform, nullptr, 0);
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

void Exlode::OnDestroy(shared_ptr<ParticleInstance>& instance)
{
}
