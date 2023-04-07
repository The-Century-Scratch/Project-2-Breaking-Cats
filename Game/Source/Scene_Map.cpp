#include "Scene_Map.h"

#include "Log.h"

bool Scene_Map::isReady()
{
	return true;
}
int Scene_Map::Test()
{
	LOG("the test is working");
	return 0;
}

void Scene_Map::Load(std::string const& path, LookUpXMLNodeFromString const& info, Window_Factory const& windowFactory, std::string const fileToLoad)
{
	// Load map
	currentMap = "Base";

	if (std::string mapToLoad = currentMap + ".tmx";
		!map.Load(path, mapToLoad))
	{
		LOG("Map %s couldn't be loaded.", mapToLoad);
	}
	Test();

	player.Create();
}

void Scene_Map::Start()
{
}

void Scene_Map::Draw()
{
	map.Draw();
	player.Draw();
}

int Scene_Map::Update()
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
	//std::string vec = map.eventManager.GetEventVector();
	
	//LOG("this is the name of the event: %s", vec);

	if (app->input->GetKey(SDL_SCANCODE_Q) == KeyState::KEY_UP)
	{
		return 5;
	}

	return 0;
}

int Scene_Map::CheckNextScene()
{
	return 0;
}
