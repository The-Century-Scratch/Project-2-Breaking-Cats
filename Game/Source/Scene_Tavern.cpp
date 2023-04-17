#include "Scene_Tavern.h"
#include "Event_Base.h"

#include "Log.h"

bool Scene_Tavern::isReady()
{
	return true;
}
int Scene_Tavern::Test()
{
	LOG("the test is working");
	return 0;
}

void Scene_Tavern::Load(std::string const& path, LookUpXMLNodeFromString const& info, Window_Factory const& windowFactory, std::string const fileToLoad)
{
	// Load Interface
	auto sceneHash = info.find("Vertical");
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

	if (std::string mapToLoad = fileToLoad + ".tmx";
		!map.Load(path, mapToLoad))
	{
		LOG("Map %s couldn't be loaded.", mapToLoad);
	}
	Test();

	player.Create();
	pauseMenu = app->tex->Load("Assets/UI/pixel-simplicity-gui.png");
}

void Scene_Tavern::Start()
{
}

void Scene_Tavern::Draw()
{
	map.Draw();
	player.Draw();

	for (auto const& elem : windows)
	{
		elem->Draw();
	}
}

int Scene_Tavern::Update()
{
	int ret = 0;
	// Interface Logic
	for (auto const& elem : windows)
	{
		if (auto result = elem->Update();
			result != 0)
			return result;
	}


	// Player 
	auto playerAction = player.HandleInput();

	using PA = Player::PlayerAction::Action;

	if ((playerAction.action & PA::MOVE) == PA::MOVE)
	{
		player.FaceTo(playerAction.willFace);
		if (map.IsWalkable(playerAction.destinationTile))
		{
			player.StartAction(playerAction);
		}
	}

	if ((playerAction.action & PA::INTERACT) == PA::INTERACT)
	{
		if (map.IsEvent(playerAction.destinationTile, player.facing))
		{
			LOG("Is event funciona :)");
			ret = player.StartAction(playerAction, map.getEvent(playerAction.destinationTile, player.facing));
		}
	}
	
	player.Update();

	//std::vector<Event_Base> vec = map.eventManager.GetEventVector();
	//std::string vec = map.eventManager.GetEventVector();
	
	//LOG("this is the name of the event: %s", vec);
	return CheckNextScene(ret);
}

int Scene_Tavern::CheckNextScene(int ret)
{
	using enum SceneType;
	using enum KeyState;
	if (app->input->GetKey(SDL_SCANCODE_Q) == KEY_UP)
	{
		return static_cast<int>(SceneType::TITLESCENE);
	}

	return ret;
}

void Scene_Tavern::DrawPause()
{
	app->render->DrawTexture(DrawParameters(pauseMenu, { 100, 100 }));
}
