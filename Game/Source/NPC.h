#ifndef __NPC_H__
#define __NPC_H__

#include "Entity.h"
#include "Point.h"
#include "Animation.h"
#include "Colliders.h"
#include "DialogueManager.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;


enum class NPCTYPE
{
	SIGN,
	VILLAGE,
	GUARDIAN,
	CONTRABANDIST,
	SHOP,
	TABERN, //15 frames npcs
	BARMAN,
	FRAME5, //5 frames npcs
	VILLAGE2, //9 frames npcs
	WORKER,

	
};

class NPC : public Entity
{
public:

	NPC();
	virtual ~NPC();

	bool Awake();

	bool Start();

	bool Update();

	bool PostUpdate();

	bool CleanUp();

	void OnCollision(Collider* c1, Collider* c2) override;

	SDL_Rect cRect;

	Collider* boundaries;

	int npcid = 0;
	int dialogueid = 0;

	NPCTYPE npctype;

private:

	SDL_Texture* texture;
	const char* texturePath;


	Animation NPCIdle;
	Animation NPCIdleAction;
	int actionanimcounter = 0;
};

#endif // __NPC_H__