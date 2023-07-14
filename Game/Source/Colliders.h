#ifndef __COLLIDER_H__
#define __COLLIDER_H__

#include "List.h"
#include "Scene.h"
#include "SDL/include/SDL_Rect.h"
#include <PugiXml/src/pugixml.hpp>
#include "Point.h"

class Module;
class Entity;

struct ColliderProperties
{
	// TELEPORT
	MapType destinationMap;
	iPoint destination;

	//
};

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
		MOVABLEOBJECT,
		TRIGGERABLEOBJECT,
		COLLECTIBLEOBJECT,

		MAX
	};

	//Methods
	Collider(SDL_Rect rectangle, Type type, Entity* listener = nullptr);

	void SetPos(int x, int y);

	void UpdatePos(int x, int y);

	bool Intersects(const SDL_Rect& r) const;

	void ParseMapProperties(pugi::xml_node const& node);

	//Variables
	SDL_Rect rect;
	bool pendingToDelete = false;
	Type type;
	Entity* listener = nullptr;

	bool mapCollider = false;

	ColliderProperties properties;

	List<Collider*> onCollision;
};


#endif // !__COLLIDER_H__
