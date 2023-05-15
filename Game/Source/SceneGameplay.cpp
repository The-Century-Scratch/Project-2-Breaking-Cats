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
	default:
		break;
	}

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
		if (app->sceneManager->currentScene == 1)
		{
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

	//Add items --> the final idea is to create a switch wich gets an id from 1-6 from the quest or chest and the switch will add the corresponding item to the inv
	if (app->input->GetKey(SDL_SCANCODE_I) == KeyState::KEY_DOWN)
	{
		app->inventory->isActivated = !app->inventory->isActivated;
	}

	

	if (app->input->GetKey(SDL_SCANCODE_1) == KeyState::KEY_DOWN)
	{
		firePaw->equiped = true;
		app->inventory->AddItem(firePaw);
	}
	if (app->input->GetKey(SDL_SCANCODE_2) == KeyState::KEY_DOWN)
	{
		dragonSlayer->equiped = true;
		app->inventory->AddItem(dragonSlayer);
	}
	if (app->input->GetKey(SDL_SCANCODE_3) == KeyState::KEY_DOWN)
	{
		grapplingHook->equiped = true;
		app->inventory->AddItem(grapplingHook);
	}
	if (app->input->GetKey(SDL_SCANCODE_4) == KeyState::KEY_DOWN)
	{
		bulletPenetration->equiped = true;
		app->inventory->AddItem(bulletPenetration);
	}
	if (app->input->GetKey(SDL_SCANCODE_5) == KeyState::KEY_DOWN)
	{
		mysticalEnergy->equiped = true;
		app->inventory->AddItem(mysticalEnergy);
	}
	if (app->input->GetKey(SDL_SCANCODE_6) == KeyState::KEY_DOWN)
	{
		arcaneSpirit->equiped = true;
		app->inventory->AddItem(arcaneSpirit);
	}

	if (app->input->GetKey(SDL_SCANCODE_C) == KeyState::KEY_DOWN)
	{
		
		int aux = app->inventory->GetFirePaw();
		LOG("the number that you are trying to check is %i", aux);

		app->map->CleanUp();
		app->map->ClearMaps();

		app->sceneManager->currentScene = 0; //TODO: after finishing the loading of enemies from maps, make this the way to randomly select which map to go to

		app->render->camera.x = 0;
		app->render->camera.y = 0;

		

		app->sceneManager->current->TransitionToScene(SceneType::BATTLE, TransitionType::ALTERNATING_BARS);

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
			ChangeMap(LEAVEVILLAGE, 4);
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
				ChangeMap(LEAVEPRELABTOP, IDSCENEMAP);
				app->sceneManager->downCity = false;
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
	
	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		app->hud->prevstate = app->hud->hudstate;
		app->hud->hudstate = hudSTATE::PAUSESCREEN;
		app->hud->wait1frame = true;
	}

	//switch (gameState)
	//{
	//case GameplayState::ROAMING:
	//	switch (menuState)
	//	{
	//	case GameplayMenuState::NONE:
	//		/*if(SDL_ShowCursor(-1) == SDL_DISABLE)
	//			SDL_ShowCursor(SDL_ENABLE);*/

	//		if (!firstQuestAdded)
	//		{
	//			firstQuest += 2.0f * dt;
	//			if (firstQuest >= 3.0f)
	//			{
	//				firstQuest = 0.0f;
	//				firstQuestAdded = true;
	//				questManager->ActivateQuest(0);
	//			}
	//		}

	//		if (dialogueManager->isDialogueActive == false)
	//		{
	//			if (!transition) questManager->Update(app->input, dt);
	//			
	//			if (questManager->QuestState() == false)
	//			{
	//				particles->PreUpdate();
	//				map->Update(dt);
	//				HandleInput(app->input, dt);
	//				SDL_Rect tmpBounds = currentPlayer->bounds;
	//				if (currentPlayer->canMove) currentPlayer->Update(dt);

	//				particles->Update(dt);

	//				eastl::list<Enemy*>::iterator enemies = enemyList.begin();
	//				eastl::list<Enemy*>::iterator enemyEnd = enemyList.end();
	//				for (; enemies != enemyEnd; ++enemies)
	//				{
	//					Enemy* enemy = (*enemies);
	//					if (map->name == enemy->mapName.c_str())
	//					{
	//						enemy->Update(dt);
	//						if (currentPlayer->canMove && !showColliders && CheckCollision(enemy->bounds, currentPlayer->bounds))
	//						{
	//							GenerateBattle();
	//							tmp = enemy;
	//							break;
	//						}
	//					}
	//				}
	//				SDL_Rect bounds = { currentPlayer->bounds.x,currentPlayer->bounds.y + currentPlayer->bounds.h - 7,currentPlayer->bounds.w,7 };
	//				
	//				if (showColliders == false && CollisionMapEntity(bounds, currentPlayer->type) == true)
	//					currentPlayer->bounds = tmpBounds;

	//				//PUZZLE
	//				if (isDungeon)
	//				{
	//					eastl::list<Entity*>::iterator entity = entityManager->entities.begin();
	//					eastl::list<Entity*>::iterator entityEnd = entityManager->entities.end();

	//					for (; entity != entityEnd; ++entity)
	//					{
	//						if (((*entity)->type) == EntityType::ICE_BLOCK)
	//						{
	//							bounds = (*entity)->bounds;
	//							break;
	//						}
	//					}

	//					IceBlock* tmpIceBlock = (IceBlock*)(*entity);
	//					if ((*entity) != nullptr)
	//					{
	//						if (*entity != nullptr && CollisionMapEntity(bounds, EntityType::ICE_BLOCK) == true)
	//						{
	//							bounds = iceBlockRect;

	//							if (tmpIceBlock->tmpState == EntityState::WALKING_RIGHT)
	//							{
	//								(*entity)->SetBounds({ tmpIceBlock->bounds.x - 1,  tmpIceBlock->bounds.y,  tmpIceBlock->bounds.w,  tmpIceBlock->bounds.h });
	//								(*entity)->SetState(EntityState::STOP_DOWN);
	//							}
	//							if (tmpIceBlock->tmpState == EntityState::WALLKING_LEFT)
	//							{
	//								(*entity)->SetBounds({ tmpIceBlock->bounds.x + 1,  tmpIceBlock->bounds.y,  tmpIceBlock->bounds.w,  tmpIceBlock->bounds.h });
	//								(*entity)->SetState(EntityState::STOP_DOWN);
	//							}
	//							if (tmpIceBlock->tmpState == EntityState::WALLKING_UP)
	//							{
	//								(*entity)->SetBounds({ tmpIceBlock->bounds.x,  tmpIceBlock->bounds.y + 1,  tmpIceBlock->bounds.w,  tmpIceBlock->bounds.h });
	//								(*entity)->SetState(EntityState::STOP_DOWN);
	//							}
	//							if (tmpIceBlock->tmpState == EntityState::WALLKING_DOWN)
	//							{
	//								(*entity)->SetBounds({ tmpIceBlock->bounds.x,  tmpIceBlock->bounds.y - 1,  tmpIceBlock->bounds.w,  tmpIceBlock->bounds.h });
	//								(*entity)->SetState(EntityState::STOP_DOWN);
	//							}
	//						}
	//						if (*entity != nullptr && CollisionMapEntity(bounds, EntityType::ICE_BLOCK) == false)
	//						{
	//							iceBlockRect = bounds;
	//						}
	//					}

	//					if (!currentPlayer->canMove)
	//					{
	//						int target_x = 0;
	//						int target_y = 0;

	//						switch (whereMove)
	//						{
	//						case 1:
	//							target_x = 0;
	//							target_y = 10;
	//							SetCameraMovement(target_x, target_y, dt);
	//							if (cameraCounter >= 0 && app->render->camera.x >= target_x - 20 && app->render->camera.y >= target_y - 20)
	//							{
	//								cameraCounter--;
	//								app->render->camera.x = target_x;
	//								app->render->camera.y = target_y;
	//								if (cameraCounter == 60)
	//								{
	//									entityManager->DeleteEntity(EntityType::DOOR, 2);
	//									app->audio->PlayFx(channel, puzzleCompletedFx);
	//								}
	//								if (cameraCounter <= 0)
	//								{
	//									currentPlayer->bounds.x += 10;
	//									currentPlayer->canMove = true;
	//									cameraCounter = 120;
	//								}
	//							}
	//							break;
	//						case 2:
	//							target_x = -1010;
	//							target_y = -459;
	//							SetCameraMovement(target_x, target_y, dt);
	//							if (cameraCounter >= 0 && app->render->camera.x <= target_x + 20 && app->render->camera.y >= target_y - 20)
	//							{
	//								cameraCounter--;
	//								app->render->camera.x = target_x;
	//								app->render->camera.y = target_y;
	//								if (cameraCounter == 60)
	//								{
	//									entityManager->DeleteEntity(EntityType::STATUE, 4);
	//									app->audio->PlayFx(channel, puzzleCompletedFx);
	//								}
	//								if (cameraCounter <= 0)
	//								{
	//									currentPlayer->bounds.x += 10;
	//									currentPlayer->canMove = true;
	//									cameraCounter = 120;
	//								}
	//							}
	//							break;
	//						case 3:
	//							target_x = -802;
	//							target_y = -323;
	//							SetCameraMovement(target_x, target_y, dt);
	//							if (cameraCounter >= 0 && app->render->camera.x <= target_x + 20 && app->render->camera.y >= target_y - 20)
	//							{
	//								cameraCounter--;
	//								app->render->camera.x = target_x;
	//								app->render->camera.y = target_y;
	//								if (cameraCounter == 60)
	//								{
	//									entityManager->DeleteEntity(EntityType::DOOR, 4);
	//									app->audio->PlayFx(channel, puzzleCompletedFx);
	//								}
	//								if (cameraCounter <= 0)
	//								{
	//									currentPlayer->bounds.x += 10;
	//									currentPlayer->canMove = true;
	//									cameraCounter = 120;
	//								}
	//							}
	//							break;
	//						case 4:
	//							target_x = -587;
	//							target_y = -453;
	//							SetCameraMovement(target_x, target_y, dt);
	//							if (cameraCounter >= 0 && app->render->camera.x <= target_x + 20 && app->render->camera.y >= target_y - 20)
	//							{
	//								cameraCounter--;
	//								app->render->camera.x = target_x;
	//								app->render->camera.y = target_y;
	//								if (cameraCounter == 60)
	//								{
	//									entityManager->DeleteEntity(EntityType::STATUE, 3);
	//									app->audio->PlayFx(channel, puzzleCompletedFx);
	//								}
	//								if (cameraCounter <= 0)
	//								{
	//									currentPlayer->bounds.x += 10;
	//									currentPlayer->canMove = true;
	//									cameraCounter = 120;
	//								}
	//							}
	//							break;
	//						case 6:
	//							target_x = -1600;
	//							target_y = -616;
	//							SetCameraMovement(target_x, target_y, dt);
	//							if (cameraCounter >= 0 && app->render->camera.x >= target_x - 20 && app->render->camera.y <= target_y + 20)
	//							{
	//								cameraCounter--;
	//								app->render->camera.x = target_x;
	//								app->render->camera.y = target_y;
	//								if (cameraCounter == 60)
	//								{
	//									entityManager->DeleteEntity(EntityType::DOOR, 1);
	//									app->audio->PlayFx(channel, puzzleCompletedFx);
	//								}
	//								if (cameraCounter <= 0)
	//								{
	//									currentPlayer->canMove = true;
	//									cameraCounter = 120;
	//								}
	//							}
	//							break;
	//						default:
	//							break;
	//						}
	//					}
	//				}

	//				if (currentPlayer->canMove) CameraFollow(app->render);

	//				int tmpId = -1;
	//				entityManager->Update(dt, currentPlayer, dialogueManager->isDialogueActive, tmpId, this);
	//				if (tmpId != -1)
	//				{
	//					dialogueManager->LoadDialogue(tmpId);
	//					dialogueManager->printText = true;
	//				}

	//				eastl::list<Item*>::iterator it = items.begin();
	//				for (; it != items.end(); ++it)
	//				{
	//					if (map->name == (*it)->mapName.c_str())
	//					{
	//						(*it)->Update(dt);
	//						if (CheckCollision(currentPlayer->bounds, (*it)->bounds) && (*it)->isDropped == false)
	//						{
	//							questManager->CheckQuests(*it);
	//							inventory->AddItem(*it);
	//							items.erase(it);

	//							if (inventory->ObjectQuantity(ItemType::ORB) == 1)
	//							{
	//								app->audio->PlayFx(channel, puzzleCompletedFx);
	//								entityManager->DeleteEntity(EntityType::STATUE, 1);
	//								entityManager->DeleteEntity(EntityType::STATUE, 2);
	//								entityManager->DeleteEntity(EntityType::DOOR, 3);
	//							}
	//						}
	//					}
	//				}
	//			}
	//		}
	//		else
	//		{
	//			dialogueManager->Update(dt);
	//		}
	//		break;
	//	case GameplayMenuState::CHARACTER_SWAP:
	//		charManager->Update(dt);
	//		break;

	//	case GameplayMenuState::INVENTORY:
	//		inventory->Update(dt);
	//		HandleInput(app->input, dt);
	//		break;

	//	case GameplayMenuState::PAUSE:
	//		ret = pause->Update(dt);
	//		break;

	//	case GameplayMenuState::QUESTS:
	//		quests->Update(dt);
	//		HandleInput(app->input, dt);
	//		break;
	//	case GameplayMenuState::SHOP:
	//		shop->Update(dt);
	//		break;
	//	}
	//	break;
	//case GameplayState::BATTLE:
	//	if (sceneBattle->Update(dt) == false)
	//	{
	//		transition = true;
	//		fadeOut = true;
	//	}
	//break;
	//}

	//if (transition) Transitioning(dt);

	return ret;
}

void SceneGameplay::Draw()
{
	app->map->Draw();

	/*ListItem<Item*>* it = items.start;
	for (; it != items.end; ++it)
	{
		it->data->Draw();
	}*/

	if (app->debug->drawVariables)
	{
		DrawDebugVariable();
	}

	//SDL_Rect r = { 110,0,100,100 };

	//switch (gameState)
	//{
	//case GameplayState::ROAMING:
	//	map->Draw(showColliders,entityManager,currentPlayer);
	//	//entityManager->Draw(showColliders);
	//	//particles->PostUpdate();
	//	//currentPlayer->Draw(showColliders);
	//	
	//	app->render->DrawTexture(goldTexture, 5, 60, NULL, false);

	//	char tmp[32];
	//	sprintf_s(tmp, "%i", currentPlayer->gold);
	//	app->render->DrawText(font, tmp, 80, 75, 50, 10, SDL_Color({ 0,0,0,255 }));
	//	app->render->DrawText(font, tmp, 77, 72, 50, 10, SDL_Color({ 255,255,255,255 }));

	//	{
	//		eastl::list<Item*>::iterator it = items.begin();
	//		for (; it != items.end(); ++it)
	//		{
	//			if (map->name == (*it)->mapName.c_str())
	//			{
	//				(*it)->Draw(showColliders);
	//			}
	//		}
	//	}	

	//	{
	//		eastl::list<Enemy*>::iterator enemies = enemyList.begin();
	//		eastl::list<Enemy*>::iterator enemyEnd = enemyList.end();
	//		for (; enemies != enemyEnd; ++enemies)
	//		{
	//			Enemy* enemy = (*enemies);
	//			if (map->name == enemy->mapName.c_str())
	//			{
	//				enemy->Draw(showColliders);
	//			}
	//		}
	//	}

	//	app->render->DrawTexture(guiTex, 1170, 608, &r, false);
	//	r = { 0,0,100,100 };
	//	app->render->DrawTexture(guiTex, 1060, 608, &r, false);
	//	if (lastUserInput == 1)
	//	{
	//		r = { 0,100,70,40 };

	//		app->render->DrawTexture(guiTex, 1170, 668, &r, false);
	//		app->render->DrawCenterText(font, "E", { 1206,676,28,27 }, 34, 5, SDL_Color({ 255,255,255,255 }));

	//		app->render->DrawTexture(guiTex, 1060, 668, &r, false);
	//		app->render->DrawCenterText(font, "Q", { 1097,675,28,27 }, 34, 5, SDL_Color({ 255,255,255,255 }));
	//	}
	//	else if (lastUserInput == 0)
	//	{
	//		r = { 41,40,40,40 };
	//		app->render->DrawTexture(guiPad, 1200, 668, &r, false);

	//		r = { 203,40,40,40 };
	//		app->render->DrawTexture(guiPad, 1090, 668, &r, false);
	//	}

	//	if (menuState != GameplayMenuState::SHOP && dialogueManager->isDialogueActive)
	//	{
	//		app->render->DrawRectangle({ 0, 0,1280, 720 }, 0, 0, 0, 150, true, false);
	//		dialogueManager->Draw();
	//	}
	//	else
	//	{
	//		if (!transition) questManager->Draw(app->render, font);

	//		//if(!questManager->QuestState()) particles->PostUpdate();
	//	}

	//	switch (menuState)
	//	{
	//	case GameplayMenuState::NONE:
	//		if (!questManager->QuestState() && !dialogueManager->isDialogueActive) particles->PostUpdate();
	//		break;

	//	case GameplayMenuState::CHARACTER_SWAP:
	//		charManager->Draw(font, showColliders);
	//		break;

	//	case GameplayMenuState::PAUSE:
	//		pause->Draw(font, showColliders);
	//		break;

	//	case GameplayMenuState::INVENTORY:
	//		inventory->Draw(font, showColliders);
	//		break;
	//	case GameplayMenuState::QUESTS:
	//		quests->Draw(font, showColliders);
	//		break;
	//	case GameplayMenuState::SHOP:
	//		shop->Draw(font, showColliders);
	//		break;
	//	}
	//	break;
	//case GameplayState::BATTLE:
	//	if (sceneBattle != nullptr) sceneBattle->Draw(showColliders);
	//	break;
	//}

	//if (transition) TransitionsManager::GetInstance()->Draw();
}

bool SceneGameplay::UnLoad()
{
	LOG("Unloading Scene Gameplay");
	bool ret = true;

	if (app->entityManager->state) { app->entityManager->Disable(); }

	/*ListItem<Item*>* it = items.start;
	for (; it != items.end; ++it)
	{
		it->data->CleanUp();
	}*/

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

	//entityManager->UnLoad();
	//RELEASE(entityManager);

	//questManager->UnLoad();
	//RELEASE(questManager);

	//map->CleanUp();
	//RELEASE(map);
	//
	//eastl::list<Player*>::iterator it = playerList.begin();
	//eastl::list<Player*>::iterator itEnd = playerList.end();
	//for (; it != itEnd; ++it)
	//{
	//	(*it)->UnLoad();
	//	RELEASE((*it));
	//	playerList.erase(it);
	//}
	//playerList.clear();

	//eastl::list<Item*>::iterator item = items.begin();
	//eastl::list<Item*>::iterator itemEnd = items.end();
	//for (; item != itemEnd; ++item)
	//{
	//	(*item)->UnLoad();
	//	RELEASE((*item));
	//	items.erase(item);
	//}
	//items.clear();

	//eastl::list<Enemy*>::iterator en = enemyList.begin();
	//eastl::list<Enemy*>::iterator enemyEnd = enemyList.end();
	//for (; en != enemyEnd; ++en)
	//{
	//	(*en)->UnLoad();
	//	RELEASE((*en));
	//	enemyList.erase(en);
	//}
	//enemyList.clear();

	//charManager->UnLoad();
	//RELEASE(charManager);

	//pause->UnLoad();
	//RELEASE(pause);

	//inventory->UnLoad();
	//RELEASE(inventory);
	//
	//dialogueManager->UnLoad();
	//RELEASE(dialogueManager);

	//font->UnLoad(app->tex);
	//RELEASE(font);
	//
	//particles->CleanUp();
	//RELEASE(particles);

	//app->audio->UnLoadFxs();
	//app->tex->UnLoad(goldTexture);
	//app->tex->UnLoad(guiTex);
	//app->tex->UnLoad(guiPad);

	return ret;
}

void SceneGameplay::CharacterSwap(PlayerType player)
{
	//SDL_Rect tmpBounds = currentPlayer->bounds;
	//currentPlayer->generator->Stop();
	//if (player != currentPlayer->playerType)
	//{
	//	eastl::list<Player*>::iterator it = playerList.begin();
	//	eastl::list<Player*>::iterator itEnd = playerList.end();
	//	for (; it != itEnd; ++it)
	//	{
	//		Player* pl = (*it);
	//		if (pl->playerType == player)
	//		{
	//			currentPlayer = pl;
	//			break;
	//		}
	//	}
	//}
	//currentPlayer->bounds = tmpBounds;
}

bool SceneGameplay::LoadState(pugi::xml_node& load)
{
	//// If the player is not in the map he saved at, set the current map to that one
	//SString mapName = load.attribute("map_name").as_string();
	//firstQuestAdded = load.attribute("first_quest").as_bool();
	//if (map == nullptr)
	//{
	//	map = new Map();
	//	map->Load(mapName.GetString(), app->tex);
	//}
	//else if (mapName != map->name)
	//{
	//	map->CleanUp();
	//	map->Load(mapName.GetString(), app->tex);
	//}

	//pugi::xml_node toLoadEntities = load.child("entities");
	//pugi::xml_document animations;
	//pugi::xml_node anims;
	//int size = app->assetsManager->MakeLoad("Xml/animations.xml");
	//pugi::xml_parse_result result = animations.load_buffer(app->assetsManager->GetLastBuffer(), size);
	//app->assetsManager->DeleteBuffer();
	////pugi::xml_parse_result result = animations.load_file("animations.xml");
	//anims = animations.child("animations");

	//entityManager->LoadState(&toLoadEntities, &anims);

	//pugi::xml_node nodePlayer = toLoadEntities.child("players").child("player");
	//bool isCurrent = false;

	//eastl::list<Player*>::iterator item = playerList.begin();
	//eastl::list<Player*>::iterator itEnd = playerList.end();
	//for (; item != itEnd; ++item, nodePlayer = nodePlayer.next_sibling())
	//{
	//	Player* pl = (*item);
	//	isCurrent = nodePlayer.child("isCurrent").attribute("current").as_bool();
	//	pl->LoadState(nodePlayer);
	//	
	//	if (isCurrent) currentPlayer = pl;
	//	
	//	for (pugi::xml_attribute n = nodePlayer.child("Equipment").attribute("helmet"); n; n = n.next_attribute())
	//	{
	//		Armor* armor = nullptr;
	//		switch ((ArmorType)n.as_int())
	//		{
	//		case ArmorType::HELMET:
	//			armor = new KnightHelmet({ 1376, 1056, 32, 32 }, iPoint(1376, 1056), atlas, "town_map.tmx");
	//			armor->Load();
	//			break;
	//		case ArmorType::CHEST:
	//			armor = new KnightChest({ 1408, 1056, 32, 32 }, iPoint(1408, 1056), atlas, "town_map.tmx");
	//			armor->Load();
	//			break;
	//		}
	//		if(armor != nullptr) pl->SetEquipment(armor);
	//	}
	//}

	//inventory->SetPlayersList(playerList);
	//inventory->LoadState(load);
	//
	//eastl::list<Enemy*>::iterator enemies = enemyList.begin();
	//eastl::list<Enemy*>::iterator enemyEnd = enemyList.end();

	//for (; enemies != enemyEnd; ++enemies)
	//{
	//	(*enemies)->UnLoad();
	//	RELEASE(*enemies);
	//	enemyList.remove((*enemies));
	//}
	//enemyList.clear();

	//int enemyAmount = toLoadEntities.child("Enemies").attribute("amount").as_int();
	//pugi::xml_node NodeEnemy = toLoadEntities.child("Enemies");
	//pugi::xml_node NodeEnemyAuxiliar = NodeEnemy.child("Enemy");
	//for (int a = 0; a < enemyAmount; ++a)
	//{
	//	SString string;
	//	string = NodeEnemyAuxiliar.child("EnemyType").attribute("type").as_string();

	//	EntityType plType;
	//	Enemy* enemy = nullptr;
	//	if (string == "BAT")
	//		enemy = new Bat({ NodeEnemyAuxiliar.child("bounds").attribute("X").as_int(), NodeEnemyAuxiliar.child("bounds").attribute("Y").as_int() }, anims, NodeEnemyAuxiliar.child("map_name").attribute("value").as_string());
	//	else if (string == "GOLEM")
	//		enemy = new Golem({ NodeEnemyAuxiliar.child("bounds").attribute("X").as_int(), NodeEnemyAuxiliar.child("bounds").attribute("Y").as_int() }, anims, NodeEnemyAuxiliar.child("map_name").attribute("value").as_string());
	//	else if (string == "SKULL")
	//		enemy = new Skull({ NodeEnemyAuxiliar.child("bounds").attribute("X").as_int(), NodeEnemyAuxiliar.child("bounds").attribute("Y").as_int() }, anims, NodeEnemyAuxiliar.child("map_name").attribute("value").as_string());
	//	enemy->SetCurrentState(EnemyState::ROAMING);
	//	enemyList.push_back(enemy);
	//	NodeEnemyAuxiliar = NodeEnemyAuxiliar.next_sibling();
	//}

	//eastl::list<Item*>::iterator it = items.begin();
	//eastl::list<Item*>::iterator itemEnd = items.end();
	//for (; it != itemEnd; ++it)
	//{
	//	(*it)->UnLoad();
	//	RELEASE(*it);
	//	items.erase(it);
	//}
	//items.clear();

	//LoadItems(toLoadEntities.child("items"));

	//questManager->LoadQuests(load);

	return true;
}

bool SceneGameplay::SaveState(pugi::xml_node& save) const
{
	//save.append_attribute("map_name").set_value(map->name.GetString());
	//save.append_attribute("first_quest").set_value(firstQuestAdded);
	//
	//pugi::xml_node toSaveEntites = save.append_child("entities");
	//entityManager->SaveState(&toSaveEntites);

	//pugi::xml_node toSaveEnemies = toSaveEntites.append_child("Enemies");
	//pugi::xml_node enemiesAuxiliar;
	//toSaveEnemies.append_attribute("amount").set_value(enemyList.size());
	//eastl::list<Enemy*>::iterator enemies = enemyList.begin().mpNode;
	//eastl::list<Enemy*>::iterator enemyEnd = enemyList.end().mpNode;

	//for (; enemies != enemyEnd; ++enemies)
	//{
	//	enemiesAuxiliar = toSaveEnemies.append_child("Enemy");
	//	(*enemies)->SaveState(enemiesAuxiliar);
	//}

	//pugi::xml_node nodePlayers = toSaveEntites.append_child("players");
	//pugi::xml_node nodePlayersAuxiliar;
	//int playerAmount = 0;
	//nodePlayers.append_attribute("amount").set_value(playerList.size());
	//nodePlayersAuxiliar = nodePlayers.append_child("player");
	//eastl::list<Player*>::iterator aux = playerList.begin().mpNode;
	//eastl::list<Player*>::iterator auxEnd = playerList.end().mpNode;
	//for (aux; aux != auxEnd; ++aux, nodePlayersAuxiliar = nodePlayers.append_child("player"))
	//{
	//	Player* pl = (*aux);
	//	if (pl == currentPlayer)
	//	{
	//		nodePlayersAuxiliar.append_child("isCurrent").append_attribute("current").set_value(true);
	//	}
	//	else
	//	{
	//		nodePlayersAuxiliar.append_child("isCurrent").append_attribute("current").set_value(false);
	//	}
	//	pl->SaveState(nodePlayersAuxiliar);
	//}

	//eastl::list<Item*>::iterator it = items.begin().mpNode;
	//eastl::list<Item*>::iterator itEnd = items.end().mpNode;
	//pugi::xml_node items = toSaveEntites.append_child("items");
	//for (; it != itEnd; ++it)
	//{
	//	(*it)->SaveState(items.append_child("item"));
	//}

	//inventory->SaveState(save);

	//questManager->SaveQuests(save);

	return true;
}

void SceneGameplay::ChangeState(GameplayMenuState type)
{
	//menuState = type;
}

void SceneGameplay::HandleInput(Input* input, float dt)
{
	////if (app->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN || app->input->pad->GetButton(SDL_CONTROLLER_BUTTON_X) == KEY_DOWN)
	////{
	////	// Instantiate and load scene battle
	////	GenerateBattle();
	////}

	//if (input->GetKey(SDL_SCANCODE_I) == KEY_DOWN || input->pad->GetButton(SDL_CONTROLLER_BUTTON_Y) == KEY_DOWN) ChangeState(GameplayMenuState::CHARACTER_SWAP);
	//
	//if (menuState != GameplayMenuState::INVENTORY && input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN || input->pad->GetButton(SDL_CONTROLLER_BUTTON_START) == KEY_DOWN) ChangeState(GameplayMenuState::PAUSE);

	//if (input->GetKey(SDL_SCANCODE_E) == KEY_DOWN || input->pad->GetButton(SDL_CONTROLLER_BUTTON_X) == KEY_DOWN)
	//{
	//	//if (menuState == GameplayMenuState::INVENTORY)
	//	//{
	//	//	menuState = GameplayMenuState::NONE;
	//	//	inventory->ResetStates();
	//	//}
	//	//else
	//	//{
	//		menuState = GameplayMenuState::INVENTORY;
	//		inventory->GetEquipment(currentPlayer);
	//	//}
	//}

	//if (input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN || input->pad->GetButton(SDL_CONTROLLER_BUTTON_BACK) == KEY_UP) menuState = GameplayMenuState::QUESTS;
}

bool SceneGameplay::CollisionMapEntity(SDL_Rect rect, EntityType type)
{
	//iPoint pos = map->WorldToMap(rect.x, rect.y);

	//if (rect.x + rect.w > map->data.width * map->data.tileWidth) return true;
	//else if (rect.x < 0) return true;
	//else if (rect.y > map->data.tileWidth * map->data.height) return true;
	//else if (rect.y < 0) return true;

	//int x = pos.x + (rect.w / map->data.tileWidth);
	//int y = pos.y + (rect.h / map->data.tileHeight);

	//if (rect.w < map->data.tileWidth) x = pos.x + 1;
	//if (rect.h < map->data.tileHeight) y = pos.y + 1;

	//bool exit = false;
	//MapLayer* layer = (*map->data.layers.end().prev());

	/////////////
	//// 
	//// Only check adyacent tiles
	//for (int j = pos.y; j <= y; ++j)
	//{
	//	for (int i = pos.x; i <= x; ++i)
	//	{
	//		if (i >= 0 && j >= 0)
	//		{
	//			if ((layer->Get(i, j) == 769) && CheckCollision(map->GetTilemapRec(i, j), rect) ||
	//				(layer->Get(i, j) == 1537) && CheckCollision(map->GetTilemapRec(i, j), rect) ||
	//				(layer->Get(i, j) == 1) && CheckCollision(map->GetTilemapRec(i, j), rect) ||
	//				(layer->Get(i, j) == 3073) && CheckCollision(map->GetTilemapRec(i, j), rect))
	//			{
	//				return true;
	//			}
	//			if (type != EntityType::TOWN && type != EntityType::TABERN && type != EntityType::NPC_WIZARD)
	//			{
	//				if (isTown && (layer->Get(i, j) == 771) && CheckCollision(map->GetTilemapRec(i, j), rect))
	//				{
	//					isTown = false;
	//					isDungeon = false;
	//					
	//					ChangeMap("house1.tmx", { 625,480 }, doorOpenedFx);
	//					
	//					exit = true;
	//					break;
	//				}
	//				else if ((layer->Get(i, j) == 771) && CheckCollision(map->GetTilemapRec(i, j), rect))
	//				{
	//					isTown = true;
	//					ChangeMap("town_map.tmx", { 97,520 }, doorClosedFx);
	//					LoadNpc(map->name);
	//					isDungeon = false;

	//					exit = true;
	//					break;
	//				}
	//				if (isTown && (layer->Get(i, j) == 772) && CheckCollision(map->GetTilemapRec(i, j), rect))
	//				{
	//					ChangeMap("house2.tmx", { 625,480 }, doorOpenedFx);
	//					//app->audio->PlayFx(channel, doorOpenedFx);
	//					isTown = false;
	//					/*entityManager->DeleteAllNpcActive();
	//					iPoint position = { 625,480 };
	//					currentPlayer->bounds.x = position.x;
	//					currentPlayer->bounds.y = position.y;
	//					map->CleanUp();
	//					map->Load("house2.tmx", app->tex);*/
	//					isDungeon = false;
	//					/*questManager->CheckQuests(map->name);
	//					particles->SetAllParticlesDesactivated();*/
	//					exit = true;
	//					break;
	//				}
	//				else if ((layer->Get(i, j) == 772) && CheckCollision(map->GetTilemapRec(i, j), rect))
	//				{
	//					ChangeMap("town_map.tmx", { 70,765 }, doorClosedFx);
	//					LoadNpc(map->name);
	//					isTown = true;
	//					isDungeon = false;

	//					exit = true;
	//					break;
	//				}
	//				if (isTown && (layer->Get(i, j) == 773) && CheckCollision(map->GetTilemapRec(i, j), rect))
	//				{
	//					ChangeMap("house3.tmx", { 625,480 }, doorOpenedFx);
	//					isTown = false;
	//					isDungeon = false;

	//					exit = true;
	//					break;
	//				}
	//				else if ((layer->Get(i, j) == 773) && CheckCollision(map->GetTilemapRec(i, j), rect))
	//				{
	//					ChangeMap("town_map.tmx", { 380,1120 }, doorClosedFx);
	//					LoadNpc(map->name);
	//					
	//					isTown = true;
	//					isDungeon = false;

	//					exit = true;
	//					break;
	//				}
	//				if (isTown && (layer->Get(i, j) == 774) && CheckCollision(map->GetTilemapRec(i, j), rect))
	//				{
	//					ChangeMap("cave_house.tmx", { 625,430 }, doorOpenedFx);
	//					isTown = false;
	//					isDungeon = false;

	//					exit = true;
	//					break;
	//				}
	//				else if ((layer->Get(i, j) == 6) && CheckCollision(map->GetTilemapRec(i, j), rect))
	//				{
	//					ChangeMap("town_map.tmx", { 97,1100 }, doorClosedFx);
	//					isTown = true;
	//					isDungeon = false;

	//					exit = true;
	//					break;
	//				}
	//				if (isTown && (layer->Get(i, j) == 775) && CheckCollision(map->GetTilemapRec(i, j), rect))
	//				{
	//					ChangeMap("pub.tmx", { 655,430 }, doorOpenedFx);
	//					LoadNpc(map->name);
	//					isTown = false;
	//					isDungeon = false;

	//					exit = true;
	//					break;						
	//				}
	//				else if ((layer->Get(i, j) == 7) && CheckCollision(map->GetTilemapRec(i, j), rect))
	//				{
	//					ChangeMap("town_map.tmx", { 770,710 }, doorClosedFx);
	//					LoadNpc(map->name);
	//					isTown = true;
	//					isDungeon = false;

	//					exit = true;
	//					break;
	//				}
	//				if (isTown && (layer->Get(i, j) == 776) && CheckCollision(map->GetTilemapRec(i, j), rect))
	//				{
	//					ChangeMap("adventurer_house.tmx", { 630,450 }, doorOpenedFx);
	//					LoadNpc(map->name);
	//					isTown = false;
	//					isDungeon = false;
	//				
	//					exit = true;
	//					break;
	//				}
	//				else if ((layer->Get(i, j) == 1544) && CheckCollision(map->GetTilemapRec(i, j), rect))
	//				{
	//					ChangeMap("town_map.tmx", { 385,610 }, doorClosedFx);
	//					LoadNpc(map->name);
	//					isTown = true;
	//					isDungeon = false;

	//					exit = true;
	//					break;
	//				}
	//				if (isTown && (layer->Get(i, j) == 777) && CheckCollision(map->GetTilemapRec(i, j), rect))
	//				{
	//					ChangeMap("adventurer_house.tmx", { 750,300 }, doorOpenedFx);
	//					LoadNpc(map->name);
	//					isTown = false;
	//					isDungeon = false;

	//					exit = true;
	//					break;

	//				}
	//				else if ((layer->Get(i, j) == 1545) && CheckCollision(map->GetTilemapRec(i, j), rect))
	//				{
	//					ChangeMap("town_map.tmx", { 448,465 }, doorClosedFx);
	//					LoadNpc(map->name);
	//					isTown = true;
	//					isDungeon = false;

	//					exit = true;
	//					break;
	//				}
	//				if (isTown && (layer->Get(i, j) == 778) && CheckCollision(map->GetTilemapRec(i, j), rect))
	//				{
	//					ChangeMap("inn.tmx", { 650,480 }, doorOpenedFx);
	//					isTown = false;
	//					isDungeon = false;

	//					exit = true;
	//					break;
	//				}
	//				else if ((layer->Get(i, j) == 1546) && CheckCollision(map->GetTilemapRec(i, j), rect))
	//				{
	//					ChangeMap("town_map.tmx", { 380,136 }, doorClosedFx);
	//					LoadNpc(map->name);
	//					isTown = true;
	//					isDungeon = false;

	//					exit = true;
	//					break;
	//				}
	//				if (isTown && (layer->Get(i, j) == 779) && CheckCollision(map->GetTilemapRec(i, j), rect))
	//				{
	//					ChangeMap("library.tmx", { 615,480 }, doorOpenedFx);
	//					LoadNpc(map->name);
	//					isTown = false;
	//					isDungeon = false;

	//					exit = true;
	//					break;
	//				}
	//				else if ((layer->Get(i, j) == 1547) && CheckCollision(map->GetTilemapRec(i, j), rect))
	//				{
	//					ChangeMap("town_map.tmx", { 160,136 }, doorClosedFx);
	//					LoadNpc(map->name);
	//					isTown = true;
	//					isDungeon = false;
	//					
	//					exit = true;
	//					break;
	//				}
	//				if (isTown && (layer->Get(i, j) == 780) && CheckCollision(map->GetTilemapRec(i, j), rect))
	//				{
	//					ChangeMap("shop.tmx", { 615,480 }, doorOpenedFx);
	//					LoadNpc(map->name);
	//					isTown = false;
	//					isDungeon = false;

	//					exit = true;
	//					break;
	//				}
	//				else if ((layer->Get(i, j) == 12) && CheckCollision(map->GetTilemapRec(i, j), rect))
	//				{
	//					ChangeMap("town_map.tmx", { 898,152 }, doorClosedFx);
	//					LoadNpc(map->name);
	//					isTown = true;
	//					isDungeon = false;
	//					
	//					exit = true;
	//					break;
	//				}
	//				if (isTown && (layer->Get(i, j) == 782) && CheckCollision(map->GetTilemapRec(i, j), rect))
	//				{
	//					ChangeMap("shop2.tmx", { 615,480 }, doorOpenedFx);
	//					isTown = false;
	//					isDungeon = false;

	//					exit = true;
	//					break;
	//				}
	//				else if ((layer->Get(i, j) == 14) && CheckCollision(map->GetTilemapRec(i, j), rect))
	//				{
	//					ChangeMap("town_map.tmx", { 900,1250 }, doorClosedFx);
	//					isTown = true;
	//					isDungeon = false;
	//					LoadNpc(map->name);

	//					exit = true;
	//					break;
	//				}
	//				if (isTown && (layer->Get(i, j) == 783) && CheckCollision(map->GetTilemapRec(i, j), rect))
	//				{
	//					ChangeMap("red_house.tmx", { 615,480 }, doorOpenedFx);
	//					isTown = false;
	//					isDungeon = false;

	//					exit = true;
	//					break;
	//				}
	//				else if ((layer->Get(i, j) == 1551) && CheckCollision(map->GetTilemapRec(i, j), rect))
	//				{
	//					ChangeMap("town_map.tmx", { 1350,390 }, doorClosedFx);
	//					LoadNpc(map->name);
	//					isTown = true;
	//					isDungeon = false;

	//					exit = true;
	//					break;
	//				}
	//				if ((layer->Get(i, j) == 784) && CheckCollision(map->GetTilemapRec(i, j), rect))
	//				{
	//					ChangeMap("graveyard_path.tmx", { 705,950 });
	//					isTown = false;
	//					isDungeon = false;

	//					exit = true;
	//					break;
	//				}
	//				else if ((layer->Get(i, j) == 786) && CheckCollision(map->GetTilemapRec(i, j), rect))
	//				{
	//					ChangeMap("town_map.tmx", { 705,30 });
	//					LoadNpc(map->name);
	//					isTown = true;
	//					isDungeon = false;

	//					exit = true;
	//					break;
	//				}
	//				if ((layer->Get(i, j) == 785) && CheckCollision(map->GetTilemapRec(i, j), rect))
	//				{
	//					ChangeMap("bath_house.tmx", { 620,480 }, doorOpenedFx);
	//					isTown = false;
	//					isDungeon = false;

	//					exit = true;
	//					break;
	//				}
	//				else if ((layer->Get(i, j) == 1553) && CheckCollision(map->GetTilemapRec(i, j), rect))
	//				{
	//					ChangeMap("graveyard_path.tmx", { 320, 384 }, doorClosedFx);
	//					isTown = true;
	//					isDungeon = false;

	//					exit = true;
	//					break;
	//				}
	//				if ((layer->Get(i, j) == 3089) && CheckCollision(map->GetTilemapRec(i, j), rect))
	//				{
	//					ChangeMap("graveyard_path.tmx", { 320,380 });
	//					isTown = true;
	//					isDungeon = false;

	//					exit = true;
	//					break;
	//				}
	//				else if ((layer->Get(i, j) == 787) && CheckCollision(map->GetTilemapRec(i, j), rect))
	//				{
	//					ChangeMap("graveyard.tmx", { 630,960 });
	//					isTown = false;
	//					isDungeon = false;

	//					exit = true;
	//					break;
	//				}
	//				if ((layer->Get(i, j) == 19) && CheckCollision(map->GetTilemapRec(i, j), rect))
	//				{
	//					ChangeMap("graveyard_path.tmx", { 500,50 });
	//					isTown = true;
	//					isDungeon = false;

	//					exit = true;
	//					break;
	//				}
	//				else if ((layer->Get(i, j) == 20) && CheckCollision(map->GetTilemapRec(i, j), rect))
	//				{
	//					iPoint position = { 1420,720 };
	//					ChangeMap("dungeon_map.tmx", position);
	//					isTown = false;
	//					isDungeon = true;
	//					if (loadObjects)
	//					{
	//						position = { 2048,320 };
	//						IceBlock* iceBlock = (IceBlock*)entityManager->CreateEntity2(EntityType::ICE_BLOCK, position, currentPlayer, 1);
	//						position = { 2240,992 };
	//						Door* door = (Door*)entityManager->CreateEntity2(EntityType::DOOR, position, currentPlayer, 1);
	//						position = { 576,288 };
	//						Door* door2 = (Door*)entityManager->CreateEntity2(EntityType::DOOR, position, currentPlayer, 2);
	//						position = { 1408,2496 };
	//						Door* door3 = (Door*)entityManager->CreateEntity2(EntityType::DOOR, position, currentPlayer, 3);
	//						position = { 1408,608 };
	//						Door* door4 = (Door*)entityManager->CreateEntity2(EntityType::DOOR, position, currentPlayer, 4);

	//						Item* item = new OrbFragment(iPoint(2144, 2496), atlas, "dungeon_map.tmx");
	//						items.push_back(item);

	//						item = new OrbFragment(iPoint(2368, 2496), atlas, "dungeon_map.tmx");
	//						items.push_back(item);

	//						position = { 1184, 1937 };
	//						Statue* statue = (Statue*)entityManager->CreateEntity2(EntityType::STATUE, position, currentPlayer, 1);
	//						position = { 1184, 1990 };
	//						Statue* statue2 = (Statue*)entityManager->CreateEntity2(EntityType::STATUE, position, currentPlayer, 2);
	//						position = { 1184, 730 };
	//						Statue* statue3 = (Statue*)entityManager->CreateEntity2(EntityType::STATUE, position, currentPlayer, 3);
	//						position = { 1664, 730 };
	//						Statue* statue4 = (Statue*)entityManager->CreateEntity2(EntityType::STATUE, position, currentPlayer, 4);

	//						interruptorBlock = new InterruptorBlock(iPoint(1024, 416), atlas, "dungeon_map.tmx");
	//						items.push_back(interruptorBlock);

	//						loadObjects = false;
	//					}
	//					exit = true;
	//					break;
	//				}
	//				if ((layer->Get(i, j) == 18) && CheckCollision(map->GetTilemapRec(i, j), rect))
	//				{
	//					if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP || app->input->pad->GetButton(SDL_CONTROLLER_BUTTON_A) == KEY_UP)
	//					{
	//						// TODO IMPLEMENT PUZZLE BOX INTERRUPTOR
	//						interruptorBlock->bounds.x = currentPlayer->bounds.x;
	//						interruptorBlock->bounds.y = currentPlayer->bounds.y;
	//						items.push_back(interruptorBlock);
	//						interruptorBlock->isDropped = true;
	//					}
	//				}
	//				if (canSound1 && (layer->Get(i, j) == 21) && CheckCollision(map->GetTilemapRec(i, j), rect) && interruptorBlock->isDropped)
	//				{
	//					whereMove = 1;
	//					currentPlayer->canMove = false;
	//					canSound1 = false;
	//				}
	//				if (canSound2 && (layer->Get(i, j) == 17) && CheckCollision(map->GetTilemapRec(i, j), rect))
	//				{
	//					whereMove = 2;
	//					currentPlayer->canMove = false;
	//					canSound2 = false;
	//					exit = true;
	//					break;
	//				}
	//				if (canSound3 && (layer->Get(i, j) == 15) && CheckCollision(map->GetTilemapRec(i, j), rect))
	//				{
	//					whereMove = 3;
	//					currentPlayer->canMove = false;
	//					canSound3 = false;
	//					exit = true;
	//					break;
	//				}
	//				if (canSound4 && (layer->Get(i, j) == 16) && CheckCollision(map->GetTilemapRec(i, j), rect))
	//				{
	//					whereMove = 4;
	//					currentPlayer->canMove = false;
	//					canSound4 = false;
	//					exit = true;
	//					break;
	//				}
	//				if ((layer->Get(i, j) == 9) && CheckCollision(map->GetTilemapRec(i, j), iceBlockRect))
	//				{
	//					//isTown = false;
	//					//entityManager->SetAllNpcInactive();
	//					iPoint position = { 1950,360 };
	//					currentPlayer->bounds.x = position.x;
	//					currentPlayer->bounds.y = position.y;
	//					//isDungeon = true;
	//					ChangeBlockBounds(2048, 320);
	//					exit = true;
	//					break;
	//				}
	//				if (deleteDoor && (layer->Get(i, j) == 10) && CheckCollision(map->GetTilemapRec(i, j), iceBlockRect))
	//				{
	//					//isTown = false;
	//					//entityManager->SetAllNpcInactive();
	//					whereMove = 6;
	//					currentPlayer->canMove = false;
	//					deleteDoor = false;
	//					//isDungeon = true;
	//					exit = true;
	//					break;
	//				}
	//				if ((layer->Get(i, j) == 3) && CheckCollision(map->GetTilemapRec(i, j), rect))
	//				{
	//					//isTown = false;
	//					//entityManager->SetAllNpcInactive();
	//					iPoint position = { 1950,1650 };
	//					currentPlayer->bounds.x = position.x;
	//					currentPlayer->bounds.y = position.y;
	//					//isDungeon = true;
	//					exit = true;
	//					break;
	//				}
	//				if ((layer->Get(i, j) == 4) && CheckCollision(map->GetTilemapRec(i, j), rect))
	//				{
	//					isTown = false;
	//					entityManager->DeleteAllNpcActive();
	//					iPoint position = { 2570,1650 };
	//					currentPlayer->bounds.x = position.x;
	//					currentPlayer->bounds.y = position.y;
	//					//map->CleanUp();
	//					//map->Load("dungeon_map.tmx", app->tex);
	//					isDungeon = true;
	//					exit = true;
	//					break;
	//				}
	//				if ((layer->Get(i, j) == 13) && CheckCollision(map->GetTilemapRec(i, j), rect))
	//				{
	//					ChangeMap("graveyard.tmx", { 630,360 });
	//					isTown = true;
	//					isDungeon = false;

	//					entityManager->DeleteEntity(EntityType::DOOR);
	//					entityManager->DeleteEntity(EntityType::ICE_BLOCK);
	//					entityManager->DeleteEntity(EntityType::STATUE);
	//					loadObjects = true;
	//					
	//					exit = true;
	//					break;
	//				}
	//			}
	//		}
	//	}
	//	if (exit) break;
	//}

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
	//tmpPosPlayer = iPoint(currentPlayer->bounds.x, currentPlayer->bounds.y);
	//transition = true;
	//fadeOut = true;
	//TransitionsManager::GetInstance()->SetType(TransitionType::WIPE);
	//TransitionsManager::GetInstance()->SetStep(TransitionStep::ENTERING);
}

void SceneGameplay::CameraFollow(Render* render)
{
	//int midPlayerPosX = (currentPlayer->bounds.w / 2) + currentPlayer->bounds.x;
	//int midPlayerPosY = (currentPlayer->bounds.h / 2) + currentPlayer->bounds.y;
	//if (midPlayerPosX > -render->camera.x + (render->camera.w / 2) || midPlayerPosX < -render->camera.x + (render->camera.w / 2))
	//{
	//	render->camera.x = -(midPlayerPosX - (render->camera.w / 2));
	//}

	//if (midPlayerPosY > -render->camera.y + (render->camera.h / 2) || midPlayerPosY < -render->camera.y + (render->camera.h / 2))
	//{
	//	render->camera.y = -(midPlayerPosY - (render->camera.h / 2));
	//}

	//int mapWidth = map->data.width * map->data.tileWidth;
	//int mapHeight = map->data.height * map->data.tileHeight;
	//if (-render->camera.x + render->camera.w >= mapWidth) render->camera.x = -(mapWidth - render->camera.w);
	//if (-render->camera.x <= 0) render->camera.x = 0;

	//if (-render->camera.y + render->camera.h >= mapHeight) render->camera.y = -(mapHeight - render->camera.h);
	//if (-render->camera.y <= 0) render->camera.y = 0;
}

void SceneGameplay::Transitioning(float dt)
{
	//if (fadeOut)
	//{
	//	TransitionsManager::GetInstance()->EnteringTransition(dt);
	//	if (TransitionsManager::GetInstance()->GetStep() == TransitionStep::CHANGING)
	//	{
	//		if (sceneBattle == nullptr)
	//		{
	//			sceneBattle = new SceneBattle(playerList, tmp, this, inventory);
	//			sceneBattle->Load();
	//			gameState = GameplayState::BATTLE;
	//			fadeOut = false;
	//		}
	//		else
	//		{
	//			sceneBattle->UnLoad();
	//			RELEASE(sceneBattle);
	//			eastl::list<Enemy*>::iterator en = enemyList.begin();
	//			eastl::list<Enemy*>::iterator enEnd = enemyList.end();
	//			bool win = false;
	//			for (; en != enEnd; ++en)
	//			{
	//				Enemy* enemy = (*en);
	//				if (tmp != nullptr && enemy == tmp)
	//				{
	//					questManager->CheckQuests(*en);
	//					if (tmp->GetEnemyType() == EnemyType::GOLEM)
	//					{
	//						win = true;
	//					}

	//					tmp = nullptr;
	//					enemyList.erase(en);
	//					break;
	//				}
	//			}
	//			eastl::list<Player*>::iterator pl = playerList.begin();
	//			eastl::list<Player*>::iterator plEnd = playerList.end();
	//			int num = 0;
	//			for (; pl != plEnd; ++pl)
	//			{
	//				if ((*pl)->GetHealthPoints() <= 0) ++num;

	//				(*pl)->GetHealed((*pl)->GetMaxHealthPoints());
	//				(*pl)->GetMana((*pl)->GetMaxManaPoints());
	//			}

	//			if (win)
	//			{
	//				TransitionToScene(SceneType::ENDING, TransitionType::ALTERNATING_BARS, true);
	//			}
	//			else if (num == playerList.size())
	//			{
	//				TransitionToScene(SceneType::ENDING, TransitionType::ALTERNATING_BARS, false);
	//			}
	//			else
	//			{
	//				fadeOut = false;
	//				gameState = GameplayState::ROAMING;
	//			}
	//			
	//			currentPlayer->bounds.x = tmpPosPlayer.x;
	//			currentPlayer->bounds.y = tmpPosPlayer.y;
	//			app->audio->PlayMusic("Audio/Music/village_theme_1.ogg", true);
	//		}
	//	}
	//}
	//else
	//{
	//	if (TransitionsManager::GetInstance()->ExitingTransition(dt) == TransitionStep::NONE) transition = false;
	//}
}

void SceneGameplay::ChangeBlockBounds(int bounds_x, int bounds_y)
{
	//if (isDungeon)
	//{
	//	eastl::list<Entity*>::iterator entity = entityManager->entities.begin();
	//	eastl::list<Entity*>::iterator entityEnd = entityManager->entities.end();

	//	for (; entity != entityEnd; ++entity)
	//	{
	//		if (((*entity)->type) == EntityType::ICE_BLOCK)
	//		{
	//			(*entity)->bounds.x = bounds_x;
	//			(*entity)->bounds.y = bounds_y;
	//			(*entity)->SetState(EntityState::STOP_DOWN);
	//			break;
	//		}
	//	}
	//}
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
			if (app->sceneManager->currentScene == IDAFTERLABRINTH && !app->sceneManager->puzzle2solved)
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






	//pugi::xml_document animations;
	//pugi::xml_node anims;
	//int size = app->assetsManager->MakeLoad("Xml/animations.xml");
	//pugi::xml_parse_result result = animations.load_buffer(app->assetsManager->GetLastBuffer(), size);
	//app->assetsManager->DeleteBuffer();
	////pugi::xml_parse_result result = animations.load_file("animations.xml");
	//if (result == NULL)
	//	LOG("Could not load xml file: %s. pugi error: %s", CONFIG_FILENAME, result.description());
	//else
	//	anims = animations.child("animations");

	//if (mapName == "town_map.tmx")
	//{
	//	iPoint position = { 200,250 };
	//	entityManager->CreateEntity(EntityType::TOWN, position, anims, 0);

	//	position = { 700,1060 };
	//	entityManager->CreateEntity(EntityType::RAY, position, anims, 3);

	//	position = { 700,810 };
	//	entityManager->CreateEntity(EntityType::TABERN, position, anims, 2);

	//	position = { 500,350 };
	//	entityManager->CreateEntity(EntityType::NPC_WIZARD, position, anims, 1);
	//}
	//else if (mapName == "pub.tmx")
	//{
	//	iPoint position = { 638,280 };
	//	Npc* npc = (Npc*)entityManager->CreateEntity(EntityType::TOWN, position, anims, 4);
	//	npc->NpcMove(false);

	//	position = { 670,360 };
	//	entityManager->CreateEntity(EntityType::NPC_WIZARD, position, anims, 1);
	//}
	//else if (mapName == "adventurer_house.tmx")
	//{
	//	iPoint position = { 630,450 };
	//	entityManager->CreateEntity(EntityType::TABERN, position, anims, 4);

	//	position = { 630,420 };
	//	entityManager->CreateEntity(EntityType::NPC_WIZARD, position, anims, 1);
	//}
	//else if (mapName == "library.tmx")
	//{
	//	iPoint position = { 615,460 };
	//	entityManager->CreateEntity(EntityType::TOWN, position, anims, 5);

	//	position = { 600,450 };
	//	entityManager->CreateEntity(EntityType::NPC_WIZARD, position, anims, 6);
	//}
	//else if (mapName == "shop.tmx")
	//{
	//	iPoint position = { 608, 256 };
	//	entityManager->CreateEntity(EntityType::MERCHANT, position, anims, 7);
	//}
}

void SceneGameplay::LoadItems(pugi::xml_node& n)
{
	//pugi::xml_node node = n.child("item");

	//for (; node; node = node.next_sibling("item"))
	//{
	//	Item* item = nullptr;
	//	iPoint pos;
	//	if ((ObjectType)node.child("object_type").attribute("value").as_int() == ObjectType::ITEM)
	//	{
	//		switch ((ItemType)node.child("item_type").attribute("value").as_int())
	//		{
	//		case ItemType::POTION:
	//			pos = iPoint(node.child("position").attribute("x").as_int(), node.child("position").attribute("y").as_int());
	//			item = new Potion(pos, atlas, node.child("map_name").attribute("value").as_string());
	//			break;

	//		case ItemType::ULTRA_POTION:
	//			pos = iPoint(node.child("position").attribute("x").as_int(), node.child("position").attribute("y").as_int());
	//			item = new UltraPotion(pos, atlas, node.child("map_name").attribute("value").as_string());
	//			break;

	//		case ItemType::OMNI_POTION:
	//			pos = iPoint(node.child("position").attribute("x").as_int(), node.child("position").attribute("y").as_int());
	//			item = new OmniPotion(pos, atlas, node.child("map_name").attribute("value").as_string());
	//			break;

	//		case ItemType::FAIRY_TEAR:
	//			pos = iPoint(node.child("position").attribute("x").as_int(), node.child("position").attribute("y").as_int());
	//			item = new FairyTear(pos, atlas, node.child("map_name").attribute("value").as_string());
	//			break;
	//		}
	//	}
	//	else if ((ObjectType)node.child("object_type").attribute("value").as_int() == ObjectType::ARMOR)
	//	{
	//		switch ((ArmorType)node.child("armor_type").attribute("value").as_int())
	//		{
	//		case ArmorType::HELMET:
	//			pos = iPoint(node.child("position").attribute("x").as_int(), node.child("position").attribute("y").as_int());
	//			item = new KnightHelmet({pos.x, pos.y, node.child("position").attribute("w").as_int(), node.child("position").attribute("h").as_int() }, pos, atlas, node.child("map_name").attribute("value").as_string());
	//			break;

	//		case ArmorType::CHEST:
	//			pos = iPoint(node.child("position").attribute("x").as_int(), node.child("position").attribute("y").as_int());
	//			item = new KnightChest({ pos.x, pos.y, node.child("position").attribute("w").as_int(), node.child("position").attribute("h").as_int() }, pos, atlas, node.child("map_name").attribute("value").as_string());
	//			break;

	//		case ArmorType::BOOTS:
	//			break;
	//		}
	//	}
	//	if (item != nullptr)
	//	{
	//		item->Load();
	//		items.push_back(item);
	//	}
	//}
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