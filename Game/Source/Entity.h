#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "Point.h"
#include "SString.h"
#include "Input.h"
#include "Render.h"


#define PIXELS_PER_METER 32.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.03125f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

enum class EntityType
{
	PLAYER,
	NPC,
	ITEM,//CHEST?
	MOVABLEOBJECT,
	TRIGGERABLEOBJECT,
	STATICOBJECT,
	COLLECTIBLEOBJECT,

	UNKNOWN
};

class Entity
{
public:
	Entity();
	Entity(EntityType type) : type(type), active(true) {}

	virtual bool Awake()
	{
		return true;
	}

	virtual bool Start()
	{
		return true;
	}

	virtual bool Update()
	{
		return true;
	}

	virtual bool PostUpdate()
	{
		return true;
	}

	virtual bool CleanUp()
	{
		return true;
	}

	virtual void OnCollision(Collider* c1, Collider* c2)
	{

	}

	virtual void EndCollision(Collider* c1, Collider* c2)
	{

	}

	const Collider* GetCollider() const
	{
		return eCollider;
	}

	virtual bool LoadState(pugi::xml_node&)
	{
		return true;
	}

	virtual bool SaveState(pugi::xml_node&)
	{
		return true;
	}

	void Entity::Enable()
	{
		if (!active)
		{
			active = true;
			Start();
		}
	}

	void Entity::Disable()
	{
		if (active)
		{
			active = false;
			CleanUp();
		}
	}

public:

	virtual void Draw() {}

	SString name;
	EntityType type;
	bool active = true;
	bool toDelete = false;
	pugi::xml_node parameters;

	Collider* eCollider;

	// Possible properties, it depends on how generic we
	// want our Entity class, maybe it's not renderable...
	iPoint position;
	fPoint mPosition;
	int w, h;
	bool renderable = true;
};

#endif // __ENTITY_H__