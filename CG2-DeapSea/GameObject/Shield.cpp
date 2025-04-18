#include "Shield.h"

Shield::~Shield()
{
}

void Shield::Initialize()
{
    modelManager_ = ModelManager::GetInstance();
    modelManager_->LoadModel(shieldModel, shieldSkin, true);
    Model* model = modelManager_->FindModel(shieldModel);
    Model::ModelData* modelData = model->GetModelData();
    for (Model::VertexData& vertex : modelData->vertices)
	{
	    vertex.normal.x = vertex.position.x;
	    vertex.normal.y = vertex.position.y;
	    vertex.normal.z = vertex.position.z;
    }
    model->Memcpy();
    isDead = true;
    collision.center = {0.0f, 0.0f, 0.0f};
    collision.radius = 0.0f;

	for (uint32_t i = 0; i < 1; i++)
	{
		particle_ = std::make_unique<Particle>();
		particle_->SetElements(1.0f, 1.0f, 1.0f, 4.0f,
			collision.center.x - collision.radius, collision.center.x + collision.radius,
			collision.center.y - collision.radius, collision.center.y + collision.radius,
		    collision.center.z - collision.radius, collision.center.z + collision.radius,
			0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
		particle_->Initialize("Resource/clearbabble.png", particle_->GetElements(), 30);
		particle_->SetScale({0.75f, 0.75f, 0.75f});
		particle_->SetDistance(0.0f, 0.0f, 0.0f);
	}
}

void Shield::Update(Vector3 playerPos,Vector3 whalePos) 
{
	collision.center = {(playerPos.x + whalePos.x) / 2, (playerPos.y + whalePos.y) / 2, (playerPos.z + whalePos.z) / 2};

   if (isDead == false)
   {
		if (collision.radius <= Length(Subtract(playerPos, collision.center)) + 4.0f)
		{
		   collision.radius += 0.5f;
			particle_->SetDistance(particle_->GetDistance().distancex + particle_->GetParticles()->velocity.x, 0.0f, particle_->GetDistance().distancez + particle_->GetParticles()->velocity.z);
		   if (particle_->GetDistance().distancex >= collision.radius)
		   {
			   particle_->SetDistance(collision.radius, 0.0f, collision.radius);
		   }
		}
		else if (collision.radius >= Length(Subtract(playerPos, collision.center)) + 4.0f)
		{
		   collision.radius -= 0.2f;
		}
	   particle_->SetElements(1.0f, 1.0f, 0.1f, 2.0f, 
		   collision.center.x - collision.radius, collision.center.x + collision.radius,
		   collision.center.y - collision.radius, collision.center.y + collision.radius,
		   collision.center.z - collision.radius, collision.center.z + collision.radius,
		   0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
		particle_->RandomInitialize(particle_->GetElements());

	   particle_->Update(true, particle_->GetElements());
	   particle_->SetRotate({0.0f, rotateSpeed++, 0.0f});
	   particle_->Revolution({0.0f, particle_->GetDistance().distancey, -particle_->GetDistance().distancez}, collision.center);
   }
   else if (collision.radius > 0.0f)
   {
	   collision.radius -= 0.2f;

	   particle_->Update(false, particle_->GetElements());
	   particle_->SetRotate({0.0f, rotateSpeed++, 0.0f});
	   particle_->Revolution({0.0f, particle_->GetDistance().distancey, -particle_->GetDistance().distancez}, collision.center);
   }
}

void Shield::Draw()
{
	particle_->Draw();
}