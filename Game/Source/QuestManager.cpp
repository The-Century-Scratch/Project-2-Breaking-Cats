#include "QuestManager.h"
#include "App.h"
#include "Textures.h"
#include "LabyrinthQuest.h"
#include "TalkQuest.h"
#include "MoveRockQuest.h"
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
		case QuestType::MOVEROCK:
			quest = new MoveRockQuest(node);
			break;
		case QuestType::TALK:
			quest = new TalkQuest(node);
			break;
		case QuestType::LABYRINTH:
			quest = new LabyrinthQuest(node);
			break;
		default:
			break;
		}		
		loadedQuests.push_back(quest);
	}

	completeQuestFx = app->audio->LoadFx("Assets/Audio/Fx/CompleteQuestPlaceholder .wav");
	QuestMenuBox = app->tex->Load("Assets/Textures/GUI/menuBoxPlaceholder.png");
	questFinished = nullptr;
	questActive = nullptr;
	currentQuestComplete = false;
	font = new Font(app, "Fonts/prova.xml");
	quest1 = false;
	quest3 = false;

	ItemText = app->tex->Load("Assets/Textures/Items.png");

	firePaw = new FirePaws(iPoint(384,48), ItemText);
	firePaw->Start();
	return ret;
}

bool QuestManager::Update(float dt)
{
	bool ret = true;

	if (app->sceneManager->puzzle2solved && quest1 == false) {
		ActivateQuest(1);
		quest1 = true;
	}

	

	if (app->sceneManager->puzzle3solved &&quest3 == false) {
		ActivateQuest(-1);
		quest3 = true;
		firePaw->equiped = true;
		app->inventory->AddItem(firePaw);
		app->audio->PlayFx(app->hud->getitemfx);
	}
	

	//ListItem<Quest*>* item;
	//Quest* pQuest = NULL;

	//for (item = activeQuests.start; item != NULL; item = item->next)
	//{
	//	pQuest = item->data;

	//	if (pQuest->Update() == false) {
	//		activeQuests.Del(item);

	//		ListItem<Quest*>* qitem = quests.start;
	//		while (qitem != nullptr)
	//		{
	//			Quest* item = qitem->data;
	//			if (item->id == pQuest->nextQuestId) {
	//				activeQuests.Add(item);
	//				break;
	//			}

	//			qitem = qitem->next;
	//		}

	//		completedQuests.Add(pQuest);
	//	}
	//}

	if (app->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN) 
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
		DeleteAllQuests();
	}

	return true;
}

void QuestManager::DeleteAllQuests()
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

	if (!finishedQuests.empty())
	{
		eastl::list<Quest*>::iterator it = finishedQuests.begin();
		eastl::list<Quest*>::iterator itEnd = finishedQuests.end();
		for (; it != itEnd; ++it)
		{
			RELEASE(*it);
			finishedQuests.erase(it);
		}
	}
	finishedQuests.clear();

	RELEASE(questActive);
	questActive = nullptr;

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