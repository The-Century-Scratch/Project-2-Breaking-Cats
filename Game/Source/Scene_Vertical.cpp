#include "Scene_Vertical.h"
#include "Event_Base.h"
#include "DialogueManager.h"

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
			player.StartAction(playerAction, map.getEvent(playerAction.destinationTile, player.facing));
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

	if (app->input->GetKey(SDL_SCANCODE_1) == KeyState::KEY_UP)
	{
		app->Dialogue.get()->dialogueEnabled = true;
		app->Dialogue.get()->miceyDialog = true;
		app->Dialogue.get()->sentenceQueue = app->Dialogue.get()->M_greeting.sentenceList;
		app->Dialogue.get()->micey = M_GREETING;
	}

	if (app->input->GetKey(SDL_SCANCODE_2) == KeyState::KEY_UP)
	{
		app->Dialogue.get()->dialogueEnabled = true;
		app->Dialogue.get()->contrabandistDialog = true;
		if (app->Dialogue.get()->contrabandist == C_GREETING) {
			app->Dialogue.get()->sentenceQueue = app->Dialogue.get()->C_greeting.sentenceList;
		}

	}

	if (app->input->GetKey(SDL_SCANCODE_3) == KeyState::KEY_UP)
	{
		app->Dialogue.get()->dialogueEnabled = true;
		app->Dialogue.get()->wardDialog = true;
		if (app->Dialogue.get()->ward == W_CONVERSATION) {
			app->Dialogue.get()->sentenceQueue = app->Dialogue.get()->W_conversation.sentenceList;
		}
	}

	if (app->input->GetKey(SDL_SCANCODE_4) == KeyState::KEY_UP)
	{
		app->Dialogue.get()->dialogueEnabled = true;
		app->Dialogue.get()->signDialog = true;
		app->Dialogue.get()->sentenceQueue = app->Dialogue.get()->S_text.sentenceList;
	}

	return 0;
}

int Scene_Vertical::CheckNextScene()
{
	return 0;
}
