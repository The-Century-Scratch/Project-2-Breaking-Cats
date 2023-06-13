#ifndef __QUESTMANAGER_H__
#define __QUESTMANAGER_H__

#include "Module.h"
#include "Quest.h"
#include "List.h"
#include "EASTL/list.h"
#include "Fonts.h"
#include "Item.h"
#include "Inventory.h"

//main quests
#define TUTORIAL 0
#define MOVEROCKS 1
#define EXPLORECITY 2
#define LABERYNTH 3
#define LABORATORY 4

#define ENDMISSIONS -1

//sidequests
#define COLLECT1 0
#define COLLECT2 1

//number of menus and number of sidequests
#define SIDEQUESTSCOUNTFROM0 1
#define MENUSCOUNT 2



class QuestManager : public Module
{
public:

	QuestManager(bool startEnabled);

	// Destructor
	virtual ~QuestManager();

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

	bool ActivateQuest(int id);

	bool ActivateSideQuest(int id);

	void DeleteAllMainQuests();

	void DeleteAllSideQuests();

public:

	eastl::list<Quest*> loadedQuests;
	eastl::list<Quest*> activeQuests;

	eastl::list<Quest*> loadedSideQuests;
	eastl::list<Quest*> activeSideQuests;


	SDL_Texture* QuestMenuBox;
	Quest* questActive;
	Quest* sidequestActive;
	int completeQuestFx;
	bool printQuestMenu;
	bool RocksQuest, quest3;
	bool changeDialogueIdAfterRocks, changeDialogueIdAfterCollecting;

	//count of collectible objects
	uint ObjectsCount;
	//change menus
	uint MenuID;
	uint SideQuestID;

	//items
	SDL_Texture* ItemText;

	Item* firePaw;
	Item* dragonSlayer;
	Item* grapplingHook;
	Item* bulletPenetration;
	Item* mysticalEnergy;
	Item* arcaneSpirit;

	bool GiveItem;

	Font* font;


};

#endif // __QUESTMANAGER_H__
