#include "App.h"

#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Audio.h"
#include "Debug.h"
#include "ModuleCollisions.h"
#include "Hud.h"
#include "Window.h"
#include "SceneGameplay.h"
#include "SceneManager.h"
#include "SceneBattle.h"
#include "EntityManager.h"
#include "Map.h"
#include "Inventory.h"
#include "Easings.h"
#include "SceneEnding.h"

#include "Log.h"

SceneEnding::SceneEnding(bool win)
{
	//bg = nullptr;
	//font = nullptr;
	//hasWin = win;

	//if (hasWin) app->audio->PlayMusic("Audio/Music/win_theme.ogg", true);
	//else app->audio->PlayMusic("Audio/Music/gameover_theme.ogg", true);

	//easingUp = new Easing();
	//easingDown = new Easing();

	//easingUp->easingsActivated = true;
	//easingUp->currentIteration = 0;
	//easingUp->totalIterations = 75;
	//easingUp->initialPos = 200;
	//easingUp->deltaPos = 50;

	//easingDown->easingsActivated = false;
	//easingDown->currentIteration = 0;
	//easingDown->totalIterations = 75;
	//easingDown->initialPos = 250;
	//easingDown->deltaPos = -50;
	//
	//easingPosition = 300;

	//showColliders = false;

	//// Font
	//font = new Font(app, "Font/font3.xml", app->tex);

	//menu = new EndingMenu(font, this);
}

bool SceneEnding::Load()
{
	LOG("Loading Scene Ending");
	bool ret = true;

	if (hasWin) bg = app->tex->Load("Assets/Textures/victory.png");
	else bg = app->tex->Load("Assets/Textures/defeat.png");

	return ret;
}

bool SceneEnding::Update(float dt)
{
	bool ret = true;
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KeyState::KEY_DOWN)
	{

		app->map->CleanUp();
		app->map->ClearMaps();

		app->sceneManager->currentScene = 0; //TODO: after finishing the loading of enemies from maps, make this the way to randomly select which map to go to

		app->render->camera.x = 0;
		app->render->camera.y = 0;



		app->sceneManager->current->TransitionToScene(SceneType::TITLE, TransitionType::ALTERNATING_BARS);

	}
	//if (easingUp->easingsActivated)
	//{
	//	easingPosition = easingUp->exponentialEaseInOut(easingUp->currentIteration, easingUp->initialPos, easingUp->deltaPos, easingUp->totalIterations);
	//	if (easingUp->currentIteration < easingUp->totalIterations)
	//	{
	//		easingUp->currentIteration++;
	//	}
	//	else
	//	{
	//		easingUp->currentIteration = 0;
	//		easingUp->easingsActivated = false;
	//		easingDown->easingsActivated = true;
	//	}
	//}

	//if (easingDown->easingsActivated)
	//{
	//	easingPosition = easingDown->exponentialEaseInOut(easingDown->currentIteration, easingDown->initialPos, easingDown->deltaPos, easingDown->totalIterations);
	//	if (easingDown->currentIteration < easingDown->totalIterations)
	//	{
	//		easingDown->currentIteration++;
	//	}
	//	else
	//	{
	//		easingDown->currentIteration = 0;
	//		easingDown->easingsActivated = false;
	//		easingUp->easingsActivated = true;
	//	}
	//}

	//menu->Update(dt);

	return ret;
}

void SceneEnding::Draw()
{
	app->render->DrawTexture(bg, 0, 0);
}

bool SceneEnding::UnLoad()
{
	LOG("Unloading Scene Ending");
	bool ret = true;

	app->tex->Unload(bg);
	//RELEASE(easingUp);
	//RELEASE(easingDown);
	return ret;
}