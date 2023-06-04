#include "QuestManager.h"
#include "App.h"
#include "Textures.h"
#include "MainQuest.h"
#include "SideQuest.h"
#include"AssetsManager.h"
#include "Audio.h"
#include "SceneManager.h"
#include "FirePaws.h"

#include "Defs.h"
#include "Log.h"

QuestManager::QuestManager(bool active) : Module(active)
{
	name.Create("questmanager");
}

// Destructor
QuestManager::~QuestManager()
{}

// Called before render is available
bool QuestManager::Awake(pugi::xml_node& config)
{
	LOG("Loading Quest Manager");
	bool ret = true;

	/*for (pugi::xml_node node = config.child("activequests").child("quest"); node; node = node.next_sibling("quest"))
	{
		ListItem<Quest*>* qitem = quests.start;
		while (qitem != nullptr)
		{
			Quest* item = qitem->data;
			if (item->id == node.attribute("id").as_int()) {
				activeQuests.Add(item);
				break;
			}

			qitem = qitem->next;
		}
	}*/

	return ret;
}

bool QuestManager::Start() {

	LOG("Starting Quest Manager");
	bool ret = true;



	pugi::xml_document questFile;
	int size = app->assetsManager->MakeLoad("Xml/quests.xml");
	pugi::xml_parse_result result = questFile.load_buffer(app->assetsManager->GetLastBuffer(), size);
	app->assetsManager->DeleteBuffer();
	pugi::xml_node node = questFile.child("quests").child("quest");
	for (; node; node = node.next_sibling("quest"))
	{
		Quest* quest;
		switch ((QuestType)node.attribute("type").as_int())
		{
		case QuestType::MAINQUEST:
			quest = new MainQuest(node);
			loadedQuests.push_back(quest);
			break;
		case QuestType::SIDEQUEST:
			quest = new SideQuest(node);
			loadedSideQuests.push_back(quest);
			break;
		default:
			break;
		}		
		
	}

	completeQuestFx = app->audio->LoadFx("Assets/Audio/Fx/CompleteQuestPlaceholder .wav");
	QuestMenuBox = app->tex->Load("Assets/Textures/GUI/menuBoxPlaceholder.png");
	questActive = nullptr;
	sidequestActive = nullptr;
	font = new Font(app, "Fonts/prova.xml");
	RocksQuest = false;
	quest2 = false;
	quest3 = false;
	printQuestMenu = false;

	ObjectsCount = 0;

	ItemText = app->tex->Load("Assets/Textures/Items.png");

	firePaw = new FirePaws(iPoint(384,48), ItemText);
	firePaw->Start();

	ActivateQuest(TUTORIAL);
	return ret;
}

bool QuestManager::Update(float dt)
{
	bool ret = true;

	if (app->sceneManager->puzzle2solved && RocksQuest == false) {
		ActivateQuest(EXPLORECITY);
		ActivateSideQuest(COLLECT1);
		RocksQuest = true;
	}

	if (app->sceneManager->puzzle3solved &&quest3 == false) {
		ActivateQuest(ENDMISSIONS);
		quest3 = true;
		firePaw->equiped = true;
		app->inventory->AddItem(firePaw);
		app->audio->PlayFx(app->hud->getitemfx);
	}

	if (app->input->GetKey(SDL_SCANCODE_O) == KEY_DOWN) 
	{
		printQuestMenu = !printQuestMenu;
	}
	
	return ret;
}

bool QuestManager::PostUpdate() {
	if(printQuestMenu) {
		SDL_Rect sect{ 0,0,256,256 };
		app->render->DrawTexture(QuestMenuBox, 0, 0, &sect, false);
		if (questActive == nullptr) {
			app->render->DrawText(font, "No active quests", 50, 50, 48, 5, { 255,255,255,255 }, 528);
		}
		else {
			questActive->Draw(font);
		}

		sect = { 0,0,192,256 };
		app->render->DrawTexture(QuestMenuBox, 256, 0, &sect, false);
		if (sidequestActive == nullptr) {
			app->render->DrawText(font, "No active sidequests", 830, 0, 48, 5, { 255,255,255,255 }, 528);
		}
		else {
			sidequestActive->Draw(font);
		}
		
	}
	return true;
}

bool QuestManager::ActivateQuest(int id)
{
	if (id != -1)
	{
		eastl::list<Quest*>::iterator it = loadedQuests.begin();
		eastl::list<Quest*>::iterator itEnd = loadedQuests.end();
		for (; it != itEnd; ++it)
		{
			if ((*it)->id == id)
			{
				loadedQuests.erase(it);
				questActive = *it;
				break;
			}
		}
	}
	else {
		DeleteAllMainQuests();
	}

	return true;
}

bool QuestManager::ActivateSideQuest(int id)
{
	if (id != -1)
	{
		eastl::list<Quest*>::iterator it = loadedSideQuests.begin();
		eastl::list<Quest*>::iterator itEnd = loadedSideQuests.end();
		for (; it != itEnd; ++it)
		{
			if ((*it)->id == id)
			{
				loadedSideQuests.erase(it);
				sidequestActive = *it;
				break;
			}
		}
	}
	else {
		DeleteAllSideQuests();
	}

	return true;
}


void QuestManager::DeleteAllMainQuests()
{
	if (!loadedQuests.empty())
	{
		eastl::list<Quest*>::iterator it = loadedQuests.begin();
		eastl::list<Quest*>::iterator itEnd = loadedQuests.end();
		for (; it != itEnd; ++it)
		{
			RELEASE(*it);
			loadedQuests.erase(it);
		}
	}
	loadedQuests.clear();

	if (!activeQuests.empty())
	{
		eastl::list<Quest*>::iterator it = activeQuests.begin();
		eastl::list<Quest*>::iterator itEnd = activeQuests.end();
		for (; it != itEnd; ++it)
		{
			RELEASE(*it);
			activeQuests.erase(it);
		}
	}
	activeQuests.clear();

	RELEASE(questActive);
	questActive = nullptr;

}

void QuestManager::DeleteAllSideQuests()
{
	if (!loadedSideQuests.empty())
	{
		eastl::list<Quest*>::iterator it = loadedSideQuests.begin();
		eastl::list<Quest*>::iterator itEnd = loadedSideQuests.end();
		for (; it != itEnd; ++it)
		{
			RELEASE(*it);
			loadedSideQuests.erase(it);
		}
	}
	loadedSideQuests.clear();

	if (!activeSideQuests.empty())
	{
		eastl::list<Quest*>::iterator it = activeSideQuests.begin();
		eastl::list<Quest*>::iterator itEnd = activeSideQuests.end();
		for (; it != itEnd; ++it)
		{
			RELEASE(*it);
			activeSideQuests.erase(it);
		}
	}
	activeSideQuests.clear();

	RELEASE(sidequestActive);
	sidequestActive = nullptr;

}

bool QuestManager::CleanUp() {


	app->tex->Unload(QuestMenuBox);
	font->UnLoad(app->tex);
	RELEASE(font);

	firePaw->CleanUp();
	delete(firePaw);
	firePaw = nullptr;

	return true;
}