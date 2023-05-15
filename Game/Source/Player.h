#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Point.h"
#include "Animation.h"
#include "Colliders.h"
#include "SDL/include/SDL.h"
#include "List.h"

struct SDL_Texture;

enum class PlayerState
{
	IDLE,
	WUP,
	WLEFT,
	WRIGHT,
	WDOWN,
	DYING,
	UNKNOWN
};

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

	void AnimationState();

public:

	Animation* currentAnim = nullptr;

	Animation idleAnim;
	Animation walkUpAnim;
	Animation walkLeftAnim;
	Animation walkRightAnim;
	Animation walkDownAnim;
	Animation dieAnim;

	bool godMode = false;

	bool npcRange = false;
	bool cheastRange = false;
	PlayerState state;

private:

	// Declare player parameters
	SDL_Texture* texture;
	const char* texturePath;
	uint walkinggrassfx;
	uint walkingcityfx;
	int walkingTimer;

};

#endif // __PLAYER_H__