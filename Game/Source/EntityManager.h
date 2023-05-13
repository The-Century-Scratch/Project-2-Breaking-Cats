#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include "Module.h"
#include "Entity.h"
#include "List.h"
#include "List.h"
#include "NPC.h"

#define MAX_NPC 5

class EntityManager : public Module
{
public:

	EntityManager(bool startEnabled);

	// Destructor
	virtual ~EntityManager();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called after Awake
	bool Start();

	// Called every frame
	bool Update(float dt);

	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Additional methods
	Entity* CreateEntity(EntityType type, bool solved = false);

	void DestroyEntity(Entity* entity);

	void AddEntity(Entity* entity);

public:

	List<Entity*> entities;

};

#endif // __ENTITYMANAGER_H__
