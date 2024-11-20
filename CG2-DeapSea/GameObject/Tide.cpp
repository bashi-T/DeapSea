#include "Tide.h"

Tide::~Tide()
{
}

void Tide::Initialize()
{
	for (uint32_t i = 0; i < 1; i++)
	{
		std::unique_ptr<Particle> particle = std::make_unique<Particle>();
		particle->SetElements(1.0f, 1.0f, 1.0f, 6.0f,
			-7.0f, 7.0f, -6.0f, -6.0f, -2.0f, -2.0f,
			0.0f, 0.0f, 6.0f, 12.0f, 0.0f, 0.0f);
		particle->Initialize("Resource/clearbabble.png", ParticleCommon::GetInstance(), SRVManager::GetInstance(), Object3dCommon::GetInstance(), particle->GetElements());
		particles.push_back(std::move(particle));
	}
	object3d = std::make_unique<Object3d>();
	object3d->Initialize(Object3dCommon::GetInstance(), SRVManager::GetInstance());
}

void Tide::Update()
{
	for (int i = 0; i < particles.size(); i++)
	{
		particles[i]->Update(false, particles[i]->GetElements());
	}
}

void Tide::Draw()
{
	for (int i = 0; i < particles.size(); i++)
	{
		particles[i]->Draw();
	}
}
