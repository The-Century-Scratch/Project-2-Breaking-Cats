#include "EntityManager.h"
#include "Player.h"
#include "NPC.h"
#include "MovableObject.h"
#include "TriggerableObject.h"
#include "StaticObject.h"
#include "App.h"
#include "Textures.h"
#include "SceneManager.h"

#include "Defs.h"
#include "Log.h"

EntityManager::EntityManager(bool active) : Module(active)
{
	name.Create("entitymanager");
}

// Destructor
EntityManager::~EntityManager()
{}

// Called before render is available
bool EntityManager::Awake(pugi::xml_node& config)
{
	return true;
}

bool EntityManager::Start() {

	LOG("Loading Entity Manager");
	bool ret = true;

	//Iterates over the entities and calls the Awake
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Awake();
	}

	//Iterates over the entities and calls Start
	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Start();
	}

	return ret;
}

// Called before quitting
bool EntityManager::CleanUp()
{
	bool ret = true;
	ListItem<Entity*>* item;
	item = entities.end;

	while (item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	entities.Clear();

	return ret;
}

Entity* EntityManager::CreateEntity(EntityType type, bool solved)
{
	Entity* entity = nullptr; 

	// Instantiate entity according to the type and add the new entoty it to the list of Entities

	switch (type)
	{

	case EntityType::PLAYER:
		entity = new Player();
		break;

	case EntityType::NPC:
		entity = new NPC();
		break;

	case EntityType::MOVABLEOBJECT:
		entity = new MovableObject(solved);
		break;

	case EntityType::TRIGGERABLEOBJECT:
		entity = new TriggerableObject(solved);
		break;

	case EntityType::STATICOBJECT:
		entity = new StaticObject();
		break;
	default: break;
	}

	// Created entities are added to the list
	AddEntity(entity);

	return entity;
}

void EntityManager::DestroyEntity(Entity* entity)
{
	ListItem<Entity*>* item;

	for (item = entities.start; item != NULL; item = item->next)
	{
		if (item->data == entity) entities.Del(item);
	}
}

void EntityManager::AddEntity(Entity* entity)
{
	if ( entity != nullptr) entities.Add(entity);
}

bool EntityManager::Update(float dt)
{
	//if (app->pauseMenus->isPaused()) { return true; } //This is to use in the case a "game pause" is implemented for skiping entities update

	bool ret = true;

	ListItem<Entity*>* entityItem = entities.start;
	while (entityItem != NULL && ret)
	{
		if (entityItem->data->toDelete)
		{
			entityItem->data->CleanUp();
			DestroyEntity(entityItem->data);
		}
		entityItem = entityItem->next;
	}


	if (app->sceneManager->Pause)
	{
		return ret;
	}

	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->Update();
	}

	return ret;
}

bool EntityManager::PostUpdate()
{
	bool ret = true;
	ListItem<Entity*>* item;
	Entity* pEntity = NULL;

	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		pEntity = item->data;

		if (pEntity->active == false) continue;
		ret = item->data->PostUpdate();
	}

	return ret;
}