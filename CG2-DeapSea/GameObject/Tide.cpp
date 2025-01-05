#include "Tide.h"

Tide::~Tide()
{
}

	void Tide::Initialize()
	{
		//object3d = std::make_unique<Object3d>();
		//object3d->Initialize(Object3dCommon::GetInstance(), SRVManager::GetInstance());
		tideTranslate = { 0.0f,2.0f,20.0f };
		collision =
		{
			.min{tideTranslate.x - 6.0f,tideTranslate.y - 2.0f,tideTranslate.z - 1.0f},
			.max{tideTranslate.x, tideTranslate.y + 1.0f,tideTranslate.z + 1.0f}
		};
		for (uint32_t i = 0; i < 1; i++)
		{
			std::unique_ptr<Particle> particle = std::make_unique<Particle>();
			particle->SetElements(1.0f, 1.0f, 1.0f, 4.0f,
				collision.min.x, collision.max.x, collision.min.y, collision.max.y, collision.min.z, collision.max.z,
				0.0f, tideVector.x * 16, 0.0f, tideVector.y, 0.0f, tideVector.z);
			particle->Initialize("Resource/clearbabble.png", particle->GetElements(),100);
			particle->SetScale({ 0.75f,0.75f,0.75f });
			particles.push_back(std::move(particle));
		}
}

	void Tide::Update()
	{
		collision =
		{
			.min{tideTranslate.x - 6.0f,tideTranslate.y - 2.0f,tideTranslate.z - 1.0f},
			.max{tideTranslate.x ,tideTranslate.y + 1.0f,tideTranslate.z + 1.0f}
		};
		for (int i = 0; i < particles.size(); i++)
		{
			particles[i]->SetElements(1.0f, 1.0f, 1.0f, 4.0f,
				collision.min.x, collision.max.x, collision.min.y, collision.max.y, collision.min.z, collision.max.z,
				0.0f, tideVector.x * 16, 0.0f, tideVector.y, 0.0f, tideVector.z);
			particles[i]->Update(true, particles[i]->GetElements());
		}
	}

	void Tide::Draw()
	{
		for (int i = 0; i < particles.size(); i++)
		{
			particles[i]->Draw();
		}
	}

