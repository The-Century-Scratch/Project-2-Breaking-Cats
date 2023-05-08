#ifndef __COLLIDER_H__
#define __COLLIDER_H__

#include "List.h"
#include "SDL/include/SDL_Rect.h"

class Module;
class Entity;

struct Collider
{
	enum Type
	{
		NONE = -1,
		//WALL,
		//PLAYER,
		//ENEMY,
		//PLAYER_SHOT,
		//ENEMY_SHOT,
		//CAMLIMIT,
		//POWER_UP,

		WALL,
		PLAYER,
		ENEMY,
		NPC,
		NPCINTERACTION,
		CAMLIMIT,
		CHEAST,
		CHEASTINTERACTION,
		CHANGESCENE,

		MAX
	};

	//Methods
	Collider(SDL_Rect rectangle, Type type, Entity* listener = nullptr);

	void SetPos(int x, int y);

	void UpdatePos(int x, int y);

	bool Intersects(const SDL_Rect& r) const;

	//Variables
	SDL_Rect rect;
	bool pendingToDelete = false;
	Type type;
	Entity* listener = nullptr;

	List<Collider*> onCollision;
};


#endif // !__COLLIDER_H__
