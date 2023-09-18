#ifndef __SCENEGAMEPLAY_H__
#define __SCENEGAMEPLAY_H__

#include "Scene.h"
#include "Point.h"
#include "Player.h"
#include "NPC.h"
#include "MovableObject.h"
#include "TriggerableObject.h"
#include "StaticObject.h"
#include "CollectibleObject.h"
#include "List.h"
//#include "ParticlesManager.h"

//#include "Font.h"
#include "EASTL/list.h"

class Player;
class Enemy;
class SceneBattle;
class Map;
class Render;
class Item;

class SceneGameplay : public Scene
{
public:
	SceneGameplay();

	virtual ~SceneGameplay() {}

	bool Load() override;

	bool Update(float dt) override;

	void Draw() override;

	bool UnLoad() override;

	bool LoadState(pugi::xml_node&) override;

	bool SaveState(pugi::xml_node&) const override;

	void ChangeMap(MapType newMap) override;
private:
	void DrawDebugVariable();

	void LoadMovableObjects();
	void LoadTriggerableObjects();
	void LoadStaticObject();
	void LoadCollectibleObjects();
	void LoadNpc();
	void LoadItems(pugi::xml_node& n);

	void SetCamMovability();

	void UpdatingButtons(Input* input);

private:
	Player* currentPlayer;
	List<NPC*> npcs;
	List<MovableObject*> movableObjectList;
	List<TriggerableObject*> triggerableObjectList;
	List<StaticObject*> staticObjectList;
	List<CollectibleObject*> collectibleObjectList;

	SDL_Texture* itemText;
	SDL_Texture* guiTex;
	SDL_Texture* guiPad;

	List<Item*> items;

	Item* firePaw;
	Item* dragonSlayer;
	Item* grapplingHook;
	Item* bulletPenetration;
	Item* mysticalEnergy;
	Item* arcaneSpirit;

	iPoint tmpPosPlayer;

	bool addItems_ = 0;

	eastl::list<Enemy*> enemyList;
	Enemy* tmp;

	int lastUserInput = 0;

	//Music
	SString cityTheme;
	SString shopTheme;
	SString forestTheme;
};

#endif //__SCENEGAMEPLAY_H__