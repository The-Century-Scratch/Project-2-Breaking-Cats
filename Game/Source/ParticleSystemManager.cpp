#include "ParticleSystemManager.h"

#include "App.h"

#include "Textures.h"

ParticleSystemManager::ParticleSystemManager(bool startEnabled) : Module(startEnabled)
{
	name.Create("psmanager");
}

ParticleSystemManager::~ParticleSystemManager()
{
}

bool ParticleSystemManager::Awake()
{
	return true;
}

bool ParticleSystemManager::Start()
{
	firstParticleAvailable = &particlePool[0];

	for (int i = 0; i < PARTICLE_POOL_SIZE - 1; ++i) {
		particlePool[i].SetNext(&particlePool[i + 1]);
	}

	particlePool[PARTICLE_POOL_SIZE - 1].SetNext(NULL);

	alphaTextures[0] = app->tex->Load("Assets/Textures/particles/particle.png");

	alphaTextures[1] = app->tex->Load("Assets/Textures/particles/smoke_white.png");

	alphaTextures[2] = app->tex->Load("Assets/Textures/particles/smoke_shaded.png");


	// adapt it to xml
	/*for (int i = 0; i < ALPHAS_AVAILABLES; ++i) {
		app->tex->Load();
	}*/


	return true;
}

bool ParticleSystemManager::PreUpdate()
{
	return true;
}

bool ParticleSystemManager::Update(float dt)
{
	for (ListItem<ParticleSystem*>* item = particleSystems.start; item != NULL; item = item->next) {
		bool isAlive = item->data->Update(dt);
		if (!isAlive) {
			ParticleSystem* PSToDelete = item->data;
			TakeParticlesFromPS(item->data);
			particleSystems.Del(item);
		}
	}

	return true;
}

bool ParticleSystemManager::PostUpdate()
{
	for (ListItem<ParticleSystem*>* item = particleSystems.start; item != NULL; item = item->next) {
		item->data->PostUpdate();
	}
	return true;
}

ParticleSystem* ParticleSystemManager::CreateParticleSystem(iPoint initialPosition, Blueprint blueprint)
{
	ParticleSystem* PS = new ParticleSystem();
	PS->initialPosition = initialPosition;
	PS->position = initialPosition;
	PS->objectivePosition = initialPosition;

	switch (blueprint)
	{
	case CONSTANT_FIRE:
		GiveParticlesToPS(PS, 150);
		PS->PSLifespan = 5;
		PS->SetTexture(alphaTextures[AlphasIDs::BASIC]);
		PS->spawnRate = 0.01f;
		PS->isConstant = true;
		PS->initialColor.Set(255, 255, 0, 255);
		PS->objectiveColor.Set(255, 0, 0, 0);
		PS->particleLifespan = 1.3f;
		PS->shootingAcceleration = fPoint{ 0.0f, -10.0f };
		PS->randomShootingVelocityRangeMin = iPoint{ -5, -10 };
		PS->randomShootingVelocityRangeMax = iPoint{ 5, 2 };
		PS->randomSpawnPositionRangeMin = iPoint{ 0, 0 };
		PS->randomSpawnPositionRangeMax = iPoint{ 0, 0 };
		PS->initialScale = 6.0f;
		PS->objectiveScale = 1.0f;
		break;
	case FIRE:
		GiveParticlesToPS(PS, 150);
		PS->PSLifespan = 5;
		PS->SetTexture(alphaTextures[AlphasIDs::BASIC]);
		PS->spawnRate = 0.01f;
		PS->isConstant = false;
		PS->initialColor.Set(255, 255, 0, 255);
		PS->objectiveColor.Set(255, 0, 0, 0);
		PS->particleLifespan = 1.3f;
		PS->shootingAcceleration = fPoint{ 0.0f, -10.0f };
		PS->randomShootingVelocityRangeMin = iPoint{ -5, -10 };
		PS->randomShootingVelocityRangeMax = iPoint{ 5, 2 };
		PS->randomSpawnPositionRangeMin = iPoint{ 0, 0 };
		PS->randomSpawnPositionRangeMax = iPoint{ 0, 0 };
		PS->initialScale = 6.0f;
		PS->objectiveScale = 1.0f;
		break;
	case SMOKE:
		GiveParticlesToPS(PS, 50);
		PS->SetTexture(alphaTextures[SMOKE_SHADED]);
		PS->spawnRate = 0.3f;
		PS->isConstant = true;
		PS->initialColor.Set(150, 150, 150, 255);
		PS->objectiveColor.Set(0, 0, 0, 0);
		PS->particleLifespan = 10;
		PS->shootingAcceleration = fPoint{ 0.0f, 0.5f };
		PS->randomSpawnPositionRangeMin = iPoint{ -20, 0 };
		PS->randomSpawnPositionRangeMax = iPoint{ 20, 0 };
		PS->randomShootingVelocityRangeMin = iPoint{ 2, 0 };
		PS->randomShootingVelocityRangeMax = iPoint{ 10, 0 };

		break;
	case EXPLOSION:
		GiveParticlesToPS(PS, 20);
		PS->PSLifespan = 0.1f;
		PS->SetTexture(alphaTextures[AlphasIDs::BASIC]);
		PS->spawnRate = 0.0f;
		PS->isConstant = false;
		PS->initialColor.Set(255, 255, 0, 255);
		PS->objectiveColor.Set(255, 0, 0, 0);
		PS->particleLifespan = 0.4f;
		PS->shootingAcceleration = fPoint{ 0.0f, 100.0f };
		PS->randomShootingVelocityRangeMin = iPoint{ -100, -300 };
		PS->randomShootingVelocityRangeMax = iPoint{ 300, 100 };
		PS->randomSpawnPositionRangeMin = iPoint{ 0, 0 };
		PS->randomSpawnPositionRangeMax = iPoint{ 0, 0 };
		PS->initialScale = 6.0f;
		PS->objectiveScale = 1.0f;
		break;
	case NONE:
		break;
	default:
		break;
	}

	particleSystems.Add(PS);

	return PS;
}

void ParticleSystemManager::GiveParticlesToPS(ParticleSystem* particleSystem, int amount)
{
	for (int i = 0; i < amount; ++i) {
		if (firstParticleAvailable != NULL) {
			Particle* newParticle = firstParticleAvailable;

			particleSystem->AssignParticle(newParticle);

			firstParticleAvailable = newParticle->GetNext();
		}
		else {
			// here you can LOG that there are not enough particles
			break;
		}
	}
}

void ParticleSystemManager::TakeParticlesFromPS(ParticleSystem* particleSystem)
{
	List<Particle*> particleList = particleSystem->GetParticleList();
	for (ListItem<Particle*>* item = particleList.start; item != NULL; item = item->next) {
		item->data->SetNext(firstParticleAvailable);
		firstParticleAvailable = item->data;
		particleList.Del(item);
	}
}

bool ParticleSystemManager::CleanUp()
{
	for (int i = 0; i < ALPHAS_AVAILABLES; ++i) {
		app->tex->Unload(alphaTextures[i]);
		alphaTextures[i] = nullptr;
	}

	return true;
}
