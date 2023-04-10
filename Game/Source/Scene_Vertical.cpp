#include "Scene_Vertical.h"

#include "Log.h"

bool Scene_Vertical::isReady()
{
	return true;
}
int Scene_Vertical::Test()
{
	LOG("the test is working");
	return 0;
}

void Scene_Vertical::Load(std::string const& path, LookUpXMLNodeFromString const& info, Window_Factory const& windowFactory, std::string const fileToLoad)
{
	// Load map
	currentMap = "Vertical";

	if (std::string mapToLoad = currentMap + ".tmx";
		!map.Load(path, mapToLoad))
	{
		LOG("Map %s couldn't be loaded.", mapToLoad);
	}
	Test();

	player.Create();
}

void Scene_Vertical::Start()
{
}

void Scene_Vertical::Draw()
{
	map.Draw();
	player.Draw();
}

int Scene_Vertical::Update()
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
		return 6;
	}

	return 0;
}

int Scene_Vertical::CheckNextScene()
{
	return 0;
}
