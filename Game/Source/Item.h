#ifndef __ITEM_H__
#define __ITEM_H__

#include "Entity.h"
#include "Player.h"
#include "Point.h"
#include "Animation.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

enum class ItemType
{
	NONE,
	FIRE_PAWS,
	DRAGON_SLAYER,
	GRAPPLING_HOOK,
	BULLET_PENETRATION,
	MYSTICAL_ENERGY,
	ARCANE_SPIRIT
};

class Item : public Entity
{
public:

	Item() {};
	Item(iPoint pos, SDL_Texture* itemText_, EntityType entityType = EntityType::ITEM) : Entity(entityType), texture(itemText_) 
	{
		position.x = pos.x;
		position.y = pos.y;
		equiped = false;
	}

	virtual ~Item() {};

	virtual void UseItem(Player* player) {};
	virtual void UseItem() {};

public:
	ItemType itemType;

	SDL_Texture* texture;
	SDL_Rect itemTextSection;
	const char* texturePath;

	bool equiped = 0;

	int itemid = 0;

	eastl::string mapName;
};

#endif // __ITEM_H__