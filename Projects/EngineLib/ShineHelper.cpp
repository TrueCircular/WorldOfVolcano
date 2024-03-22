#include "pch.h"
#include "ShineHelper.h"
#include "Utils.h"
void ShineHelper::Update()
{

	for (int i = 0; i < instanceList.size(); ++i) {

		if (instanceList[i]->particleTransform) {
			instanceList[i]->particleTransform->UpdateTransform();
			auto particle1 = MANATER_PARTICLE()->GetParticleFromName(L"LineSpark");
			if (particle1) {
				{
					shared_ptr<Transform> particleTransform = make_shared<Transform>();
					Vec3 particlePos = instanceList[i]->particleTransform->GetPosition();
					particlePos += Vec3(15 + Utils::Randstep(-5, 5), 10 +Utils::Randstep(-2, 10) , 15 + Utils::Randstep(-5, 5));
					particleTransform->SetScale(Vec3(10, 10, 10));
					particleTransform->SetLocalPosition(particlePos);
					shared_ptr<ParticleInstance> particles;
					particles = make_shared<ParticleInstance>(1.6, particleTransform, nullptr, 50);
					particle1->AddParticle(particles);
				} 
				{
					shared_ptr<Transform> particleTransform = make_shared<Transform>();
					Vec3 particlePos = instanceList[i]->particleTransform->GetPosition();
					particlePos += Vec3(15 + Utils::Randstep(-5, 5), 10 + Utils::Randstep(-2, 10), -15 + Utils::Randstep(-5, 5));
					particleTransform->SetScale(Vec3(10, 10, 10));
					particleTransform->SetLocalPosition(particlePos);
					shared_ptr<ParticleInstance> particles;
					particles = make_shared<ParticleInstance>(1.6, particleTransform, nullptr, 50);
					particle1->AddParticle(particles);
				} 
				{
					shared_ptr<Transform> particleTransform = make_shared<Transform>();
					Vec3 particlePos = instanceList[i]->particleTransform->GetPosition();
					particlePos += Vec3(-15 + Utils::Randstep(-5, 5), 10 + Utils::Randstep(-2, 10), -15 + Utils::Randstep(-5, 5));
					particleTransform->SetScale(Vec3(10, 10, 10));
					particleTransform->SetLocalPosition(particlePos);
					shared_ptr<ParticleInstance> particles;
					particles = make_shared<ParticleInstance>(1.6, particleTransform, nullptr, 50);
					particle1->AddParticle(particles);
				}
				{
					shared_ptr<Transform> particleTransform = make_shared<Transform>();
					Vec3 particlePos = instanceList[i]->particleTransform->GetPosition();
					particlePos += Vec3(-15 + Utils::Randstep(-5, 5), 10 + Utils::Randstep(-2, 10), 15 + Utils::Randstep(-5, 5));
					particleTransform->SetScale(Vec3(10, 10, 10));
					particleTransform->SetLocalPosition(particlePos);
					shared_ptr<ParticleInstance> particles;
					particles = make_shared<ParticleInstance>(1.6, particleTransform, nullptr, 50);
					particle1->AddParticle(particles);
				}
				{
					shared_ptr<Transform> particleTransform = make_shared<Transform>();
					Vec3 particlePos = instanceList[i]->particleTransform->GetPosition();
					particlePos += Vec3(0 + Utils::Randstep(-5, 5), 10 + Utils::Randstep(-2, 10), -15 + Utils::Randstep(-5, 5));
					particleTransform->SetScale(Vec3(10, 10, 10));
					particleTransform->SetLocalPosition(particlePos);
					shared_ptr<ParticleInstance> particles;
					particles = make_shared<ParticleInstance>(1.6, particleTransform, nullptr, 50);
					particle1->AddParticle(particles);
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

void ShineHelper::OnDestroy(shared_ptr<ParticleInstance>& instance)
{
}

/*

				{
					shared_ptr<Transform> particleTransform = make_shared<Transform>();
					Vec3 particlePos = instanceList[i]->particleTransform->GetPosition();
					particlePos += Vec3(-3.55 *3+ Utils::Randstep(-1.0, 1.0), 27.3/3, 9.52 *3+ Utils::Randstep(-1.0, 1.0));
					particleTransform->SetScale(Vec3(10, 10, 10));
					particleTransform->SetLocalPosition(particlePos);
					shared_ptr<ParticleInstance> particles;
					particles = make_shared<ParticleInstance>(1, particleTransform, nullptr, 50);
					particle1->AddParticle(particles);
				}
				{
					shared_ptr<Transform> particleTransform = make_shared<Transform>();
					Vec3 particlePos = instanceList[i]->particleTransform->GetPosition();
					particlePos += Vec3(1.8*3 + Utils::Randstep(-1.0, 1.0), 26.1/3, -14.12 *3+ Utils::Randstep(-1.0, 1.0));
					particleTransform->SetScale(Vec3(10, 10, 10));
					particleTransform->SetLocalPosition(particlePos);
					shared_ptr<ParticleInstance> particles;
					particles = make_shared<ParticleInstance>(1, particleTransform, nullptr, 50);
					particle1->AddParticle(particles);
				}
				{
					shared_ptr<Transform> particleTransform = make_shared<Transform>();
					Vec3 particlePos = instanceList[i]->particleTransform->GetPosition();
					particlePos += Vec3(-2.05*3 + Utils::Randstep(-1.0, 1.0), 38.1/3, -9.34*3 + Utils::Randstep(-1.0, 1.0));
					particleTransform->SetScale(Vec3(10, 10, 10));
					particleTransform->SetLocalPosition(particlePos);
					shared_ptr<ParticleInstance> particles;
					particles = make_shared<ParticleInstance>(1, particleTransform, nullptr, 50);
					particle1->AddParticle(particles);
				}
				{
					shared_ptr<Transform> particleTransform = make_shared<Transform>();
					Vec3 particlePos = instanceList[i]->particleTransform->GetPosition();
					particlePos += Vec3(1.9*3 + Utils::Randstep(-1, 1), 15.1/3, -9.65*3 + Utils::Randstep(-1.0, 1.0));
					particleTransform->SetScale(Vec3(10, 10, 10));
					particleTransform->SetLocalPosition(particlePos);
					shared_ptr<ParticleInstance> particles;
					particles = make_shared<ParticleInstance>(1, particleTransform, nullptr, 50);
					particle1->AddParticle(particles);
				}
			}
*/