#include "Tide.h"

Tide::~Tide()
{
}

void Tide::Initialize()
{
	for (uint32_t i = 0; i < 1; i++)
	{
		Particle* particle = new Particle;
		particle->SetElements(1.0f, 1.0f, 1.0f, 6.0f,
			-7.0f, 7.0f, -6.0f, -6.0f, -2.0f, -2.0f,
			0.0f, 0.0f, 6.0f, 12.0f, 0.0f, 0.0f);
		particle->Initialize("Resource/clearbabble.png", ParticleCommon::GetInstance(), SRVManager::GetInstance(), Object3dCommon::GetInstance(), particle->GetElements());
		particles.push_back(particle);
	}

}

void Tide::Update()
{
	for (Particle* particle : particles)
	{
		particle->Update(false, particle->GetElements());
	}
}

void Tide::Draw()
{
	for (Particle* particle : particles)
	{
		particle->Draw();
	}
}
