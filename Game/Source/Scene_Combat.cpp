#include "Scene_Combat.h"

#include "Log.h"
#include "Audio.h"

bool Scene_Combat::isReady()
{
	return true;
}

void Scene_Combat::Load(std::string const& path, LookUpXMLNodeFromString const& info, Window_Factory const& windowFactory, std::string const fileToLoad)
{
	// Load Interface
	auto sceneHash = info.find("Combat");
	if (sceneHash == info.end())
	{
		LOG("Title scene not found in XML.");
		return;
	}

	auto scene = sceneHash->second;

	for (auto const& window : scene.children("window"))
	{
		if (auto result = windowFactory.CreateWindow(window.attribute("name").as_string());
			result != nullptr)
		{
			windows.push_back(std::move(result));
		}
	}

	// Load map
	currentMap = "Map2";
	currentMap = fileToLoad;

	if (std::string mapToLoad = currentMap + ".tmx";
		!map.Load(path, mapToLoad))
	{
		LOG("Map %s couldn't be loaded.", mapToLoad);
	}

	LoadEnemies(path, fileToLoad);

	//player.Create();


	std::tuple<iPoint, int, int> vec;
	
	while (std::get<0>(vec).x >= 0)
	{
		using enum UnitType;

		vec = map.eventManager.GetEnemyInfo();

		UnitType type = static_cast<UnitType>(std::get<1>(vec));

		if (std::get<0>(vec).x > 0)
		{
			LOG("this is the dam amount of cats with giant swords that have been loaded from the tiled map edditor");
			switch (type)
			{
			case GUARDIAN:
			{
				//std::unique_ptr<Input> input;
				//input = std::make_unique<Input>();
				std::unique_ptr<Unit> unit;
				unit = std::make_unique<Guardian>();
				unit->Create(std::get<0>(vec));
				unit.get()->velocity = std::get<2>(vec);
				units.push_back(std::move(unit));
				break;
			}
				

			case STRAW:
			{
				std::unique_ptr<Unit> straw;
				straw = std::make_unique<Straw>();
				//straw.Create(std::get<0>(vec));
				straw->Create(std::get<0>(vec));
				straw.get()->velocity = std::get<2>(vec);
				units.push_back(std::move(straw));

				//std::unique_ptr<Unit> unit;
				//unit = std::make_unique<Unit>();
				//unit->Create(std::get<0>(vec));
				////units.push_back(unit);
				break;
			}
			case GATS:
			{
				
				std::unique_ptr<Unit> gats;
				gats = std::make_unique<Gats>();
				gats->Create(std::get<0>(vec));
				gats.get()->velocity = std::get<2>(vec);
				units.push_back(std::move(gats));
				break;
			}
			case CATSKA:
			{
				
				std::unique_ptr<Unit> catska;
				catska = std::make_unique<Catska>();
				catska->Create(std::get<0>(vec));
				catska.get()->velocity = std::get<2>(vec);
				units.push_back(std::move(catska));
				break;
			}
			case LONG_RANGE:
			{

				std::unique_ptr<Unit> longRange;
				longRange = std::make_unique<LongRange>();
				longRange->Create(std::get<0>(vec));
				longRange.get()->velocity = std::get<2>(vec);
				units.push_back(std::move(longRange));
				break;
			}

			case NONE:

				break;
				

			default:
			{
				//std::unique_ptr<Unit> unit;
				//unit = std::make_unique<Unit>();
				//unit->Create(std::get<0>(vec));
				//units.push_back(std::move(unit));
				break;
			}
				
			}

			
			
			//LOG("this is the position of the event: %i, %i", vec.x, vec.y);
		}
	}
	//units[0].swap()
	//units[0].get().
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
	pauseMenu = app->tex->Load("Assets/UI/pixel-simplicity-gui.png");

	music = app->audio->PlayMusic("Assets/Audio/Music/Combat.ogg");

}

void Scene_Combat::CreateUnit()
{
	LOG("but this is a good thing");
}

void Scene_Combat::LoadEnemies(const std::string& directory, const std::string& level)
{
	pugi::xml_document mapFile;
	//LOG("It doesn't crash. Yeayyyyyyy!");

	if (auto result = mapFile.load_file((directory + level).c_str()); !result)
	{
		//LOG("Could not load map xml file %s. pugi error: %s", directory + level, result.description());
		LOG("something bad is happening");
		return;
	}

	pugi::xml_node map = mapFile.child("map");

	if (!map)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		return;
	}

	for (auto const& child : mapFile.child("map"))
	{
		if (StrEquals(child.name(), "objectgroup"))
		{
			LOG("It doesn't crash. Yeayyyyyyy!");
		}
	}



}

void Scene_Combat::Start()
{
}

void Scene_Combat::Draw()
{
	map.Draw();
	//player.Draw();

	for (auto& i : units)
	{
		if (i->GetHealthPoints() > 0)
		{
			i->Draw();
		}
		
	}

	for (auto const& elem : windows)
	{
		elem->Draw();
	}
}

int Scene_Combat::Update()
{
	// Interface Logic
	for (auto const& elem : windows)
	{
		if (auto result = elem->Update();
			result != 0)
			return result;
	}

	//auto playerAction = player.HandleInput();
	//
	//
	//using PA = Player::PlayerAction::Action;
	using UA = Unit::PlayerAction::Action;
	//
	//if ((playerAction.action & PA::MOVE) == PA::MOVE)
	//{
	//	if (map.IsWalkable(playerAction.destinationTile))
	//	{
	//		player.StartAction(playerAction);
	//
	//	}
	//
	//}
	

	//for (auto& i : units)
	//{
	//	
	//	auto unitAction = i.HandleInput();
	//
	//	if ((unitAction.action & UA::MOVE) == UA::MOVE)
	//	{
	//		
	//		if (map.IsWalkable(unitAction.destinationTile))
	//		{
	//			
	//			{
	//				i.StartAction(unitAction);
	//			}
	//		}
	//	}
	//	i.Update();
	//}


	//bool hasTurnEnded = false;                  // if done with a vector
	//auto unitAction = units[turn].HandleInput();
	//if ((unitAction.action & UA::MOVE) == UA::MOVE)
	//{
	//	if (map.IsWalkable(unitAction.destinationTile))
	//	{
	//		LOG("the turn is: %i", turn);
	//		units[turn].StartAction(unitAction);
	//		hasTurnEnded = true;
	//	}
	//}
	////LOG("the update is is: %i", turn);
	//units[turn].Update();
	//
	//if (hasTurnEnded)
	//	turn++;
	//if (turn >= units.size())
	//{
	//	turn = 0;
	//}


	//bool hasTurnEnded = false;
	//auto unitAction = units.begin()->HandleInput();      // if done with a list
	//if ((unitAction.action & UA::MOVE) == UA::MOVE)
	//{
	//	if (map.IsWalkable(unitAction.destinationTile))
	//	{
	//		LOG("the turn is: %i", turn);
	//		units.begin()->StartAction(unitAction);
	//		hasTurnEnded = true;
	//	}
	//}
	//units.begin()->Update();
	////units.front();
	////units.
	////Unit helper = units[units.size() - 1];
	//if (hasTurnEnded)
	//{
	//	units.push_back(units.front());
	//	units.pop_front();
	//}


	bool noUnitHasActed = true;

	for (auto& i : units)
	{
		
		if (i->GetIsMyTurn() && !i->GetHasFinishedTurn() && i->GetHealthPoints() > 0)
		{
			noUnitHasActed = false;
			
			auto unitAction = i->HandleInput();

			if ((unitAction.action) == UA::MOVE)
			{
				
				if (map.IsWalkable(unitAction.destinationTile))
				{
					i->StartAction(unitAction);
				}
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
							unit->DealDamage(10);
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
							unit->DealDamage(10);
							i->StartAction(unitAction);
							LOG("the health points that this unit has after the attack that you have thrown to it are the number that you are going to see: %i", unit->GetHealthPoints());
							i->SetHasFinishedTurn(true);
							
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
							unit->DealDamage(10);
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
							unit->DealDamage(10);
							i->StartAction(unitAction);
							LOG("the health points that this unit has after the attack that you have thrown to it are the number that you are going to see: %i", unit->GetHealthPoints());
							i->SetHasFinishedTurn(true);
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
		if (!enemiesAlive)
		{
			return 5;
		}
		
	}




	//player.Update();

	//std::vector<Event_Base> vec = map.eventManager.GetEventVector();





	if (app->input->GetKey(SDL_SCANCODE_Q) == KeyState::KEY_UP)
	{
		return 6;
	}

	return 0;
}

int Scene_Combat::CheckNextScene(int ret)
{
	using enum SceneType;
	using enum KeyState;
	if (app->input->GetKey(SDL_SCANCODE_Q) == KEY_UP)
	{
		return static_cast<int>(SceneType::TITLESCENE);
	}

	return ret;
}

void Scene_Combat::DrawPause()
{
	app->render->DrawTexture(DrawParameters(pauseMenu, { 100, 100 }));
}
