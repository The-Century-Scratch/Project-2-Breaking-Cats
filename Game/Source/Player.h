#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Point.h"
#include "Animation.h"
#include "Colliders.h"
#include "SDL/include/SDL.h"
#include "List.h"

struct SDL_Texture;

class Player : public Entity
{
public:

	Player();
	
	virtual ~Player();

	bool Awake();

	bool Start();

	bool Update();

	bool PostUpdate();

	bool CleanUp();

	void OnCollision(Collider* c1, Collider* c2) override;

	void EndCollision(Collider* c1, Collider* c2) override;

public:

	Animation* currentAnim = nullptr;

	Animation idleanim;

	bool godMode = false;

	bool npcRange = false;
	bool cheastRange = false;

private:

	// Declare player parameters
	SDL_Texture* texture;
	const char* texturePath;

};

#endif // __PLAYER_H__