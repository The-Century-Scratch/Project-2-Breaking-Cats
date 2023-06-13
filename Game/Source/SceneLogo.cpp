#include "App.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Audio.h"
#include "Animation.h"
#include "Easings.h"
#include "SceneManager.h"
#include "Hud.h"
#include "SceneLogo.h"

#include "Log.h"

#define LOGO_FADE_IN_SPEED 0.4f
#define LOGO_FADE_OUT_SPEED 1.0f

SceneLogo::SceneLogo()
{
	//logo = nullptr;

	//state = 0;
	//timeCounter = 0.0f;
	//logoAlpha = 1.0f;
	//drawCounter = 0.75f;

	////Easings
	easing = new Easing(true,0,-40,40,100);

	//logoPositionX = -650.0f;
	//logoPositionX = 1280.0f;

	//channel = app->audio->SetChannel();

	//showColliders = false;
}

bool SceneLogo::Load()
{
	LOG("Loading Scene Logo");
	bool ret = true;
	//logoFx = app->audio->LoadFx("Audio/Fx/logo_intro.wav");

	logo = app->tex->Load(app->LoadConfigFileToVar().child("sceneLogo").child("img").attribute("texturepath").as_string());
	logofx = app->audio->LoadFx(app->LoadConfigFileToVar().child("sceneLogo").child("logofx").attribute("path").as_string());
	
	//SDL_Rect s1 =	{ 184 * 0, 0, 184, 98 } ;
	//SDL_Rect s2 =	{ 184 * 1, 0, 184, 98 } ;
	//SDL_Rect s3 =	{ 184 * 2, 0, 184, 98 } ;
	//SDL_Rect s4 =	{ 184 * 3, 0, 184, 98 } ;
	//SDL_Rect s5 =	{ 184 * 4, 0, 184, 98 } ;
	//SDL_Rect s6 =	{ 184 * 5, 0, 184, 98 } ;
	//SDL_Rect s7 =	{ 184 * 6, 0, 184, 98 } ;
	//SDL_Rect s8 =	{ 184 * 7, 0, 184, 98 } ;
	//SDL_Rect s9 =	{ 184 * 8, 0, 184, 98 } ;
	//SDL_Rect s10 =	{ 184 * 9, 0, 184, 98 } ;
	//SDL_Rect s11 =	{ 184 * 10, 0, 184, 98} ;
	//SDL_Rect s12 =	{ 184 * 11, 0, 184, 98} ;

	logoAnimation.PushBack({ 184 * 0, 0, 184, 98 });
	logoAnimation.PushBack({ 184 * 1, 0, 184, 98 });
	logoAnimation.PushBack({ 184 * 2, 0, 184, 98 });
	logoAnimation.PushBack({ 184 * 3, 0, 184, 98 });
	logoAnimation.PushBack({ 184 * 4, 0, 184, 98 });
	logoAnimation.PushBack({ 184 * 5, 0, 184, 98 });
	logoAnimation.PushBack({ 184 * 6, 0, 184, 98 });
	logoAnimation.PushBack({ 184 * 7, 0, 184, 98 });
	logoAnimation.PushBack({ 184 * 8, 0, 184, 98 });
	logoAnimation.PushBack({ 184 * 9, 0, 184, 98 });
	logoAnimation.PushBack({ 184 * 10, 0, 184, 98 });
	logoAnimation.PushBack({ 184 * 11, 0, 184, 98 });
	logoAnimation.PushBack({ 184 * 0, 0, 184, 98 });
	logoAnimation.speed = 0.3f;
	logoAnimation.loop = false;


	logoAnimationStatic.PushBack({ 184 * 0, 0, 184, 98 });

	currentAnimation = &logoAnimationStatic;

	timer = 0;

	return ret;
}

bool SceneLogo::Update(float dt)
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) 
	{
		TransitionToScene(SceneType::TITLE, TransitionType::WIPE);
		SDL_ShowCursor(SDL_ENABLE);
	}
	if (CONTROLLERA || CONTROLLERB || CONTROLLERSTART)
	{
		TransitionToScene(SceneType::TITLE, TransitionType::WIPE);
		SDL_ShowCursor(SDL_DISABLE);
	}

	if (timer == 200)
	{
		TransitionToScene(SceneType::TITLE, TransitionType::WIPE);
	}
	else if (timer > 101)
	{
		currentAnimation = &logoAnimation;
	}
	else if (timer == 100)
	{
		app->audio->PlayFx(logofx);
	}
	else if (timer > 0)
	{
		if (easing->easingsActivated)
		{
			Pos = easing->bounceEaseOut(easing->currentIteration, easing->initialPos, easing->deltaPos, easing->totalIterations);
			if (easing->currentIteration < easing->totalIterations)
			{
				easing->currentIteration++;
			}
			else
			{
				easing->currentIteration = 0;
				easing->easingsActivated = false;
			}
		}
	}

	//if (easing2->easingsActivated)
	//{
	//	logoPositionX = easing2->elasticEaseOut(easing2->currentIteration, easing2->initialPos, easing2->deltaPos, easing2->totalIterations);
	//	logoPositionX2 = easing2->elasticEaseOut(easing2->currentIteration, easing1->initialPos, 965, easing2->totalIterations);
	//	if (easing2->currentIteration < easing2->totalIterations)
	//	{
	//		easing2->currentIteration++;
	//	}
	//	else
	//	{
	//		easing2->currentIteration = 0;
	//		easing2->easingsActivated = false;
	//	}
	//}

	//switch (state)
	//{
	//case 0:
	//	state = 1;
	//	break;
	//case 1:

	//	drawCounter -= dt;

	//	if (drawCounter <= 0)
	//	{
	//		app->audio->PlayFx(channel, logoFx);
	//		logoAlpha -= (LOGO_FADE_IN_SPEED * dt);
	//	}
	//	if (logoAlpha < 0.0f)
	//	{
	//		logoAlpha = 0.0f;
	//		state = 2;
	//	}
	//	break;
	//case 2:
	//	timeCounter += dt;
	//	if (timeCounter >= 1.5f) state = 3;
	//	break;
	//case 3:
	//	logoAlpha += (LOGO_FADE_OUT_SPEED * dt);

	//	if (logoAlpha > 1.0f)
	//	{
	//		logoAlpha = 1.0f;
	//		TransitionToScene(SceneType::TITLE, TransitionType::FADE_TO_BLACK);
	//	}
	//	break;
	//}
	timer++;

	currentAnimation->Update();
	return ret;
}


void SceneLogo::Draw()
{
	//if (drawCounter <= 0)
	//{
	//	drawCounter = 0;
	//	app->render->DrawRectangle({ 0,0,1280,720 }, 255, 255, 255, 255);

	//	SDL_Rect rect = {0,0,651,325};
	//	app->render->DrawTexture(logo, logoPositionX, 35, &rect);
	//	rect = {0,325,651,325};
	//	app->render->DrawTexture(logo, logoPositionX2, 35+325, &rect);
	//}

	//// Fade in Logo
	//if (state == 1) app->render->DrawRectangle({ 0, 0, 1280, 720 }, 0, 0, 0, 255 * logoAlpha);
	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(logo, 0, Pos, &rect, 1.0f, 0.0, 2147483647, 2147483647, true, 7);

}

bool SceneLogo::UnLoad()
{
	LOG("Unloading Scene Logo");
	bool ret = true;

	app->tex->Unload(logo);
	RELEASE(easing);
	//RELEASE(easing2);

	return ret;
}