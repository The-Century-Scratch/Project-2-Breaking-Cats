#include "App.h"
#include "Render.h"
//#include"ParticlesManager.h"
#include "Textures.h"
//#include"AssetsManager.h"
#include "SceneBattle.h"
#include "SceneGameplay.h"
#include "SceneManager.h"
#include "Map.h"
#include "Catska.h"
#include "Gats.h"
#include "Guardian.h"
#include "LongRange.h"
#include "Straw.h"
//#include "BattleMenu.h"
#include "Player.h"
//#include "Enemy.h"
//#include "Golem.h"
//#include "Skull.h"
//#include "Bat.h"
#include "Audio.h"
//#include "Inventory.h"
#include <time.h>
#include "ModuleCollisions.h"
#include "Debug.h"

SceneBattle::SceneBattle()
{
	name.Create("scenebattle");
	//battleMenu = new BattleMenu(this, inventory);
	//map = new Map();
	//
	//if (enemy != nullptr)
	//{
	//	enemy->SetCurrentState(EnemyState::NORMAL);
	//	enemy->SetPos(iPoint(650, 140));
	//	enemyList.push_back(enemy);
	//}
}

SceneBattle::~SceneBattle()
{
}

bool SceneBattle::Load()
{

	LOG("Loading Scene Battle");
	bool ret = true;

	pugi::xml_node configNode = app->LoadConfigFileToVar();
	pugi::xml_node config = configNode.child(name.GetString());


	app->map->Load(name.GetString());



	for (pugi::xml_node nodeUnit = config.child("gats");
		nodeUnit; nodeUnit = nodeUnit.next_sibling("gats"))
	{

		std::unique_ptr<Unit> gats;
		gats = std::make_unique<Gats>();
		gats.get()->velocity = nodeUnit.attribute("velocity").as_int();
		gats.get()->parameters = nodeUnit;
		gats.get()->Create({ nodeUnit.attribute("x").as_int(), nodeUnit.attribute("y").as_int() });
		units.push_back(std::move(gats));
	}
	for (pugi::xml_node nodeUnit = config.child("catska");
		nodeUnit; nodeUnit = nodeUnit.next_sibling("catska"))
	{

		std::unique_ptr<Unit> catska;
		catska = std::make_unique<Catska>();
		catska.get()->velocity = nodeUnit.attribute("velocity").as_int();
		catska.get()->parameters = nodeUnit;

		catska.get()->Create({ nodeUnit.attribute("x").as_int(), nodeUnit.attribute("y").as_int() });
		units.push_back(std::move(catska));
	}
	for (pugi::xml_node nodeUnit = config.child("guardian");
		nodeUnit; nodeUnit = nodeUnit.next_sibling("guardian"))
	{

		std::unique_ptr<Unit> unit;
		unit = std::make_unique<Guardian>();
		unit.get()->velocity = nodeUnit.attribute("velocity").as_int();
		unit.get()->parameters = nodeUnit;

		unit.get()->Create({ nodeUnit.attribute("x").as_int(), nodeUnit.attribute("y").as_int() });
		units.push_back(std::move(unit));
	}
	for (pugi::xml_node nodeUnit = config.child("longrange");
		nodeUnit; nodeUnit = nodeUnit.next_sibling("longrange"))
	{

		std::unique_ptr<Unit> unit;
		unit = std::make_unique<LongRange>();
		unit.get()->velocity = nodeUnit.attribute("velocity").as_int();
		unit.get()->parameters = nodeUnit;

		unit.get()->Create({ nodeUnit.attribute("x").as_int(), nodeUnit.attribute("y").as_int() });
		units.push_back(std::move(unit));
	}
	for (pugi::xml_node nodeUnit = config.child("straw");
		nodeUnit; nodeUnit = nodeUnit.next_sibling("straw"))
	{

		std::unique_ptr<Unit> unit;
		unit = std::make_unique<Straw>();
		unit.get()->velocity = nodeUnit.attribute("velocity").as_int();
		unit.get()->parameters = nodeUnit;

		unit.get()->Create({ nodeUnit.attribute("x").as_int(), nodeUnit.attribute("y").as_int() });
		units.push_back(std::move(unit));
	}


	for (int i = 0; i < units.size(); ++i)
	{
		for (int j = 0; j < units.size() - 1 - i; ++j)
		{

			if (units[j].get()->velocity < units[j + 1].get()->velocity)
			{
				units[j].swap(units[j + 1]);
			}
		}
	}




	//unit.parameters = config.child("gats");
	//
	//unit.Create({64,64});



	//particles = new ParticlesManager();
	//if (scene->isDungeon == true)
	//{
	//	map->Load("battle_map2.tmx", app->tex);
	//}
	//else
	//{
	//	map->Load("battle_map.tmx", app->tex);
	//}

	//font = new Font(app, "Font/font3.xml", app->tex);

	////backgroundTexture = app->tex->Load("Assets/Textures/Scenes/battle_bg.png");

	//// Start music
	//app->audio->PlayMusic("Audio/Music/fight_theme_2.ogg", true);
	//
	//eastl::list<Player*>::iterator it = playerList.begin();
	//for (int i = 0; it != playerList.end(); ++it, ++i)
	//{
	//	(*it)->stance = PlayerStance::BATTLE;
	//	(*it)->SetIdleBattle();
	//	(*it)->bounds.x = 400;
	//	(*it)->bounds.y = 150 + (i * 50);
	//	(*it)->battlePos = iPoint((*it)->bounds.x, (*it)->bounds.y);
	//	(*it)->generatorList(particles, 1);
	//}

	//pugi::xml_document animations;
	//pugi::xml_node anims;
	//int size = app->assetsManager->MakeLoad("Xml/animations.xml");
	//pugi::xml_parse_result result = animations.load_buffer(app->assetsManager->GetLastBuffer(), size);
	//app->assetsManager->DeleteBuffer();
	////pugi::xml_parse_result result = animations.load_file("animations.xml");

	//if (result == NULL)
	//	LOG("Could not load xml file: %s. pugi error: %s", "animations.xml", result.description());
	//else
	//	anims = animations.child("animations");

	//Enemy* enemy = nullptr;
	//particles->CreateGenerator({ 0,0 }, ParticleType::FIRE);
	//particles->CreateGenerator({ 0,0 }, ParticleType::MAGIC);
	//particles->CreateGenerator({ 0,0 }, ParticleType::GRAVITY);
	//for (int i = 0; i < 2; ++i)
	//{
	//	int num = (rand() % 3) + 1;
	//	switch(num)
	//	{
	//	case 1:
	//		enemy = new Golem(iPoint(650, 230 + (i * 90)), anims);
	//		break;
	//	case 2:
	//		enemy = new Skull(iPoint(650, 230 + (i * 90)), anims);
	//		break;
	//	case 3:
	//		enemy = new Bat(iPoint(650, 230 + (i * 90)), anims);
	//		break;
	//	}
	//	//(rand()%2)
	//	enemy->generatorList(particles, 1);

	//	if (enemy != nullptr) enemyList.push_back(enemy);
	//}
	//
	//battleMenu->Load(font);
	//
	//app->render->camera.x = 0;
	//app->render->camera.y = 0;

	return true;
}

bool SceneBattle::Update(float dt)
{
	bool ret = true;
	//LOG("Scene battle is updating");

	using UA = Unit::PlayerAction::Action;



	//for (auto& i : units)
	//{
	//	
	//	auto unitAction = i->HandleInput();
	//
	//	if ((unitAction.action & UA::MOVE) == UA::MOVE)
	//	{
	//
	//		//if (map.IsWalkable(unitAction.destinationTile))
	//		if (app->moduleCollisions->isWalkable(unitAction.destinationTile))
	//		{
	//
	//			{
	//				i->StartAction(unitAction);
	//			}
	//		}
	//	}
	//	i->Update();
	//}

	//particles->PreUpdate();
	//eastl::list<Player*>::iterator item = playerList.begin();
	//for (; item != playerList.end(); ++item)
	//{
	//	(*item)->generatorList(particles, (rand() % 3));
	//	(*item)->Update(dt);
	//}

	//eastl::list<Enemy*>::iterator it = enemyList.begin();
	//for (; it != enemyList.end(); ++it)
	//{
	//	(*it)->generatorList(particles, (rand() % 3));
	//	(*it)->Update(dt);
	//}
	//particles->Update(dt);
	//if (battleMenu->Update(dt) == false) ret = false;

	bool noUnitHasActed = true;

	for (auto& i : units)
	{

		if (i->GetIsMyTurn() && !i->GetHasFinishedTurn() && i->GetHealthPoints() > 0)
		{
			noUnitHasActed = false;

			auto unitAction = i->HandleInput();

			if ((unitAction.action) == UA::MOVE)
			{

				if (app->moduleCollisions->isWalkable(unitAction.destinationTile))
				{
					i->StartAction(unitAction);
				}
			}
			if ((unitAction.action) == UA::PREPARE_DASH)
			{
				i->StartAction(unitAction);
			}
			// TODO: make this a switch
			if ((unitAction.action) == UA::ATTACK)
			{
				for (auto& unit : units)
				{
					if (!unit.get()->GetIsAlly() && unit->GetHealthPoints() > 0)
					{
						iPoint  unitPos = unit.get()->GetPosition();
						if (i->position.DistanceTo(unitPos) < 18)
						{
							unit->DealDamage(i->GetDamage());
							i->StartAction(unitAction);
							LOG("the health points that this unit has after the attack that you have thrown to it are the number that you are going to see: %i", unit->GetHealthPoints());
							i->SetHasFinishedTurn(true);

						}
					}
				}

			}
			if ((unitAction.action) == UA::ATTACK_LONG_RANGE)
			{
				for (auto& unit : units)
				{
					if (!unit.get()->GetIsAlly() && unit->GetHealthPoints() > 0)
					{
						iPoint  unitPos = unit.get()->GetPosition();
						if (i->position.x == unit->position.x || i->position.y == unit->position.y)
						{
							unit->DealDamage(i->GetDamage());
							i->StartAction(unitAction);
							LOG("the health points that this unit has after the attack that you have thrown to it are the number that you are going to see: %i", unit->GetHealthPoints());
							i->SetHasFinishedTurn(true);
							break;

						}
					}
				}

			}
			if ((unitAction.action) == UA::ATTACK_TO_PLAYER)
			{
				for (auto& unit : units)
				{
					if (unit.get()->GetIsAlly() && unit->GetHealthPoints() > 0)
					{
						iPoint  unitPos = unit.get()->GetPosition();
						if (i->position.DistanceTo(unitPos) < 18)
						{
							unit->DealDamage(i->GetDamage());
							i->StartAction(unitAction);
							LOG("the health points that this unit has after the attack that you have thrown to it are the number that you are going to see: %i", unit->GetHealthPoints());
							i->SetHasFinishedTurn(true);
						}
					}
				}

			}
			if ((unitAction.action) == UA::ATTACK_TO_PLAYER_LONG_RANGE)
			{
				for (auto& unit : units)
				{
					if (unit.get()->GetIsAlly() && unit->GetHealthPoints() > 0)
					{
						iPoint  unitPos = unit.get()->GetPosition();
						if (i->position.x == unit->position.x || i->position.y == unit->position.y)
						{
							unit->DealDamage(i->GetDamage());
							i->StartAction(unitAction);
							LOG("the health points that this unit has after the attack that you have thrown to it are the number that you are going to see: %i", unit->GetHealthPoints());
							i->SetHasFinishedTurn(true);
							break;
						}
					}
				}

			}



		}
		i->Update();

		if (i->GetHasFinishedTurn() && i->GetIsMyTurn())
		{
			i->SetIsMyTurn(false);
			numberFinished++; // that is a variable    // all of this check first if this variable is bigger thatn the allowed number for the vector

			if (numberFinished < units.size())
			{
				units[numberFinished]->SetIsMyTurn(true);
			}


			//noUnitHasActed = false;
		}
		if (!i->GetHealthPoints() > 0)
		{
			i->SetHasFinishedTurn(true);

		}
		if (i->GetIsMyTurn() && !i->GetHasFinishedTurn() && i->GetHealthPoints() <= 0)
		{
			i->SetHasFinishedTurn(true);
		}

	}


	if (noUnitHasActed)
	{
		for (auto& i : units)
		{
			if (i->GetHealthPoints() > 0)
			{
				i->SetIsMyTurn(true);
				break;
			}

		}

		for (auto& i : units)
		{
			i->SetHasFinishedTurn(false);
		}
		numberFinished = 0;

		bool enemiesAlive = false;
		for (auto& i : units)
		{
			if (!i->GetIsAlly() && i->GetHealthPoints() > 0)
			{
				enemiesAlive = true;
			}

		}
		bool alliesAlive = false;
		for (auto& i : units)
		{
			if (i->GetIsAlly() && i->GetHealthPoints() > 0)
			{
				alliesAlive = true;
			}

		}
		if (!enemiesAlive)
		{
			app->map->CleanUp();
			app->map->ClearMaps();

			app->sceneManager->currentScene = 6;

			app->sceneManager->current->TransitionToScene(SceneType::GAMEPLAY, TransitionType::ALTERNATING_BARS);
			
		}
		if (!alliesAlive)
		{
			app->map->CleanUp();
			app->map->ClearMaps();

			app->sceneManager->currentScene = 6;

			app->sceneManager->current->TransitionToScene(SceneType::TITLE, TransitionType::ALTERNATING_BARS);

		}

	}



	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		app->moduleCollisions->debug = !app->moduleCollisions->debug;
		if (app->moduleCollisions->debug)
		{
			//CODE OF DEBUG MODE
			LOG("it does enter this scope");
		}
	}

	return ret;
}

void SceneBattle::Draw()
{
	app->map->Draw();

	for (auto& i : units)
	{
		if (i->GetHealthPoints() > 0)
		{
			i->Draw();
		}

	}

	//LOG("Scene battle is drawing");

	////app->render->DrawTexture(backgroundTexture,0,0);
	//map->Draw(colliders,nullptr, nullptr);

	//eastl::list<Player*>::iterator item = playerList.begin();
	//for (; item != playerList.end(); ++item)
	//{
	//	(*item)->Draw(colliders);
	//}
	//
	//eastl::list<Enemy*>::iterator it = enemyList.begin();
	//for (; it != enemyList.end(); ++it)
	//{
	//	(*it)->Draw(colliders);
	//}
	//particles->PostUpdate();
	//battleMenu->Draw(font, colliders);
}

bool SceneBattle::UnLoad()
{
	//font->UnLoad(app->tex);
	//RELEASE(font);

	//battleMenu->UnLoad();
	//RELEASE(battleMenu);

	//eastl::list<Player*>::iterator item = playerList.begin();
	//for (; item != playerList.end(); ++item)
	//{
	//	(*item)->stance = PlayerStance::ROAMING;
	//	(*item)->SetIdleRoaming();
	//}

	//playerList.clear();
	//particles->CleanUp();
	//RELEASE(particles)
	//enemyList.clear();
	//
	//map->CleanUp();
	//RELEASE(map);

	return true;
}