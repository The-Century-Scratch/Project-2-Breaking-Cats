#include "App.h"
#include "SceneManager.h"
#include "Input.h"

#include "Scene_Map.h"
#include "Scene_Title.h"

#include "Defs.h"
#include "Log.h"

#include <format>

SceneManager::SceneManager() : Module()
{
	name = "scene";
}

// Destructor
SceneManager::~SceneManager() = default;

// Called before render is available
bool SceneManager::Awake(pugi::xml_node& config)
{
	LOG("Loading SceneManager");

	assetPath = config.child("general").attribute("assetpath").as_string();

	windowFactory = std::make_unique<Window_Factory>(config);

	for (auto const& node : config.child("scene_info").children("scene"))
	{
		sceneInfo[node.attribute("name").as_string()] = node;
	}
	for (auto const& node : config.child("map_info").children("map"))
	{
		mapInfo[node.attribute("name").as_string()] = node;
	}

	currentScene = std::make_unique<Scene_Title>();
	return true;
}

// Called before the first frame
bool SceneManager::Start()
{
	currentScene.get()->Load(assetPath + "UI/", sceneInfo, *windowFactory);

	return true;
}

// Called each loop iteration
bool SceneManager::PreUpdate()
{
	return true;
}


bool SceneManager::Pause(int phase)
{
	// Request App to Load / Save when pressing the keys F5 (save) / F6 (load)
	if (app->input->GetKey(SDL_SCANCODE_F5) == KeyState::KEY_DOWN)
		app->SaveGameRequest();

	if (app->input->GetKey(SDL_SCANCODE_F6) == KeyState::KEY_DOWN)
		app->LoadGameRequest();

	return true;
}

// Called each loop iteration
bool SceneManager::Update(float dt)
{
	// Request App to Load / Save when pressing the keys F5 (save) / F6 (load)
	if (app->input->GetKey(SDL_SCANCODE_F5) == KeyState::KEY_DOWN)
		app->SaveGameRequest();

	if (app->input->GetKey(SDL_SCANCODE_F6) == KeyState::KEY_DOWN)
		app->LoadGameRequest();

	currentScene->Draw();



	// STATE MACHINE DE LA FUNCIONALIDAD DE LOS BOTONES
	using enum SceneType;
	switch (currentScene->Update())
	{
	case 1:
		//if (CurrentlyMainMenu)
		//    nextScene = std::make_unique<Scene_Map>();
		//else
		//    nextScene = std::make_unique<Scene_Title>();

		switch (actualScene)
		{
		case TITLESCENE:
			nextScene = std::make_unique<Scene_Map>();
			break;
		case MAP1:
			nextScene = std::make_unique<Scene_Title>();
			break;
		case UNKNOWN:
			break;
		default:
			break;
		}

		break;
	case 2:
		break;
	case 3:
		return false;
		break;

	default:
		break;
	}
	
	return true;
}

// Called each loop iteration
bool SceneManager::PostUpdate()
{
	if (nextScene && nextScene->isReady())
	{
		/*if(CurrentlyMainMenu)
			nextScene.get()->Load(assetPath + "Maps/", mapInfo, *windowFactory);
		else
			nextScene.get()->Load(assetPath + "UI/", sceneInfo, *windowFactory);

		CurrentlyMainMenu = !CurrentlyMainMenu;*/
		using enum SceneType; using enum KeyState;

		switch (actualScene)
		{
		case TITLESCENE:
			nextScene.get()->Load(assetPath + "Maps/", mapInfo, *windowFactory);
			actualScene = MAP1;
			if (app->input->GetKey(SDL_SCANCODE_O) == KEY_REPEAT)
			{

			}
			break;
		case MAP1:
			nextScene.get()->Load(assetPath + "UI/", sceneInfo, *windowFactory);
			actualScene = TITLESCENE;
			if (app->input->GetKey(SDL_SCANCODE_P) == KEY_REPEAT)
			{

			}
			break;
		case UNKNOWN:
			break;
		default:
			break;
		}

		currentScene = std::move(nextScene);
	}


	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KeyState::KEY_DOWN)
		return false;
	
	return true;
}

// Called before quitting
bool SceneManager::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

bool SceneManager::HasSaveData() const
{
	return true;
}

bool SceneManager::LoadState(pugi::xml_node const &data)
{
	return false;
}

pugi::xml_node SceneManager::SaveState(pugi::xml_node const &data) const
{
	std::string saveData2 = "<{} {}=\"{}\"/>\n";
	std::string saveOpenData2 = "<{} {}=\"{}\">\n";
	std::string saveData4 = "<{} {}=\"{}\" {}=\"{}\"/>\n";
	std::string saveOpenData4 = "<{} {}=\"{}\" {}=\"{}\">\n";
	std::string saveData6 = "<{} {}=\"{}\" {}=\"{}\" {}=\"{}\"/>\n";
	std::string saveData6OneFloat = "<{} {}=\"{}\" {}=\"{}\" {}=\"{}\" {}=\"{:.2f}\"/>\n";
	std::string saveData6OneInt = "<{} {}=\"{}\" {}=\"{:.2f}\" {}=\"{:.2f}\" {}=\"{:.2f}\"/>\n";
	std::string saveFloatData = "<{} {}=\"{:.2f}\" {}=\"{:.2f}\"/>\n";
	std::string dataToSave = "<scene>\n";
	dataToSave += "</scene>";

	app->AppendFragment(data, dataToSave.c_str());

	return data;
}
