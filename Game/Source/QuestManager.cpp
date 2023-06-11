#include "QuestManager.h"
#include "App.h"
#include "Textures.h"
#include "MainQuest.h"
#include "SideQuest.h"
#include"AssetsManager.h"
#include "Audio.h"
#include "SceneManager.h"
#include "FirePaws.h"
#include "DragonSlayer.h"
#include "GrapplingHook.h"
#include "BulletPenetration.h"
#include "MysticalEnergy.h"
#include "ArcaneSpirit.h"

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

	completeQuestFx = app->audio->LoadFx("Assets/Audio/Fx/CompleteQuestPlaceholder.wav");
	QuestMenuBox = app->tex->Load("Assets/Textures/GUI/Menu.png");
	questActive = nullptr;
	sidequestActive = nullptr;
	font = new Font(app, "Fonts/prova.xml");
	RocksQuest = false;
	quest3 = false;
	changeDialogueIdAfterCollecting = false;
	changeDialogueIdAfterRocks = false;
	printQuestMenu = false;

	ObjectsCount = 0;
	MenuID = 1;

	//items
	ItemText = app->tex->Load("Assets/Textures/Items.png");

	firePaw = new FirePaws(iPoint(384,48), ItemText);
	firePaw->Start();

	dragonSlayer = new DragonSlayer(iPoint(384, 48), ItemText);
	dragonSlayer->Start();

	grapplingHook = new GrapplingHook(iPoint(384, 48), ItemText);
	grapplingHook->Start();

	bulletPenetration = new BulletPenetration(iPoint(384, 48), ItemText);
	bulletPenetration->Start();

	mysticalEnergy = new MysticalEnergy(iPoint(384, 48), ItemText);
	mysticalEnergy->Start();

	arcaneSpirit = new ArcaneSpirit(iPoint(384, 48), ItemText);
	arcaneSpirit->Start();

	GiveItem = false;

	ActivateQuest(TUTORIAL);
	return ret;
}

bool QuestManager::Update(float dt)
{
	bool ret = true;

	if (GiveItem) {
		switch (questActive->id) {
		case 1: //tutorial finished
			firePaw->equiped = true;
			app->inventory->AddItem(firePaw);
			app->audio->PlayFx(app->hud->getitemfx);
		case 2: //when rocks finished, give coin
		case 3: //When talked with the guardian
		case 4: //When the labyrinth is resolved
		case 5: //when you left the lab and talked with the guardian
		case 6: //when you found the vident
		default:
			break;
		}
		GiveItem = false;
	}

	if (ObjectsCount == 5) {
		sidequestActive = nullptr; //when doing the logics of having more than one sidequest active delete this also logic for adding a coin should be added
		changeDialogueIdAfterCollecting = true;
	}

	if (app->sceneManager->puzzle2solved && RocksQuest == false) {
		ActivateQuest(EXPLORECITY);
		RocksQuest = true;
		changeDialogueIdAfterRocks = true;
	}

	if (app->sceneManager->puzzle3solved &&quest3 == false) {
		// add item get when enterring laboratory
		ActivateQuest(ENDMISSIONS);
		quest3 = true;
		
	}


	if (app->input->GetKey(SDL_SCANCODE_O) == KEY_DOWN) 
	{
		printQuestMenu = !printQuestMenu;
	}

	if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN)
	{
		if (MenuID != 1 && printQuestMenu) {
			--MenuID;
		}
	}

	if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
	{
		if (MenuID != 2 && printQuestMenu) {
			++MenuID;
		}
	}

	return ret;
}

bool QuestManager::PostUpdate() {
	
	if(!printQuestMenu) { //dont delete
		return true;
	}
	SDL_Rect sect{ 0,0,417,240 };
	app->render->DrawTexture(QuestMenuBox, 0, 0, &sect, false);

	if (MenuID == 1) {
		if (questActive == nullptr) {
			app->render->DrawText(font, "No active quests", 80, 115, 80, 5, { 255,255,255,255 }, 900);
		}
		else {
			questActive->Draw(font);
		}
	}
	else if (MenuID == 2) {
		//afegir mouyre sidequests actives amb fletxes adalt i avall
		if (sidequestActive == nullptr) {
			app->render->DrawText(font, "No active sidequests", 80, 115, 80, 5, { 255,255,255,255 }, 900);
		}
		else {
			sidequestActive->Draw(font);
		}
	}
	//si despues me enxixo que se puedan seleccionar y que te diga las rewards de las complete quests
	/*else {
		if (sidequestActive == nullptr) {
			app->render->DrawText(font, "No complete quests", 80, 115, 80, 5, { 255,255,255,255 }, 1000);
		}
		else {
			sidequestActive->Draw(font);
		}
	}*/
	

	
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