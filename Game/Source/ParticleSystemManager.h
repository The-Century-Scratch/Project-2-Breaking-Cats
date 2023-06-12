#ifndef __PARTICLESYSTEMMANAGER_H__
#define __PARTICLESYSTEMMANAGER_H__

#include "Module.h"
#include "List.h"
#include "ParticleSystem.h"

#define PARTICLE_POOL_SIZE 10000

enum class Blueprint {
	CONSTANT_FIRE,
	FIRE,
	SMOKE,
	EXPLOSION,
	BULLET,
	SLASH,
	DASH,
	PORTAL1,
	PORTAL2,
	MAGICIMPACT,
	NONE
};

enum AlphasIDs {
	BASIC = 0,
	SMOKE_WHITE,
	SMOKE_SHADED,
	SLASHGATS,

	ALPHAS_AVAILABLES
};

class SDL_Texture;

// an object pool for particles and a manager for particle systems

class ParticleSystemManager : public Module
{
public:

	ParticleSystemManager(bool startEnabled);

	// Destructor
	virtual ~ParticleSystemManager();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();


	ParticleSystem* CreateParticleSystem(iPoint initialPosition, Blueprint blueprint, iPoint finalPosition);


private:
	void GiveParticlesToPS(ParticleSystem* particleSystem, int amount);

	void TakeParticlesFromPS(ParticleSystem* particleSystem);

public: 

private:
	List<ParticleSystem*> particleSystems;
	Particle particlePool[PARTICLE_POOL_SIZE];
	Particle* firstParticleAvailable;

	SDL_Texture* alphaTextures[AlphasIDs::ALPHAS_AVAILABLES];

};

#endif // __PARTICLESYSTEMMANAGER_H__