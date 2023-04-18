#include "Scene_Logo.h"
#include "Event_Base.h"
#include "Audio.h"

#include "Log.h"
#include "Render.h"

bool Scene_Logo::isReady()
{
	return true;
}

void Scene_Logo::Load(std::string const& path, LookUpXMLNodeFromString const& info, Window_Factory const &windowFactory, std::string const fileToLoad)
{
	logo = app->tex->Load("Assets/Textures/TheCenturyScratchTitleAnimated12frames.png");
}

void Scene_Logo::Start()
{
	timer = 30;
}

void Scene_Logo::Draw()
{
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
	if (timer < 345)
	{
		app->render->DrawTexture(DrawParameters(logo, { 644, timer }).Section(&s1), true);
	}
	else if (timer < 360)
	{
		app->render->DrawTexture(DrawParameters(logo, { 644, 345 }).Section(&s1), true);
	}
	else if (timer < 355)
	{
		app->render->DrawTexture(DrawParameters(logo, { 644, 345 }).Section(&s2), true);
	}
	else if (timer < 360)
	{
		app->render->DrawTexture(DrawParameters(logo, { 644, 345 }).Section(&s3), true);
	}
	else if (timer < 365)
	{
		app->render->DrawTexture(DrawParameters(logo, { 644, 345 }).Section(&s4), true);
	}
	else if (timer < 370)
	{
		app->render->DrawTexture(DrawParameters(logo, { 644, 345 }).Section(&s5), true);
	}
	else if (timer < 375)
	{
		app->render->DrawTexture(DrawParameters(logo, { 644, 345 }).Section(&s6), true);
	}
	else if (timer < 380)
	{
		app->render->DrawTexture(DrawParameters(logo, { 644, 345 }).Section(&s7), true);
	}
	else if (timer < 385)
	{
		app->render->DrawTexture(DrawParameters(logo, { 644, 345 }).Section(&s8), true);
	}
	else if (timer < 390)
	{
		app->render->DrawTexture(DrawParameters(logo, { 644, 345 }).Section(&s9), true);
	}
	else if (timer < 395)
	{
		app->render->DrawTexture(DrawParameters(logo, { 644, 345 }).Section(&s10), true);
	}
	else if (timer < 400)
	{
		app->render->DrawTexture(DrawParameters(logo, { 644, 345 }).Section(&s11), true);
	}
	else if (timer < 405)
	{
		app->render->DrawTexture(DrawParameters(logo, { 644, 345 }).Section(&s12), true);
	}
	else
	{
		app->render->DrawTexture(DrawParameters(logo, { 644, 345 }).Section(&s1), true);
	}
}

int Scene_Logo::Update()
{
	int ret = 0;
	/*if (timer == 0)
		app->audio->PlayFx(app->audio->logoFx);*/
	timer++;
	if (timer > 500) return 6;
	return CheckNextScene(ret);
}

int Scene_Logo::CheckNextScene(int ret)
{
	using enum SceneType;
	using enum KeyState;
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP)
	{
		return static_cast<int>(SceneType::TITLESCENE);
	}

	return ret;
}

void Scene_Logo::DrawPause()
{
}
