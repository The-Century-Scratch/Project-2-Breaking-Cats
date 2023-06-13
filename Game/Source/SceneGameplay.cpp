#include "App.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Audio.h"
//#include "Font.h"

#include "Debug.h"
#include "ModuleCollisions.h"
#include "Hud.h"
#include "Window.h"
#include "SceneGameplay.h"
#include "SceneManager.h"
#include "SceneBattle.h"
//#include "AssetsManager.h"
#include "EntityManager.h"
//#include "Hunter.h"
//#include "Wizard.h"
//#include "Thief.h"
//#include "Warrior.h"

//#include "Golem.h"
//#include "Bat.h"
//#include "Skull.h"

//Items
#include "FirePaws.h"
#include "DragonSlayer.h"
#include "GrapplingHook.h"
#include "BulletPenetration.h"
#include "MysticalEnergy.h"
#include "ArcaneSpirit.h"

//#include "KnightHelmet.h"
//#include "KnightChest.h"

#include "Npc.h"
#include "MovableObject.h"
#include "TriggerableObject.h"
#include "Map.h"
//#include "DialogueManager.h"

//#include "IceBlock.h"
//#include "Door.h"
//#include "Statue.h"
#include "QuestManager.h"

//#include "CharacterManager.h"
//#include "PauseMenu.h"
#include "Inventory.h"
#include "InventoryShop.h"
//#include "QuestMenu.h"
//#include "Shop.h"

#include "Log.h"


#define NPC_RADIUS 10

SceneGameplay::SceneGameplay()
{
	name.Create("scenegameplay");

	//// Startup
	//menuState = GameplayMenuState::NONE;
	//gameState = GameplayState::ROAMING;

	//questManager = new QuestManager();
	//entityManager = new EntityManager(questManager);
	//particles = new ParticlesManager();

	//iPoint position = { 260,290 };

	//pugi::xml_document animations;
	//pugi::xml_node anims;
	//int size = app->assetsManager->MakeLoad("Xml/animations.xml");
	//pugi::xml_parse_result result = animations.load_buffer(app->assetsManager->GetLastBuffer(), size);
	//app->assetsManager->DeleteBuffer();

	//if (result == NULL) 
	//	LOG("Could not load xml file: %s. pugi error: %s", "animations.xml", result.description());
	//else 
	//	anims = animations.child("animations");

	//Player* player = nullptr;
	//player = new Hunter(position, anims, particles);
	//currentPlayer = player;
	//playerList.push_back(player);
	//position = { 18,90 };
	//player = new Wizard(position, anims, particles);
	//playerList.push_back(player);

	//position = { 12,45 };
	//player = new Thief(position, anims, particles);
	//playerList.push_back(player);

	//position = { 13,56 };
	//player = new Warrior(position, anims, particles);
	//playerList.push_back(player);

	//tmpPosPlayer = { 0,0 };

	//Enemy* en;
	//en = new Skull({ 608, 352 }, anims, "graveyard.tmx");
	//en->SetCurrentState(EnemyState::ROAMING);
	//enemyList.push_back(en);

	//en = new Bat({ 1408, 2680 }, anims, "dungeon_map.tmx");
	//en->SetCurrentState(EnemyState::ROAMING);
	//enemyList.push_back(en);

	//en = new Golem({ 1408, 224 }, anims, "dungeon_map.tmx");
	//en->SetCurrentState(EnemyState::ROAMING);
	//enemyList.push_back(en);

	//atlas = app->tex->Load("Textures/Items/items_atlas.png");

	//inventory = new Inventory(playerList, atlas, this);

	//Item *item = new UltraPotion(iPoint(128,1248), atlas, "town_map.tmx");
	//items.push_back(item);
	//
	//item = new OmniPotion(iPoint(768,1248), atlas, "town_map.tmx");
	//items.push_back(item);

	//item = new Potion(iPoint(1280, 80), atlas, "town_map.tmx");
	//items.push_back(item);

	//item = new FairyTear(iPoint(512, 800), atlas, "town_map.tmx");
	//items.push_back(item);

	//item = new KnightHelmet({ 1376, 1056, 32, 32 }, iPoint(1376, 1056), atlas, "town_map.tmx");
	//items.push_back(item);

	//item = new KnightChest({1408, 1056, 32, 32}, iPoint(1408, 1056), atlas, "town_map.tmx");
	//items.push_back(item);

	//pause = new PauseMenu(this);

	//font = new Font(app, "Font/font3.xml", app->tex);
	//quests = new QuestMenu(this, questManager, font);

	//showColliders = false;
	//transition = false;
	//fadeOut = false;
	//alpha = 0.0f;
	//isDungeon = false;
	//loadObjects = true;
	//deleteDoor = true;
	//cameraCounter = 120;

	//canSound1 = true;
	//canSound2 = true;
	//canSound3 = true;
	//canSound4 = true;

	//firstQuest = 0.0f;
	//firstQuestAdded = false;
	//tmp = nullptr;
	//sceneBattle = nullptr;

	//lastUserInput = 0;
	pugi::xml_node configNode = app->LoadConfigFileToVar();
	pugi::xml_node config = configNode.child(name.GetString());

	pugi::xml_node itemNode = config.child("item");
	itemText = app->tex->Load(itemNode.child("texturepath").attribute("texturepath").as_string());

	//inventory = new Inventory(playerList, atlas, this);

}

bool SceneGameplay::Load()
{
	LOG("Loading Scene Gameplay");
	bool ret = true;

	pugi::xml_node configNode = app->LoadConfigFileToVar();
	pugi::xml_node config = configNode.child(name.GetString());

	if (app->entityManager->state == false) { app->entityManager->Enable(); }

	currentPlayer = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
	currentPlayer->parameters = config.child("player");
	currentPlayer->Start();
	//map = new Map(true);
	//app->map->Load("city_square.tmx");
	isTown = app->map->Load(name.GetString());

	cityTheme = config.child("citytheme").attribute("path").as_string();
	shopTheme = config.child("shoptheme").attribute("path").as_string();
	forestTheme = config.child("foresttheme").attribute("path").as_string();

	for (pugi::xml_node npcNode = config.child("npc"); npcNode; npcNode = npcNode.next_sibling("npc"))
	{
		if (npcNode.attribute("scene").as_int() == app->sceneManager->currentScene)
		{
			NPC* npc = (NPC*)app->entityManager->CreateEntity(EntityType::NPC);
			npc->parameters = npcNode;
			npcs.Add(npc);
			npc->Start();
		}
	}

	Item* item = nullptr;
	pugi::xml_node itemNode = config.child("item");
	
	firePaw = new FirePaws(iPoint(itemNode.child("firePaws").attribute("x").as_int(), itemNode.child("firePaws").attribute("y").as_int()), itemText);
	items.Add(firePaw);
	firePaw->Start();

	dragonSlayer = new DragonSlayer(iPoint(itemNode.child("dragonSlayer").attribute("x").as_int(), itemNode.child("dragonSlayer").attribute("y").as_int()), itemText);
	items.Add(dragonSlayer);
	dragonSlayer->Start();

	grapplingHook = new GrapplingHook(iPoint(itemNode.child("grapplingHook").attribute("x").as_int(), itemNode.child("grapplingHook").attribute("y").as_int()), itemText);
	items.Add(grapplingHook);
	grapplingHook->Start();

	bulletPenetration = new BulletPenetration(iPoint(itemNode.child("bulletPenetration").attribute("x").as_int(), itemNode.child("bulletPenetration").attribute("y").as_int()), itemText);
	items.Add(bulletPenetration);
	bulletPenetration->Start();

	mysticalEnergy = new MysticalEnergy(iPoint(itemNode.child("mysticalEnergy").attribute("x").as_int(), itemNode.child("mysticalEnergy").attribute("y").as_int()), itemText);
	items.Add(mysticalEnergy);
	mysticalEnergy->Start();

	arcaneSpirit = new ArcaneSpirit(iPoint(itemNode.child("arcaneSpirit").attribute("x").as_int(), itemNode.child("arcaneSpirit").attribute("y").as_int()), itemText);
	items.Add(arcaneSpirit);
	arcaneSpirit->Start();



	for (pugi::xml_node movableObjectNode = config.child("movableObject"); movableObjectNode; movableObjectNode = movableObjectNode.next_sibling("movableObject"))
	{
		if (movableObjectNode.attribute("scene").as_int() == app->sceneManager->currentScene)
		{
			MovableObject* movableObject = (MovableObject*)app->entityManager->CreateEntity(EntityType::MOVABLEOBJECT);
			movableObject->parameters = movableObjectNode;
			movableObjectList.Add(movableObject);
			movableObject->Start();
		}
	}





	for (pugi::xml_node triggerableObjectNode = config.child("triggerableObject"); triggerableObjectNode; triggerableObjectNode = triggerableObjectNode.next_sibling("triggerableObject"))
	{
		if (triggerableObjectNode.attribute("scene").as_int() == app->sceneManager->currentScene)
		{
			TriggerableObject* triggerableObject = (TriggerableObject*)app->entityManager->CreateEntity(EntityType::TRIGGERABLEOBJECT);
			triggerableObject->parameters = triggerableObjectNode;
			triggerableObjectList.Add(triggerableObject);
			triggerableObject->Start();
		}
	}

	for (pugi::xml_node collectibleObjectNode = config.child("collectibleObject"); collectibleObjectNode; collectibleObjectNode = collectibleObjectNode.next_sibling("collectibleObject"))
	{
		if (collectibleObjectNode.attribute("scene").as_int() == app->sceneManager->currentScene)
		{
			CollectibleObject* collectibleObject = (CollectibleObject*)app->entityManager->CreateEntity(EntityType::COLLECTIBLEOBJECT);
			collectibleObject->parameters = collectibleObjectNode;
			collectibleObjectList.Add(collectibleObject);
			collectibleObject->Start();
		}
	}





	//Item* item = nullptr;
	//pugi::xml_node itemNode = config.child("item");
	//{
	//	if (itemNode.child("firePaws").attribute("scene").as_int() == app->sceneManager->currentScene)
	//	{
	//		item = new FirePaws(iPoint(itemNode.child("firePaws").attribute("x").as_int(), itemNode.child("firePaws").attribute("y").as_int()), itemText);
	//		items.push_back(item);
	//		item->Start();
	//	}
	//}




	switch (app->sceneManager->currentScene)
	{
	case -1:
		app->render->camera.x = 0;
		app->render->camera.y = 0;
		canMoveCam = false;
		break;
	case 0:
		app->render->camera.x = -597;
		app->render->camera.y = 0;
		canMoveCam = true;
		break;
	case 1:
		app->render->camera.x = 283;
		app->render->camera.y = -433;
		canMoveCam = true;
		break;
	case 2:
		app->render->camera.x = 375;
		app->render->camera.y = 43;
		canMoveCam = false;
		break;
	case 3:
		app->render->camera.x = 102;
		app->render->camera.y = 18;
		canMoveCam = false;
		break;
	case 4:
		app->render->camera.x = 0;
		app->render->camera.y = 0;
		canMoveCam = true;
	case 5:
		app->render->camera.x = 0;
		app->render->camera.y = 0;
		canMoveCam = true;
	case 6:
		app->render->camera.x = 0;
		app->render->camera.y = 0;
		canMoveCam = true;
	case 7:
		app->render->camera.x = 0;
		app->render->camera.y = 0;
		canMoveCam = true;
	case 8:
		app->render->camera.x = 0;
		app->render->camera.y = -2000;
		canMoveCam = true;
	default:
		break;
	}

	addItems_ = true;

	app->inventory->Enable();

	//goldTexture = app->tex->Load("Textures/UI/gold.png");
	//guiTex = app->tex->Load("Textures/UI/gui_gameplay_textures.png");
	//guiPad = app->tex->Load("Textures/UI/gui_pad_buttons.png");

	//entityManager->Load();

	//eastl::list<Player*>::iterator it = playerList.begin();
	//eastl::list<Player*>::iterator itEnd = playerList.end();
	//for (; it != itEnd; ++it)
	//	(*it)->Load();

	//// Instantiate character swap manager
	//charManager = new CharacterManager(this, currentPlayer->playerType, font);
	//charManager->Load(font);

	//// Start music
	//channel = app->audio->SetChannel();

	//app->audio->PlayMusic("Audio/Music/village_theme_1.ogg", true);

	////Load Fx
	//doorOpenedFx = app->audio->LoadFx("Audio/Fx/Gameplay/door_open.wav");
	//doorClosedFx = app->audio->LoadFx("Audio/Fx/Gameplay/door_close.wav");
	//puzzleCompletedFx = app->audio->LoadFx("Audio/Fx/Gameplay/puzzle_completed.wav");

	//map = new Map();
	//isTown = true;
	//map->Load("town_map.tmx", app->tex);
	//
	//LoadNpc(map->name);

	//questManager->SetPlayer(currentPlayer);

	//dialogueManager = new DialogueManager(questManager, this);
	//dialogueManager->Start();

	//pause->Load(font);

	//inventory->Load(font);

	//shop = new Shop(this, inventory, atlas, currentPlayer);
	//shop->Load(font);

	//eastl::list<Item*>::iterator item = items.begin();
	//for (; item != items.end(); ++item)
	//	(*item)->Load();

	//SDL_ShowCursor(SDL_ENABLE);

	return ret;
}

bool SceneGameplay::Update(float dt)
{
	//UpdatingButtons(app->input);

	bool ret = true;
	//canMoveCam = true;
	int cameraSpeed = 1;
	if (canMoveCam)
	{
		if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		{
			app->render->camera.y += cameraSpeed;
		}
		if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		{
			app->render->camera.y -= cameraSpeed;
		}
		if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		{
			app->render->camera.x += cameraSpeed;
		}
		if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		{
			app->render->camera.x -= cameraSpeed;
		}
	}

	if (canMoveCam)
	{
		if (app->sceneManager->currentScene == 1 || app->sceneManager->currentScene == IDLAB)
		{
			if (app->sceneManager->currentScene == 1)
			{
				app->render->camera.x = 283;
			}
			if (app->sceneManager->currentScene ==  IDLAB)
			{
				app->render->camera.x = 150;
			}
			//camera fix to player in y axis
			if (currentPlayer->position.y > 300 / app->win->scale && currentPlayer->position.y < (app->map->mapData.tileHeight * app->map->mapData.height) - 420 / app->win->scale)
			{
				app->render->camera.y = -1 * app->win->scale * (currentPlayer->position.y - 300 / app->win->scale);
			}
			else if (currentPlayer->position.y <= 300 / app->win->scale || currentPlayer->position.y < 0)
			{
				app->render->camera.y = 0;
			}
			else if (currentPlayer->position.y >= (app->map->mapData.tileHeight * app->map->mapData.height) - 420 / app->win->scale)
			{
				//app->render->camera.y = (1 - app->win->scale) * app->win->height;
				app->render->camera.y = -1 * app->win->scale * ((app->map->mapData.tileHeight * app->map->mapData.height) - 720 / app->win->scale);
			}
		}
		else
		{
			//camera fix to player in x axis
			if (currentPlayer->position.x > (app->win->width / 2) / app->win->scale && currentPlayer->position.x < (app->map->mapData.tileWidth * app->map->mapData.width) - (app->win->width / 2) / app->win->scale)
			{
				app->render->camera.x = -1 * app->win->scale * (currentPlayer->position.x - (app->win->width / 2) / app->win->scale);
			}
			else if (currentPlayer->position.x <= (app->win->width / 2) / app->win->scale)
			{
				app->render->camera.x = 0;
			}
			else if (currentPlayer->position.x >= (app->map->mapData.tileWidth * app->map->mapData.width) - (app->win->width / 2) / app->win->scale)
			{
				app->render->camera.x = -1 * app->win->scale * ((app->map->mapData.tileWidth * app->map->mapData.width) - 1280 / app->win->scale);
			}
			//camera fix to player in y axis
			if (currentPlayer->position.y > (app->win->height / 2) / app->win->scale && currentPlayer->position.y < (app->map->mapData.tileHeight * app->map->mapData.height) - (app->win->height / 2) / app->win->scale)
			{
				app->render->camera.y = -1 * app->win->scale * (currentPlayer->position.y - (app->win->height / 2) / app->win->scale);
			}
			else if (currentPlayer->position.y <= (app->win->height / 2) / app->win->scale || currentPlayer->position.y < 0)
			{
				app->render->camera.y = 0;
			}
			else if (currentPlayer->position.y >= (app->map->mapData.tileHeight * app->map->mapData.height) - (app->win->height / 2) / app->win->scale)
			{
				//app->render->camera.y = (1 - app->win->scale) * app->win->height;
				app->render->camera.y = -1 * app->win->scale * ((app->map->mapData.tileHeight * app->map->mapData.height) - 720 / app->win->scale);
			}
		}
	}

	if (addItems_)
	{
		app->inventoryShop->AddItem(dragonSlayer);
		app->inventoryShop->AddItem(bulletPenetration);
		app->inventoryShop->AddItem(arcaneSpirit);
		addItems_ = false;
	}

	if (app->input->GetKey(SDL_SCANCODE_1) == KeyState::KEY_DOWN)
	{
		app->inventory->AddItem(firePaw);
		app->audio->PlayFx(app->hud->getitemfx);
	}
	if (app->input->GetKey(SDL_SCANCODE_2) == KeyState::KEY_DOWN)
	{
		app->inventory->AddItem(dragonSlayer);
		app->audio->PlayFx(app->hud->getitemfx);
	}
	if (app->input->GetKey(SDL_SCANCODE_3) == KeyState::KEY_DOWN)
	{
		app->inventory->AddItem(grapplingHook);
		app->audio->PlayFx(app->hud->getitemfx);
	}
	if (app->input->GetKey(SDL_SCANCODE_4) == KeyState::KEY_DOWN)
	{
		app->inventory->AddItem(bulletPenetration);
		app->audio->PlayFx(app->hud->getitemfx);
	}
	if (app->input->GetKey(SDL_SCANCODE_5) == KeyState::KEY_DOWN)
	{
		app->inventory->AddItem(mysticalEnergy);
		app->audio->PlayFx(app->hud->getitemfx);
	}
	if (app->input->GetKey(SDL_SCANCODE_6) == KeyState::KEY_DOWN)
	{
		app->inventory->AddItem(arcaneSpirit);
		app->audio->PlayFx(app->hud->getitemfx);
	}

	if (app->input->GetKey(SDL_SCANCODE_C) == KeyState::KEY_DOWN)
	{
		app->map->CleanUp();
		app->map->ClearMaps();

		app->sceneManager->currentScene = rand() % 3; //TODO: after finishing the loading of enemies from maps, make this the way to randomly select which map to go to

		app->render->camera.x = 0;
		app->render->camera.y = 0;

		

		app->sceneManager->current->TransitionToScene(SceneType::BATTLE, TransitionType::ALTERNATING_BARS);

	}

	if (app->input->GetKey(SDL_SCANCODE_Z) == KeyState::KEY_DOWN)
	{

		app->map->CleanUp();
		app->map->ClearMaps();

		app->sceneManager->currentScene = 0; //TODO: after finishing the loading of enemies from maps, make this the way to randomly select which map to go to

		app->render->camera.x = 0;
		app->render->camera.y = 0;



		app->sceneManager->current->TransitionToScene(SceneType::ENDING, TransitionType::ALTERNATING_BARS);

	}


	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		app->moduleCollisions->debug = !app->moduleCollisions->debug;
		if (app->moduleCollisions->debug)
		{
			//CODE OF DEBUG MODE
		}
	}

	// check in which door is entering
	if (app->sceneManager->changeMap)
	{

		if (app->sceneManager->currentScene == IDSCENEBASE) {
			ChangeMap(LEAVEBASE, IDSCENEMAP);
		}
		else if (app->sceneManager->currentScene == IDSCENESTORE) {
			ChangeMap(LEAVESTORE, IDSCENEMAP);
		}
		else if (app->sceneManager->currentScene == IDSCENETABERN) {
			ChangeMap(LEAVETABERN, IDSCENEMAP);
		}


		//leaving village map
		if (app->sceneManager->currentScene == IDVILLAGE)
		{
			ChangeMap(LEAVEVILLAGE, IDLABRINTH);
		}

		//leaving tutorial map
		if (app->sceneManager->currentScene == IDTUTORIAL)
		{
			/*ChangeMap(LEAVETUTORIAL, IDVILLAGE);*/
		}

		//leaving labrinth map
		if (app->sceneManager->currentScene == IDLABRINTH)
		{
			if (app->sceneManager->village)
			{
				ChangeMap(LEAVELABRINTHLEFT, IDVILLAGE);
				app->sceneManager->village = false;
			}
			if (app->sceneManager->leftAfterLabrinth)
			{
				ChangeMap(LEAVELABRINTHRIGHT, IDAFTERLABRINTH);
				app->sceneManager->leftAfterLabrinth = false;
			}
		}
		//leaving afterlabrinth map
		if (app->sceneManager->currentScene == IDAFTERLABRINTH)
		{
			if (app->sceneManager->rightLabrinth)
			{
				ChangeMap(LEAVEAFTERLABRINTHLEFT, IDLABRINTH);
				app->sceneManager->rightLabrinth = false;
			}
			if (app->sceneManager->nordCity)
			{
				ChangeMap(LEAVEAFTERLABRINTHDOWN, IDSCENEMAP);
				app->sceneManager->nordCity = false;
			}
		}

		//leaving prelab map
		if (app->sceneManager->currentScene == IDPRELAB)
		{
			if (app->sceneManager->downCity)
			{
				//app->map->CleanUp();
				//app->map->ClearMaps();
				//app->sceneManager->currentScene = 0; //TODO: after finishing the loading of enemies from maps, make this the way to randomly select which map to go to
				//app->render->camera.x = 0;
				//app->render->camera.y = 0;
				////app->sceneManager->current->TransitionToScene(SceneType::BATTLE, TransitionType::ALTERNATING_BARS);

				ChangeMap(LEAVEPRELABTOP, IDSCENEMAP);
				app->sceneManager->downCity = false;
			}
			if (app->sceneManager->lab)
			{
				ChangeMap(LEAVEPRELAB, IDLAB);
				app->sceneManager->lab = false;
			}
		}

		//leaving lab map
		if (app->sceneManager->currentScene == IDLAB)
		{
			if (app->sceneManager->downCity)
			{
				ChangeMap(LEAVELABTOP, IDSCENEMAP);
				app->sceneManager->downCity = false;
			}
			if (app->sceneManager->prelab)
			{
				ChangeMap(LEAVELAB, IDPRELAB);
				app->sceneManager->prelab = false;
			}
		}

		if (app->sceneManager->currentScene == IDSCENEMAP)
		{
			//LEAVING CITY MAP
			if (app->sceneManager->store) {
				ChangeMap(INIT_POS_STORE, IDSCENESTORE);
				app->sceneManager->store = false;
			}
			else if (app->sceneManager->tabern) {
				ChangeMap(INIT_POS_TABERN, IDSCENETABERN);
				app->sceneManager->tabern = false;
			}
			else if (app->sceneManager->resistance_base) {
				ChangeMap(INIT_POS_BASE, IDSCENEBASE);
				app->sceneManager->resistance_base = false;
			}
			else if (app->sceneManager->downAfterLabrinth)
			{
				ChangeMap(LEAVECITYTOP, IDAFTERLABRINTH);
				app->sceneManager->downAfterLabrinth = false;
			}
			else if (app->sceneManager->topPreLab )
			{
				ChangeMap(LEAVECITYDOWN, IDPRELAB);
				app->sceneManager->topPreLab = false;
			}
		}
	}
	
	//load request out from a bool triggerable out of scene
	if (app->sceneManager->LoadRequestOutScene)
	{
		app->LoadGameRequest();
		app->sceneManager->LoadRequestOutScene = false;
	}

	//save and load inputs on keyboard
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
	{
		app->SaveGameRequest();
		SDL_ShowCursor(SDL_ENABLE);
	}
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
	{
		app->LoadGameRequest();
		SDL_ShowCursor(SDL_ENABLE);
	}
	//save and load inputs on controller
	if (app->input->pad->GetButton(SDL_CONTROLLER_BUTTON_BACK) == KEY_REPEAT)
	{
		if (CONTROLLERBACKRIGHT)
		{
			app->SaveGameRequest();
		}
		if (CONTROLLERBACKLEFT)
		{
			app->LoadGameRequest();
		}
		SDL_ShowCursor(SDL_DISABLE);
	}

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		app->hud->prevstate = app->hud->hudstate;
		app->hud->hudstate = hudSTATE::PAUSESCREEN;
		app->hud->wait1frame = true;
		app->sceneManager->Pause = true;
		SDL_ShowCursor(SDL_ENABLE);
	}
	if (CONTROLLERSTART)
	{
		app->hud->prevstate = app->hud->hudstate;
		app->hud->hudstate = hudSTATE::PAUSESCREEN;
		app->hud->wait1frame = true;
		app->sceneManager->Pause = true;
		SDL_ShowCursor(SDL_DISABLE);
	}

	return ret;
}

void SceneGameplay::Draw()
{
	app->map->Draw();

	

	if (app->debug->drawVariables)
	{
		DrawDebugVariable();
	}

	
}

bool SceneGameplay::UnLoad()
{
	LOG("Unloading Scene Gameplay");
	bool ret = true;

	if (app->entityManager->state) { app->entityManager->Disable(); }



	firePaw->CleanUp();
	delete(firePaw);
	firePaw = nullptr;

	dragonSlayer->CleanUp();
	delete(dragonSlayer);
	dragonSlayer = nullptr;

	grapplingHook->CleanUp();
	delete(grapplingHook);
	grapplingHook = nullptr;

	bulletPenetration->CleanUp();
	delete(bulletPenetration);
	bulletPenetration = nullptr;

	mysticalEnergy->CleanUp();
	delete(mysticalEnergy);
	mysticalEnergy = nullptr;

	arcaneSpirit->CleanUp();
	delete(arcaneSpirit);
	arcaneSpirit = nullptr;

	app->tex->Unload(itemText);
	app->tex->Unload(guiTex);
	app->tex->Unload(guiPad);

	return ret;
}

void SceneGameplay::CharacterSwap(PlayerType player)
{
	
}

bool SceneGameplay::LoadState(pugi::xml_node& load)
{
	//info of gameplay data
	app->sceneManager->currentScene = load.child("SceneGameplayInfo").attribute("CurrentMap").as_int();
	app->sceneManager->puzzle1solved = load.child("SceneGameplayInfo").attribute("Puzzle1Solved").as_bool();
	app->sceneManager->puzzle2solved = load.child("SceneGameplayInfo").attribute("Puzzle2Solved").as_bool();
	app->sceneManager->puzzle3solved = load.child("SceneGameplayInfo").attribute("Puzzle3Solved").as_bool();
	app->sceneManager->puzzle4solved = load.child("SceneGameplayInfo").attribute("Puzzle4Solved").as_bool();

	//player data
	currentPlayer->position.x = load.child("Player").attribute("x").as_int();
	currentPlayer->position.y = load.child("Player").attribute("y").as_int();


	//finally, after loading everything, lets apply automatically everything else
	ChangeMap(currentPlayer->position, app->sceneManager->currentScene);

	return true;
}

bool SceneGameplay::SaveState(pugi::xml_node& save) const
{
	//info of gameplay data
	pugi::xml_node sceneGameplayInfoNode = save.append_child("SceneGameplayInfo");
	sceneGameplayInfoNode.append_attribute("CurrentMap") = app->sceneManager->currentScene;
	sceneGameplayInfoNode.append_attribute("Puzzle1Solved") = app->sceneManager->puzzle1solved;
	sceneGameplayInfoNode.append_attribute("Puzzle2Solved") = app->sceneManager->puzzle2solved;
	sceneGameplayInfoNode.append_attribute("Puzzle3Solved") = app->sceneManager->puzzle3solved;
	sceneGameplayInfoNode.append_attribute("Puzzle4Solved") = app->sceneManager->puzzle4solved;
	
	//Player data
	pugi::xml_node playerNode = save.append_child("Player");
	playerNode.append_attribute("x") = currentPlayer->position.x;
	playerNode.append_attribute("y") = currentPlayer->position.y;

	return true;
}

void SceneGameplay::ChangeState(GameplayMenuState type)
{
	//menuState = type;
}

void SceneGameplay::HandleInput(Input* input, float dt)
{
	
}

bool SceneGameplay::CollisionMapEntity(SDL_Rect rect, EntityType type)
{
	

	return false;
}

bool SceneGameplay::CheckCollision(SDL_Rect rec1, SDL_Rect rec2)
{
	if ((rec1.x < (rec2.x + rec2.w) && (rec1.x + rec1.w) > rec2.x) &&
		(rec1.y < (rec2.y + rec2.h) && (rec1.y + rec1.h) > rec2.y)) return true;

	else return false;
}

void SceneGameplay::GenerateBattle()
{

}

void SceneGameplay::CameraFollow(Render* render)
{
	
}

void SceneGameplay::Transitioning(float dt)
{
	
}

void SceneGameplay::ChangeBlockBounds(int bounds_x, int bounds_y)
{

}

void SceneGameplay::LoadStaticObject()
{
	ListItem<StaticObject*>* staticObjectItem = staticObjectList.start;
	while (staticObjectItem != NULL)
	{
		staticObjectItem->data->toDelete = true;
		staticObjectItem = staticObjectItem->next;
	}
	staticObjectList.Clear();

	pugi::xml_node configNode = app->LoadConfigFileToVar();
	pugi::xml_node config = configNode.child(name.GetString());

	for (pugi::xml_node staticObjectNode = config.child("staticObject"); staticObjectNode; staticObjectNode = staticObjectNode.next_sibling("staticObject"))
	{
		if (staticObjectNode.attribute("scene").as_int() == app->sceneManager->currentScene)
		{
			if ((app->sceneManager->currentScene == IDAFTERLABRINTH && !app->sceneManager->puzzle2solved) ||	//if current scene is after labrinth and puzzle 2 is NOT solved it will spawn
				(app->sceneManager->currentScene == IDLAB && !app->sceneManager->puzzle4solved))		//if current scene is lab and puzzle 4 is NOT solved it will spawn
			{
				StaticObject* staticObject = (StaticObject*)app->entityManager->CreateEntity(EntityType::STATICOBJECT);
				staticObject->parameters = staticObjectNode;
				staticObjectList.Add(staticObject);
				staticObject->Start();
			}
		}
	}
}

void SceneGameplay::LoadMovableObjects()
{
	ListItem<MovableObject*>* movableObjectItem = movableObjectList.start;
	while (movableObjectItem != NULL)
	{
		movableObjectItem->data->toDelete = true;
		movableObjectItem = movableObjectItem->next;
	}
	movableObjectList.Clear();

	pugi::xml_node configNode = app->LoadConfigFileToVar();
	pugi::xml_node config = configNode.child(name.GetString());

	for (pugi::xml_node movableObjectNode = config.child("movableObject"); movableObjectNode; movableObjectNode = movableObjectNode.next_sibling("movableObject"))
	{
		if (movableObjectNode.attribute("scene").as_int() == app->sceneManager->currentScene)
		{
			MovableObject* movableObject = (MovableObject*)app->entityManager->CreateEntity(EntityType::MOVABLEOBJECT, app->sceneManager->puzzle1solved);
			movableObject->parameters = movableObjectNode;
			movableObjectList.Add(movableObject);
			movableObject->Start();
		}
	}
}

void SceneGameplay::LoadTriggerableObjects()
{
	ListItem<TriggerableObject*>* triggerableObjectItem = triggerableObjectList.start;
	while (triggerableObjectItem != NULL)
	{
		triggerableObjectItem->data->toDelete = true;
		triggerableObjectItem = triggerableObjectItem->next;
	}
	triggerableObjectList.Clear();

	pugi::xml_node configNode = app->LoadConfigFileToVar();
	pugi::xml_node config = configNode.child(name.GetString());

	for (pugi::xml_node triggerableObjectNode = config.child("triggerableObject"); triggerableObjectNode; triggerableObjectNode = triggerableObjectNode.next_sibling("triggerableObject"))
	{
		if (triggerableObjectNode.attribute("scene").as_int() == app->sceneManager->currentScene)
		{
			TriggerableObject* triggerableObject = (TriggerableObject*)app->entityManager->CreateEntity(EntityType::TRIGGERABLEOBJECT, app->sceneManager->puzzle2solved);
			triggerableObject->parameters = triggerableObjectNode;
			triggerableObjectList.Add(triggerableObject);
			triggerableObject->Start();
		}
	}
}

void SceneGameplay::LoadCollectibleObjects()
{
	ListItem<CollectibleObject*>* collectibleObjectItem = collectibleObjectList.start;
	while (collectibleObjectItem != NULL)
	{
		collectibleObjectItem->data->toDelete = true;
		collectibleObjectItem = collectibleObjectItem->next;
	}
	collectibleObjectList.Clear();

	pugi::xml_node configNode = app->LoadConfigFileToVar();
	pugi::xml_node config = configNode.child(name.GetString());

	for (pugi::xml_node collectibleObjectNode = config.child("collectibleObject"); collectibleObjectNode; collectibleObjectNode = collectibleObjectNode.next_sibling("collectibleObject"))
	{
		if (collectibleObjectNode.attribute("scene").as_int() == app->sceneManager->currentScene)
		{
			CollectibleObject* collectibleObject = (CollectibleObject*)app->entityManager->CreateEntity(EntityType::COLLECTIBLEOBJECT);
			collectibleObject->parameters = collectibleObjectNode;
			collectibleObjectList.Add(collectibleObject);
			collectibleObject->Start();
		}
	}
}

void SceneGameplay::LoadNpc()
{
	ListItem<NPC*>* npcItem = npcs.start;
	while (npcItem != NULL)
	{
		npcItem->data->toDelete = true;
		npcItem = npcItem->next;
	}
	npcs.Clear();

	pugi::xml_node configNode = app->LoadConfigFileToVar();
	pugi::xml_node config = configNode.child(name.GetString());

	for (pugi::xml_node npcNode = config.child("npc"); npcNode; npcNode = npcNode.next_sibling("npc"))
	{
		if (npcNode.attribute("scene").as_int() == app->sceneManager->currentScene)
		{
			NPC* npc = (NPC*)app->entityManager->CreateEntity(EntityType::NPC);
			npc->parameters = npcNode;
			npcs.Add(npc);
			npc->Start();
		}
	}
}

void SceneGameplay::LoadItems(pugi::xml_node& n)
{

}

void SceneGameplay::SetCameraMovement(int target_x, int target_y, float dt)
{
	if (app->render->camera.x < target_x) app->render->camera.x += 700 * dt;
	if (app->render->camera.x > target_x) app->render->camera.x -= 700 * dt;
	if (app->render->camera.y > target_y) app->render->camera.y -= 700 * dt;
	if (app->render->camera.y < target_y) app->render->camera.y += 700 * dt;
}

void SceneGameplay::ChangeMap(iPoint newPos, int newScene)
{
	app->map->CleanUp();
	//CORE VARIABLE TO CHANGE SCENE
	app->sceneManager->currentScene = newScene;

	//set new position to player
	currentPlayer->position = newPos;

	//load again new npcs
	LoadNpc();
	//load again new movableobjects
	LoadMovableObjects();
	//load again new triggerableobjects
	LoadTriggerableObjects();
	//load again new collectbleobjects
	LoadCollectibleObjects();
	//load again new staticobjects
	LoadStaticObject();

	//set camera according new scene
	switch (app->sceneManager->currentScene)
	{
	case -1:
		app->render->camera.x = 0;
		app->render->camera.y = 0;
		canMoveCam = false;
		break;
	case 0:
		app->render->camera.x = -597;
		app->render->camera.y = 0;
		canMoveCam = true;
		app->audio->PlayMusic(cityTheme.GetString());
		break;
	case 1:
		app->render->camera.x = 283;
		app->render->camera.y = -433;
		canMoveCam = true;
		break;
	case 2:
		app->render->camera.x = 375;
		app->render->camera.y = 43;
		canMoveCam = false;
		app->audio->PlayMusic(shopTheme.GetString());
		break;
	case 3:
		app->render->camera.x = 102;
		app->render->camera.y = 18;
		canMoveCam = false;
		break;
	case 4:
		break;
	case 5:
		break;
	case 6:
		app->audio->PlayMusic(forestTheme.GetString());
		break;
	default:
		break;
	}

	app->map->Load(name.GetString());
	app->sceneManager->changeMap = false;

	//entityManager->DeleteAllNpcActive();
	//app->audio->PlayFx(channel, doorFx);

	//currentPlayer->bounds.x = newPos.x;
	//currentPlayer->bounds.y = newPos.y;
	//map->CleanUp();
	//map->Load(mapName, app->tex);

	//questManager->CheckQuests(map->name);
	//particles->SetAllParticlesDesactivated();
}

void SceneGameplay::UpdatingButtons(Input* input)
{
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT || 
		app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		lastUserInput = 1;
		SDL_ShowCursor(SDL_ENABLE);
	}
	else if (app->input->pad->GetButton(SDL_CONTROLLER_BUTTON_DPAD_UP) == KEY_REPEAT || app->input->pad->l_y < -0.5 || 
		app->input->pad->GetButton(SDL_CONTROLLER_BUTTON_DPAD_DOWN) == KEY_REPEAT || app->input->pad->l_y > 0.5 ||
		app->input->pad->GetButton(SDL_CONTROLLER_BUTTON_DPAD_LEFT) == KEY_REPEAT || app->input->pad->l_x < -0.5 || 
		app->input->pad->GetButton(SDL_CONTROLLER_BUTTON_DPAD_RIGHT) == KEY_REPEAT || app->input->pad->l_x > 0.5)
	{
		lastUserInput = 0;
	}
}

void SceneGameplay::DrawDebugVariable()
{
	//const char* text, int posX, int posY, int w, int h, SDL_Color color
	//to pass to string use ---> std::to_string(currentPlayer->position.x).c_str()
	app->render->DrawText("Player X  ", app->debug->debugX, app->debug->debugY, 100, 20, app->debug->debugColor);
	app->render->DrawText(std::to_string(currentPlayer->position.x).c_str(), app->debug->debugX + 110, app->debug->debugY, 50, 20, app->debug->debugColor);
	app->render->DrawText("Player Y  ", app->debug->debugX, app->debug->debugY + 30, 100, 20, app->debug->debugColor);
	app->render->DrawText(std::to_string(currentPlayer->position.y).c_str(), app->debug->debugX + 110, app->debug->debugY + 30, 50, 20, app->debug->debugColor);

	app->render->DrawText("Camara X  ", app->debug->debugX, app->debug->debugY + 80, 100, 20, app->debug->debugColor);
	app->render->DrawText(std::to_string(app->render->camera.x).c_str(), app->debug->debugX + 110, app->debug->debugY + 80, 50, 20, app->debug->debugColor);
	app->render->DrawText("Camara Y  ", app->debug->debugX, app->debug->debugY + 110, 100, 20, app->debug->debugColor);
	app->render->DrawText(std::to_string(app->render->camera.y).c_str(), app->debug->debugX + 110, app->debug->debugY + 110, 50, 20, app->debug->debugColor);

	int MouseX_, MouseY_;
	app->input->GetMousePosition(MouseX_, MouseY_);
	app->render->DrawText("Mouse X  ", app->debug->debugX, app->debug->debugY + 160, 100, 20, app->debug->debugColor);
	app->render->DrawText(std::to_string(MouseX_).c_str(), app->debug->debugX + 110, app->debug->debugY + 160, 50, 20, app->debug->debugColor);
	app->render->DrawText("Mouse Y  ", app->debug->debugX, app->debug->debugY + 190, 100, 20, app->debug->debugColor);
	app->render->DrawText(std::to_string(MouseY_).c_str(), app->debug->debugX + 110, app->debug->debugY + 190, 50, 20, app->debug->debugColor);

	app->render->DrawText("InvPosX  ", app->debug->debugX, app->debug->debugY + 240, 100, 20, app->debug->debugColor);
	app->render->DrawText(std::to_string(app->inventory->invPos.x).c_str(), app->debug->debugX + 110, app->debug->debugY + 240, 50, 20, app->debug->debugColor);
	app->render->DrawText("InvPosY  ", app->debug->debugX, app->debug->debugY + 270, 100, 20, app->debug->debugColor);
	app->render->DrawText(std::to_string(app->inventory->invPos.y).c_str(), app->debug->debugX + 110, app->debug->debugY + 270, 50, 20, app->debug->debugColor);
}