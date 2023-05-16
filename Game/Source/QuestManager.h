#ifndef __QUESTMANAGER_H__
#define __QUESTMANAGER_H__

#include "Module.h"
#include "Quest.h"
#include "List.h"
#include "EASTL/list.h"
#include "Fonts.h"

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

	void DeleteAllQuests();

public:

	eastl::list<Quest*> loadedQuests;
	eastl::list<Quest*> activeQuests;
	eastl::list<Quest*> finishedQuests;

	SDL_Texture* QuestMenuBox;
	Quest* questFinished;
	Quest* questActive;
	int completeQuestFx;
	bool printQuestMenu;
	bool currentQuestComplete;
	bool quest1, quest2, quest3;

	Font* font;


};

#endif // __QUESTMANAGER_H__
