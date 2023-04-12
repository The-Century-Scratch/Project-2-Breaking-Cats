#include "Scene_Combat.h"

#include "Log.h"

bool Scene_Combat::isReady()
{
	return true;
}

void Scene_Combat::Load(std::string const& path, LookUpXMLNodeFromString const& info, Window_Factory const& windowFactory, std::string const fileToLoad)
{
	// Load map
	currentMap = "Map2";
	currentMap = fileToLoad;

	if (std::string mapToLoad = currentMap + ".tmx";
		!map.Load(path, mapToLoad))
	{
		LOG("Map %s couldn't be loaded.", mapToLoad);
	}

	LoadEnemies(path, fileToLoad);

	player.Create();


	std::tuple<iPoint, int> vec;
	
	while (std::get<0>(vec).x >= 0)
	{
		using enum UnitType;

		vec = map.eventManager.GetEnemyInfo();

		UnitType type = static_cast<UnitType>(std::get<1>(vec));

		if (std::get<0>(vec).x > 0)
		{
			switch (type)
			{
			case GUARDIAN:
			{
				Unit unit;
				unit.Create(std::get<0>(vec));
				units.push_back(unit);
				break;
			}
				

			case STRAW:
			{
				Straw unit;
				unit.Create(std::get<0>(vec));
				units.push_back(unit);
				break;
			}
				

			default:
			{
				Unit unit;

				unit.Create(std::get<0>(vec));
				units.push_back(unit);
				break;
			}
				
			}

			
			
			//LOG("this is the position of the event: %i, %i", vec.x, vec.y);
		}
	}


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
	player.Draw();

	for (auto i : units)
	{
		i.Draw();
	}
}

int Scene_Combat::Update()
{
	auto playerAction = player.HandleInput();


	using PA = Player::PlayerAction::Action;
	using UA = Unit::PlayerAction::Action;

	if ((playerAction.action & PA::MOVE) == PA::MOVE)
	{
		if (map.IsWalkable(playerAction.destinationTile))
		{
			player.StartAction(playerAction);

		}

	}

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

		if (i.GetIsMyTurn() && !i.GetHasFinishedTurn())
		{
			noUnitHasActed = false;

			auto unitAction = i.HandleInput();

			if ((unitAction.action & UA::MOVE) == UA::MOVE)
			{

				if (map.IsWalkable(unitAction.destinationTile))
				{

					{
						i.StartAction(unitAction);
					}
				}
			}

		}
		i.Update();

		if (i.GetHasFinishedTurn() && i.GetIsMyTurn())
		{
			i.SetIsMyTurn(false);
			numberFinished++; // that is a variable    // all of this check first if this variable is bigger thatn the allowed number for the vector

			if (numberFinished < units.size())
			{
				units[numberFinished].SetIsMyTurn(true);
			}


			//noUnitHasActed = false;
		}

	}

	if (noUnitHasActed)
	{
		units[0].SetIsMyTurn(true);
		for (auto& i : units)
		{
			i.SetHasFinishedTurn(false);
		}
		numberFinished = 0;
	}




	player.Update();

	//std::vector<Event_Base> vec = map.eventManager.GetEventVector();





	if (app->input->GetKey(SDL_SCANCODE_Q) == KeyState::KEY_UP)
	{
		return 6;
	}

	return 0;
}

int Scene_Combat::CheckNextScene()
{
	return 0;
}
