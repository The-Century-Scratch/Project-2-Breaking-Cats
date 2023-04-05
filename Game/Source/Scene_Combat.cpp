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
}

int Scene_Combat::Update()
{
	auto playerAction = player.HandleInput();

	using PA = Player::PlayerAction::Action;

	if ((playerAction.action & PA::MOVE) == PA::MOVE)
	{
		if (map.IsWalkable(playerAction.destinationTile))
		{
			player.StartAction(playerAction);
		}
	}

	player.Update();
	//std::vector<Event_Base> vec = map.eventManager.GetEventVector();
	iPoint vec = map.eventManager.GetEventVector();

	if (vec.x > 0)
	{
		LOG("this is the position of the event: %i, %i", vec.x, vec.y);
	}
	
	
	

	return 0;
}

int Scene_Combat::CheckNextScene()
{
	return 0;
}
