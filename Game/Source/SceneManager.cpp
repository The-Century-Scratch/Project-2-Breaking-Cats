#include "App.h"
#include "SceneManager.h"
#include "Input.h"

#include "Scene_Map.h"
#include "Scene_Combat.h"
#include "Scene_Title.h"
#include "Scene_Vertical.h"

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



	// STATE MACHINE DE LA FUNCIONALIDAD DE LOS BOTONES
	using enum SceneType;
	switch (static_cast<SceneType>(currentScene->Update()))
	{
	case NEWGAME:
		nextScene = std::make_unique<Scene_Map>();
		actualScene = NEWGAME;
		break;
	case CONTINUE:
		break;
	case EXIT:
		return false;
		break;
	case COMBAT:
		nextScene = std::make_unique<Scene_Combat>();
		actualScene = COMBAT;
		break;
	case VERTICAL:
		nextScene = std::make_unique<Scene_Vertical>();
		actualScene = VERTICAL;
		break;
	case TITLESCENE:
		nextScene = std::make_unique<Scene_Title>();
		actualScene = TITLESCENE;
		break;
	default:
		break;
	}


	currentScene->Draw();
	
	return true;
}

// Called each loop iteration
bool SceneManager::PostUpdate()
{
	using enum SceneType; using enum KeyState;

	if (nextScene && nextScene->isReady())
	{
		switch (actualScene)
		{
		case NEWGAME:
			nextScene.get()->Load(assetPath + "Maps/", mapInfo, *windowFactory);
			break;
		case COMBAT:
			nextScene.get()->Load(assetPath + "Maps/", mapInfo, *windowFactory, "Map2");
			break;
		case VERTICAL:
			nextScene.get()->Load(assetPath + "Maps/", sceneInfo, *windowFactory, "Shop");
			break;
		case TITLESCENE:
			nextScene.get()->Load(assetPath + "UI/", sceneInfo, *windowFactory);
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
