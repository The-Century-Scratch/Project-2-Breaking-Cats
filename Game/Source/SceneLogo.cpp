#include "App.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Audio.h"
#include "Animation.h"
#include "Easings.h"
#include "SceneManager.h"

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
	//easing1 = new Easing(true,0,-650,1930,180);
	//easing2 = new Easing(false,0,1280,-968,180);

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
	

	SDL_Rect s1 = { 0, 0, 184, 98 };
	SDL_Rect s2 = { 184 * 1, 0, 184, 98 };
	SDL_Rect s3 = { 184 * 2, 0, 184, 98 };
	SDL_Rect s4 = { 184 * 3, 0, 184, 98 };
	SDL_Rect s5 = { 184 * 4, 0, 184, 98 };
	SDL_Rect s6 = { 184 * 5, 0, 184, 98 };
	SDL_Rect s7 = { 184 * 6, 0, 184, 98 };
	SDL_Rect s8 = { 184 * 7, 0, 184, 98 };
	SDL_Rect s9 = { 184 * 8, 0, 184, 98 };
	SDL_Rect s10 = { 184 * 9, 0, 184, 98 };
	SDL_Rect s11 = { 184 * 10, 0, 184, 98 };
	SDL_Rect s12 = { 184 * 11, 0, 184, 98 };

	logoAnimation.PushBack(s1);
	logoAnimation.PushBack(s2);
	logoAnimation.PushBack(s3);
	logoAnimation.PushBack(s4);
	logoAnimation.PushBack(s5);
	logoAnimation.PushBack(s6);
	logoAnimation.PushBack(s7);
	logoAnimation.PushBack(s8);
	logoAnimation.PushBack(s9);
	logoAnimation.PushBack(s10);
	logoAnimation.PushBack(s11);
	logoAnimation.PushBack(s12);
	logoAnimation.speed = 0.005f;
	logoAnimation.loop = false;

	currentAnimation = &logoAnimation;

	timer = 30;

	return ret;
}

bool SceneLogo::Update(float dt)
{
	bool ret = true;

	//if (easing1->easingsActivated)
	//{
	//	logoPositionX = easing1->sineEaseOut(easing1->currentIteration, easing1->initialPos, easing1->deltaPos, easing1->totalIterations);
	//	logoPositionX2 = easing1->sineEaseOut(easing1->currentIteration, 1280, (easing1->deltaPos)*-1, easing1->totalIterations);
	//	if (easing1->currentIteration < easing1->totalIterations)
	//	{
	//		easing1->currentIteration++;
	//	}
	//	else
	//	{
	//		easing1->currentIteration = 0;
	//		easing1->easingsActivated = false;
	//		easing2->easingsActivated = true;
	//	}
	//}

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
	if (currentAnimation->HasFinished() == true) {
		TransitionToScene(SceneType::TITLE, TransitionType::ALTERNATING_BARS);
	}
	Draw();
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
	app->render->DrawTexture(logo, 720/6, 420/6, &rect);

}

bool SceneLogo::UnLoad()
{
	LOG("Unloading Scene Logo");
	bool ret = true;

	app->tex->Unload(logo);
	//RELEASE(easing1);
	//RELEASE(easing2);

	return ret;
}