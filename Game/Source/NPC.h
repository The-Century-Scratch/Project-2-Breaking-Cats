#ifndef __NPC_H__
#define __NPC_H__

#include "Entity.h"
#include "Point.h"
#include "Animation.h"
#include "Colliders.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

enum class NPCTYPE
{
	NORMAL,
	QUEST,
	SHOP,
};

class NPC : public Entity
{
public:

	NPC();
	virtual ~NPC();

	bool Awake();

	bool Start();

	bool Update();

	bool CleanUp();

	void OnCollision(Collider* c1, Collider* c2) override;

	SDL_Rect cRect;

	Collider* boundaries;

	int npcid;

	NPCTYPE npctype;

private:

	SDL_Texture* texture;
	const char* texturePath;

	Animation NPCAnim;
};

#endif // __NPC_H__