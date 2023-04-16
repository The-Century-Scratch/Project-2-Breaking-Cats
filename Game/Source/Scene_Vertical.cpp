#include "Scene_Vertical.h"
#include "Event_Base.h"

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
	currentMap = fileToLoad;

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

	for (auto const& elem : windows)
	{
		elem->Draw();
	}
}

int Scene_Vertical::Update()
{
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
		LOG("it is attacking right in this very moment in which you have pressed the key of your keyboard with the symbol E");
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
			LOG("Is event funciona :)"); //TODO el event tp no lo pilla
			int ret = 0;
			player.StartAction(playerAction, map.getEvent(playerAction.destinationTile, player.facing));
		}
	}
	
	player.Update();

	//std::vector<Event_Base> vec = map.eventManager.GetEventVector();
	//std::string vec = map.eventManager.GetEventVector();
	
	//LOG("this is the name of the event: %s", vec);
	return CheckNextScene();
}

int Scene_Vertical::CheckNextScene()
{
	using enum SceneType;
	using enum KeyState;
	if (app->input->GetKey(SDL_SCANCODE_Q) == KEY_UP)
	{
		return static_cast<int>(SceneType::TITLESCENE);
	}

	return 0;
}
