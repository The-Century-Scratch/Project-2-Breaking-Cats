#include "Scene_Shop.h"
#include "Event_Base.h"
#include "DialogueManager.h"
#include "Audio.h"

#include "Log.h"

bool Scene_Shop::isReady()
{
	return true;
}
int Scene_Shop::Test()
{
	LOG("the test is working");
	return 0;
}

void Scene_Shop::Load(std::string const& path, LookUpXMLNodeFromString const& info, Window_Factory const& windowFactory, std::string const fileToLoad)
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
	pauseMenu = app->tex->Load("Assets/UI/pixel-simplicity-gui.png");

	music = app->audio->PlayMusic("Assets/Audio/Music/ShopTheme.ogg");
}

void Scene_Shop::Start()
{
}

void Scene_Shop::Draw()
{
	map.Draw();
	player.Draw();

	for (auto const& elem : windows)
	{
		elem->Draw();
	}
}

int Scene_Shop::Update()
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
			LOG("Is event funciona :)"); //TODO el event tp no lo pilla
			ret = player.StartAction(playerAction, map.getEvent(playerAction.destinationTile, player.facing));
		}

		if (map.getEvent(playerAction.destinationTile, player.facing).commonData.type == EventType::DIALOGUE)
		{
			LOG("npc dialogue started %i", map.getEvent(playerAction.destinationTile, player.facing).dialogueData.script);
			switch (map.getEvent(playerAction.destinationTile, player.facing).dialogueData.script)
			{
			case 1:

				app->Dialogue.get()->dialogueEnabled = true;
				app->Dialogue.get()->miceyDialog = true;
				app->Dialogue.get()->sentenceQueue = app->Dialogue.get()->M_greeting.sentenceList;
				app->Dialogue.get()->micey = M_GREETING;
				break;
			case 2:
				app->Dialogue.get()->dialogueEnabled = true;
				app->Dialogue.get()->contrabandistDialog = true;
				if (app->Dialogue.get()->contrabandist == C_GREETING)
				{
					app->Dialogue.get()->sentenceQueue = app->Dialogue.get()->C_greeting.sentenceList;
				}
				break;
			case 3:
				app->Dialogue.get()->dialogueEnabled = true;
				app->Dialogue.get()->wardDialog = true;
				if (app->Dialogue.get()->ward == W_CONVERSATION)
				{
					app->Dialogue.get()->sentenceQueue = app->Dialogue.get()->W_conversation.sentenceList;
				}
				break;
			case 4:
				app->Dialogue.get()->dialogueEnabled = true;
				app->Dialogue.get()->signDialog = true;
				app->Dialogue.get()->sentenceQueue = app->Dialogue.get()->S_text.sentenceList;
				break;
			default:
				break;
			}
		}
	}
	
	player.Update();

	//std::vector<Event_Base> vec = map.eventManager.GetEventVector();
	//std::string vec = map.eventManager.GetEventVector();
	
	//LOG("this is the name of the event: %s", vec);
	return CheckNextScene(ret);
}

int Scene_Shop::CheckNextScene(int ret)
{
	using enum SceneType;
	using enum KeyState;
	if (app->input->GetKey(SDL_SCANCODE_Q) == KEY_UP)
	{
		return static_cast<int>(SceneType::TITLESCENE);
	}

	return ret;
}

void Scene_Shop::DrawPause()
{
	app->render->DrawTexture(DrawParameters(pauseMenu, { 100, 100 }));
}
