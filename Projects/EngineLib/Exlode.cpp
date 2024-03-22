#include "pch.h"
#include "Exlode.h"

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
					shared_ptr<Transform> particleTransform = make_shared<Transform>();
					Vec3 particlePos=instanceList[i]->particleTransform->GetPosition();
					particleTransform->SetLocalPosition(particlePos);
					shared_ptr<ParticleInstance> particles;
					particles = make_shared<ParticleInstance>(1,particleTransform,nullptr,50);
					particle1->AddParticle(particles);
					
				}
			}
			if (particle2) {

				for (int i = 0; i < 3; ++i) {
					shared_ptr<Transform> particleTransform = make_shared<Transform>();
					Vec3 particlePos = instanceList[i]->particleTransform->GetPosition();
					particleTransform->SetLocalPosition(particlePos);
					shared_ptr<ParticleInstance> particles;
					particles = make_shared<ParticleInstance>(1, particleTransform, nullptr, 50);
					particle2->AddParticle(particles);
				}
			}
			if (particle3) {

				for (int i = 0; i < 1; ++i) {
					shared_ptr<Transform> particleTransform = make_shared<Transform>();
					Vec3 particlePos = instanceList[i]->particleTransform->GetPosition();
					particleTransform->SetLocalPosition(particlePos);
					shared_ptr<ParticleInstance> particles;
					particles = make_shared<ParticleInstance>(1, particleTransform, nullptr, 0);
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
