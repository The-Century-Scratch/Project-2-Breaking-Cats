#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "SceneBattle.h"
#include "SceneGameplay.h"
#include "SceneManager.h"
#include "Map.h"
#include "Catska.h"
#include "Gats.h"
#include "Guardian.h"
#include "LongRange.h"
#include "Straw.h"
#include "Player.h"
#include "Audio.h"
#include <time.h>
#include "ModuleCollisions.h"
#include "Debug.h"
#include "Inventory.h"
#include "Hud.h"

SceneBattle::SceneBattle()
{
	name.Create("scenebattle");
	gridSystem = eastl::make_unique<GridSystem>();
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

	combatTheme = config.child("music").attribute("path").as_string();



	for (pugi::xml_node nodeUnit = config.child("gats");
		nodeUnit; nodeUnit = nodeUnit.next_sibling("gats"))
	{

		eastl::unique_ptr<Unit> gats;
		gats = eastl::make_unique<Gats>();
		gats.get()->velocity = nodeUnit.attribute("velocity").as_int();
		gats.get()->parameters = nodeUnit;
		gats.get()->Create({ nodeUnit.attribute("x").as_int(), nodeUnit.attribute("y").as_int() });
		gridSystem->LoadUnitData(gats.get());
		units.push_back(eastl::move(gats));
	}
	for (pugi::xml_node nodeUnit = config.child("catska");
		nodeUnit; nodeUnit = nodeUnit.next_sibling("catska"))
	{

		eastl::unique_ptr<Unit> catska;
		catska = eastl::make_unique<Catska>();
		catska.get()->velocity = nodeUnit.attribute("velocity").as_int();
		catska.get()->parameters = nodeUnit;
		catska.get()->Create({ nodeUnit.attribute("x").as_int(), nodeUnit.attribute("y").as_int() });
		gridSystem->LoadUnitData(catska.get());
		units.push_back(eastl::move(catska));
	}
	for (pugi::xml_node nodeUnit = config.child("guardian");
		nodeUnit; nodeUnit = nodeUnit.next_sibling("guardian"))
	{

		eastl::unique_ptr<Unit> unit;
		unit = eastl::make_unique<Guardian>();
		unit.get()->velocity = nodeUnit.attribute("velocity").as_int();
		unit.get()->parameters = nodeUnit;
		unit.get()->Create({ nodeUnit.attribute("x").as_int(), nodeUnit.attribute("y").as_int() });
		gridSystem->LoadUnitData(unit.get());
		units.push_back(eastl::move(unit));
	}
	for (pugi::xml_node nodeUnit = config.child("longrange");
		nodeUnit; nodeUnit = nodeUnit.next_sibling("longrange"))
	{

		eastl::unique_ptr<Unit> unit;
		unit = eastl::make_unique<LongRange>();
		unit.get()->velocity = nodeUnit.attribute("velocity").as_int();
		unit.get()->parameters = nodeUnit;
		unit.get()->Create({ nodeUnit.attribute("x").as_int(), nodeUnit.attribute("y").as_int() });
		gridSystem->LoadUnitData(unit.get());
		units.push_back(eastl::move(unit));
	}
	for (pugi::xml_node nodeUnit = config.child("straw");
		nodeUnit; nodeUnit = nodeUnit.next_sibling("straw"))
	{

		eastl::unique_ptr<Unit> unit;
		unit = eastl::make_unique<Straw>();
		unit.get()->velocity = nodeUnit.attribute("velocity").as_int();
		unit.get()->parameters = nodeUnit;
		unit.get()->Create({ nodeUnit.attribute("x").as_int(), nodeUnit.attribute("y").as_int() });
		gridSystem->LoadUnitData(unit.get());
		units.push_back(eastl::move(unit));
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

	
	//app->render->camera.x = 0;
	//app->render->camera.y = 0;


	app->audio->PlayMusic(combatTheme.GetString());
	turnTimer = 0;
	gridSystem->Start();

	return true;
}

bool SceneBattle::Update(float dt)
{
	bool ret = true;
	//LOG("Scene battle is updating");

	using UA = Unit::PlayerAction::Action;

	//TODO: later delete this is just a quick thing for the alpha
	if (app->input->GetKey(SDL_SCANCODE_I) == KeyState::KEY_DOWN)
	{
		app->inventory->isActivated = !app->inventory->isActivated;
		app->audio->PlayFx(app->hud->swapscenesfx);
	}

	bool noUnitHasActed = true;
	turnTimer++;
	if (turnTimer > 20)
		turnTimer = 11;

	for (auto& i : units)
	{
		if (i->GetIsMyTurn() && !i->GetHasFinishedTurn() && i->GetHealthPoints() > 0)
		{
			noUnitHasActed = false;

			Unit::PlayerAction unitAction = i->HandleInput();
			if(turnTimer > 10)
			{
				if (unitAction.action != UA::NONE)
				{
					turnTimer = 0;
				}

				switch (unitAction.action)
				{
				case UA::NONE:
					break;
				case UA::MOVE:
					gridSystem->showArea = false;

					if (gridSystem->isWalkable(unitAction.destinationTile))
					{
						gridSystem->move(i->position, unitAction.destinationTile);
						i->StartAction(unitAction);
					}
					break;
				case UA::PREPARE_DASH:
					gridSystem->showArea = !gridSystem->showArea;
					if (gridSystem->showArea)
						gridSystem->currentAction = unitAction;

					i->StartAction(unitAction);
					break;
				case UA::ATTACK:
					gridSystem->showArea = !gridSystem->showArea;
					if (gridSystem->showArea)
						gridSystem->currentAction = unitAction;
					/*for (auto& unit : units)
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
					}*/
					break;
				case UA::ATTACK_AND_HEAL_WITH_KILL:
					gridSystem->showArea = !gridSystem->showArea;
					if(gridSystem->showArea)
						gridSystem->currentAction = unitAction;
					/*for (auto& unit : units)
					{
						if (!unit.get()->GetIsAlly() && unit->GetHealthPoints() > 0)
						{
							iPoint  unitPos = unit.get()->GetPosition();
							if (i->position.DistanceTo(unitPos) < 18)
							{
								unit->DealDamage(i->GetDamage());
								if (unit->GetHealthPoints() <= 0)
								{
									i->StartAction(unitAction);
								}
								else
								{
									unitAction.action = UA::ATTACK;
									i->StartAction(unitAction);
								}

								LOG("the health points that this unit has after the attack that you have thrown to it are the number that you are going to see: %i", unit->GetHealthPoints());
								i->SetHasFinishedTurn(true);

							}
						}
					}*/
					break;
				case UA::ATTACK_LONG_RANGE:
					gridSystem->showArea = !gridSystem->showArea;
					if (gridSystem->showArea)
						gridSystem->currentAction = unitAction;
					/*for (auto& unit : units)
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
								if (!app->inventory->GetBulletPenetration())
								{
									break;
								}


							}
						}
					}*/
					break;
				case UA::ATTACK_TO_PLAYER:
					gridSystem->showArea = false;

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
					break;
				case UA::ATTACK_TO_PLAYER_LONG_RANGE:
					gridSystem->showArea = false;

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
					break;
				default:
					break;
				}
			}
			if (gridSystem->showArea)
			{
				gridSystem->showActionArea();
				if (gridSystem->Update() && gridSystem->AreaIsClicked())
				{
					eastl::vector<iPoint> hitsMade = gridSystem->getHitsPosition();

					gridSystem->showArea = !gridSystem->showArea;

					if (!hitsMade.empty())
					{
						for (eastl::unique_ptr<Unit>& unit : units)
						{
							for (iPoint& hit : hitsMade)
							{
								switch (gridSystem->currentAction.action)
								{
								case UA::NONE:
									break;
								case UA::ATTACK:
									break;
								case UA::ATTACK_LONG_RANGE:
									break;
								case UA::PREPARE_DASH:
									break;
								case UA::ATTACK_AND_HEAL_WITH_KILL:
									break;
								default:
									break;
								}
							}
						}
						i->SetHasFinishedTurn(true);
					}
				}
			}
		}
		i->Update();

		if (i->GetHasFinishedTurn() && i->GetIsMyTurn())
		{
			i->SetIsMyTurn(false);
			turn++; // that is a variable    // all of this check first if this variable is bigger thatn the allowed number for the vector

			if (turn < units.size())
			{
				units[turn]->SetIsMyTurn(true);
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
			gridSystem->removeUnit(i->position);
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
		turn = 0;

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

			app->sceneManager->current->TransitionToScene(SceneType::ENDING, TransitionType::ALTERNATING_BARS, true);
			
		}
		if (!alliesAlive)
		{
			app->map->CleanUp();
			app->map->ClearMaps();

			app->sceneManager->currentScene = 6;

			app->sceneManager->current->TransitionToScene(SceneType::ENDING, TransitionType::ALTERNATING_BARS);

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


	gridSystem->Update();

	return ret;
}

void SceneBattle::Draw()
{
	app->map->Draw();

	gridSystem->Draw();

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
	gridSystem->CleanUp();
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