#ifndef __SCENE_H__
#define __SCENE_H__

#include "TransitionsManager.h"

#include "Log.h"
#include "PugiXml/src/pugixml.hpp"
#include "SString.h"

enum MapType
{
	NONE = 0,
	TUTORIAL,
	VILLAGE,
	MOVROCK,
	LEVERDOOR,
	CITY,
	LABYRINTH,
	LAB,
	SHOP,
	TAVERN,
	RESISTANCE
};

enum class SceneType
{
	LOGO,
	MENU,
	TITLE,
	GAMEPLAY,
	BATTLE,
	ENDING,
	CREDITS
};

enum class TransitionType;

class Scene
{
public:

	Scene() : active(false), transitionRequired(false), win(false), showColliders(false) {}

	// Destructor
	virtual ~Scene() {}

	// Called before render is available
	bool Awake() { return true; }

	// Called before the first frame
	virtual bool Load() { return true; }

	// Called each loop iteration
	virtual bool Update(float dt) { return true; }

	// Called before all Updates
	virtual void Draw() {}

	// Called before quitting
	virtual bool UnLoad() { return true; }

	virtual bool LoadState(pugi::xml_node&) { return true; }

	virtual bool SaveState(pugi::xml_node&) const { return true; }

	virtual void ChangeMap(MapType newMap) {}


	void TransitionToScene(SceneType scene, TransitionType type, bool w = false)
	{
		LOG("Changing Scene");
		transitionRequired = true;
		nextScene = scene;
		win = w;
		//transitionType = type;
		TransitionsManager::GetInstance()->SetType(type);
	}

public:
	SString name;
	bool active = 0;
	
	bool canMoveCam = 1;

	bool transitionRequired = 0;
	SceneType nextScene;
	bool isTown = 0;
	bool isDungeon = 0;
	bool showColliders = 0;

	int channel = 0;
	bool win = 0;

	MapType nextMap;
};

#endif // __SCENE_H__